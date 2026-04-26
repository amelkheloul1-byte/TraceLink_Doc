/**
 * @file ExtracteurFichier.cpp
 * @brief Module 1 : Import & Parsing
 */

#include "ExtracteurFichier.h"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <zip.h>
#include <pugixml.hpp>
#include <cstdint>
#include <cctype>
#include <sstream>
#include <algorithm>

/**
 * @brief Constructeur de l'extracteur de fichier.
 * @param chemin Chemin du fichier à analyser.
 * @param motif  Regex pour reconnaître les identifiants d'exigences.
 */
ExtracteurFichier::ExtracteurFichier(const std::string& chemin,
                                     const std::string& motif)
    : cheminSource(chemin),
    motifRegex(motif),
    nbDetectees(0)
{
    if (chemin.empty()) {
        throw FileNotFoundException("(chemin vide)");
    }
}

/**
 * @brief Orchestre l'extraction complète :
 *        1. Vérifie l'existence du fichier
 *        2. Détecte l'extension
 *        3. Appelle le parseur adapté
 *        4. Retourne la liste structurée
 */
std::vector<ExigenceExtraite> ExtracteurFichier::extraire()
{
    std::vector<ExigenceExtraite> resultats;

    verifierExistence();

    std::string ext = detecterExtension();

    std::vector<std::pair<int, std::string>> lignes;

    if (ext == "csv" || ext == "xls") {
        lignes = lireCSV(ext);
    } else if (ext == "docx") {
        lignes = lireDocx();
    }else if (ext=="xlsx") {
        lignes = lireXlsx();
    
    } else {
        throw FileFormatException("Format non supporté : '." + ext + "'. "
                                                                     "Formats acceptés : .docx, .csv, .xls");
    }

    if (ext == "docx") {
        resultats = parserBlocsDocx(lignes);
    } else {
        resultats = filtrerLignes(lignes, ext);
    }

    nbDetectees = resultats.size();
    std::cout << "[Module extraction] " << nbDetectees
              << " exigence(s) détectée(s) dans « "
              << cheminSource << " »." << std::endl;

    return resultats;
}

/**
 * @brief Retourne le nombre d'exigences détectées lors de la dernière extraction.
 */
int ExtracteurFichier::getNombreExigencesDetectees() const
{
    return nbDetectees;
}

// ─── Méthodes privées ─────────────────────────────────────────────────────────

/**
 * @brief Retourne l'extension du fichier en minuscules.
 */
std::string ExtracteurFichier::detecterExtension() const
{
    size_t pos = cheminSource.rfind('.');
    if (pos == std::string::npos) return "";

    std::string ext = cheminSource.substr(pos + 1);
    for (size_t i = 0; i < ext.length(); ++i)
        ext[i] = std::tolower(static_cast<unsigned char>(ext[i]));
    return ext;
}

/**
 * @brief Vérifie si le fichier source est présent sur le disque.
 * @throw FileNotFoundException Si le fichier n'existe pas ou n'est pas accessible.
 */
void ExtracteurFichier::verifierExistence() const
{
    std::ifstream test(cheminSource);
    if (!test.is_open())
        throw FileNotFoundException(cheminSource);
}

/**
 * @brief Lit un fichier texte tabulaire (.csv ou .xls) ligne par ligne.
 *
 * Retourne chaque ligne brute (sans concaténation des colonnes)
 * pour que filtrerLignes() puisse traiter les colonnes séparément.
 * Le délimiteur (virgule ou point-virgule) est auto-détecté.
 * Les lignes entièrement vides sont ignorées.
 *
 * @param extension "csv" ou "xls"
 * @return Vecteur de (numéroLigne, ligne brute).
 */
std::vector<std::pair<int, std::string>> ExtracteurFichier::lireCSV(const std::string&) const
{
    std::ifstream fichier(cheminSource);
    if (!fichier.is_open())
        throw FileNotFoundException(cheminSource);

    std::vector<std::pair<int, std::string>> lignes;
    std::string ligneBrute;
    int numeroLigne = 0;

    while (std::getline(fichier, ligneBrute)) {
        numeroLigne++;

        // Nettoyer le \r final (fichiers Windows)
        if (!ligneBrute.empty() && ligneBrute.back() == '\r')
            ligneBrute.pop_back();

        // Ignorer les lignes vides
        if (ligneBrute.find_first_not_of(" \t\r\n") == std::string::npos)
            continue;

        lignes.push_back(std::make_pair(numeroLigne, ligneBrute));
    }

    fichier.close();
    return lignes;
}

/**
 * @brief Charge le dictionnaire des textes partagés d'Excel.
 * * Dans un .xlsx, les textes ne sont pas stockés directement dans les cellules
 * pour gagner de la place. Ils sont regroupés dans 'xl/sharedStrings.xml'.
 * * Cette méthode :
 * 1. Ouvre le fichier de dictionnaire dans l'archive ZIP.
 * 2. Lit tout le contenu XML.
 * 3. Stocke chaque texte trouvé dans un vecteur où l'indice correspond à l'ID Excel.
 * * @param archive Pointeur vers l'archive ZIP ouverte (libzip).
 * @return Un vecteur de chaînes de caractères servant de table de correspondance.
 */
std::vector<std::string> ExtracteurFichier::chargerSharedStrings(zip_t* archive) const {
    std::vector<std::string> catalogue;
    zip_file_t* f = zip_fopen(archive, "xl/sharedStrings.xml", 0);
    if (!f) return catalogue;

    std::string contenu;
    char buffer[4096];
    zip_int64_t n;
    while ((n = zip_fread(f, buffer, sizeof(buffer))) > 0) 
        contenu.append(buffer, (size_t)n);
    zip_fclose(f);

    pugi::xml_document doc;
    doc.load_string(contenu.c_str());
    
    for (pugi::xpath_node t_xpath : doc.select_nodes("//t")) {
        catalogue.push_back(t_xpath.node().child_value());
    }
    return catalogue;
}

/**
 * @brief Extrait le texte brut d'un fichier .xlsx ligne par ligne.
 * * Un fichier .xlsx est une archive ZIP contenant xl/worksheets/sheet1.xml.
 * Cette méthode décompresse l'archive, charge le dictionnaire de textes, 
 * et reconstruit les lignes du tableau.
 */
std::vector<std::pair<int, std::string>> ExtracteurFichier::lireXlsx() const {
    std::vector<std::pair<int, std::string>> lignes;
    int erreur = 0;
    // Ouverture du fichier XLSX comme une archive ZIP
    zip_t* archive = zip_open(cheminSource.c_str(), ZIP_RDONLY, &erreur);
    if (!archive) throw FileFormatException("Impossible d'ouvrir l'archive : " + cheminSource);

    // Chargement du dictionnaire "Shared Strings" (indispensable pour lire le texte Excel)
    std::vector<std::string> catalogueTextes = chargerSharedStrings(archive);

    // Extraction du fichier XML de la première feuille de calcul
    zip_file_t* xmlFichier = zip_fopen(archive, "xl/worksheets/sheet1.xml", 0);
    if (!xmlFichier) {
        zip_close(archive);
        throw FileFormatException("Fiche sheet1.xml introuvable");
    }

    // Lecture du flux de données XML vers une chaîne de caractères
    std::string xmlContenu;
    char buffer[4096];
    zip_int64_t nbLus;
    while ((nbLus = zip_fread(xmlFichier, buffer, sizeof(buffer))) > 0)
        xmlContenu.append(buffer, (size_t)nbLus);

    zip_fclose(xmlFichier);
    zip_close(archive); // On peut fermer l'archive, on a tout en mémoire

    // Analyse de la structure XML avec pugixml
    pugi::xml_document doc;
    doc.load_string(xmlContenu.c_str());

    int numLigne = 0;
    // On récupère toutes les balises <row> (les lignes du tableau)
    pugi::xpath_node_set rows = doc.select_nodes("//row");

    for (pugi::xpath_node row_xpath : rows) {
        pugi::xml_node row = row_xpath.node();
        std::string texteLigne;

        // Pour chaque ligne, on parcourt les cellules <c>
        for (pugi::xpath_node cell_xpath : row.select_nodes("c")) {
            pugi::xml_node cell = cell_xpath.node();
            
            // On vérifie si la cellule contient un texte partagé (type "s")
            std::string type = cell.attribute("t").as_string();
            std::string valeur = cell.child("v").text().as_string();

            if (type == "s" && !valeur.empty()) {
                // Si c'est un "Shared String", la valeur est un index (ex: "0")
                // On va chercher le vrai texte dans notre catalogue
                int index = std::stoi(valeur);
                if (index < (int)catalogueTextes.size()) {
                    texteLigne += catalogueTextes[index] + ";"; // Séparateur pour filtrerLignes()
                }
            } else if (!valeur.empty()) {
                // Sinon, c'est une valeur directe (nombre ou texte brut)
                texteLigne += valeur + ";";
            }
        }
        
        // Si la ligne n'est pas vide, on l'ajoute aux résultats
        if (!texteLigne.empty()) {
            numLigne++;
            lignes.push_back(std::make_pair(numLigne, texteLigne));
        }
    }
    return lignes;
}
/**
 * @brief Extrait le texte brut d'un fichier .docx paragraphe par paragraphe.
 *
 * Un fichier .docx est une archive ZIP contenant word/document.xml.
 * Ouvre l'archive avec libzip, parse le XML avec pugixml,
 * et retourne un vecteur de (numéroLigne, texte du paragraphe).
 *
 * @return Vecteur de (numéroLigne, texte du paragraphe).
 * @throw FileFormatException si le fichier n'est pas un ZIP valide.
 */
std::vector<std::pair<int, std::string>> ExtracteurFichier::lireDocx() const
{
    std::vector<std::pair<int, std::string>> lignes;

    // Ouverture de l'archive ZIP
    int erreur = 0;
    zip_t* archive = zip_open(cheminSource.c_str(), ZIP_RDONLY, &erreur);
    if (archive == nullptr)
        throw FileFormatException("Impossible d'ouvrir l'archive ZIP : " + cheminSource);

    // Localiser word/document.xml
    zip_file_t* xmlFichier = zip_fopen(archive, "word/document.xml", 0);
    if (xmlFichier == nullptr) {
        zip_close(archive);
        throw FileFormatException("word/document.xml introuvable dans : " + cheminSource);
    }

    // Lecture du contenu XML en mémoire
    std::string xmlContenu;
    char buffer[4096];
    zip_int64_t nbLus = 0;
    while ((nbLus = zip_fread(xmlFichier, buffer, sizeof(buffer))) > 0)
        xmlContenu.append(buffer, static_cast<size_t>(nbLus));

    zip_fclose(xmlFichier);
    zip_close(archive);

    if (xmlContenu.empty())
        throw FileFormatException("word/document.xml est vide dans : " + cheminSource);

    // Parser le XML avec pugixml
    pugi::xml_document doc;
    doc.load_string(xmlContenu.c_str());

    // Extraire le texte des paragraphes
    int numParagraphe = 0;
    for (int i = 0; i < (int)doc.select_nodes("//w:p").size(); i++) {
        pugi::xml_node para = doc.select_nodes("//w:p")[i].node();

        std::string texteParagraphe;
        for (int j = 0; j < (int)para.select_nodes(".//w:t").size(); j++) {
            pugi::xml_node t = para.select_nodes(".//w:t")[j].node();
            texteParagraphe += t.child_value();
        }

        if (texteParagraphe.find_first_not_of(" \t\r\n") == std::string::npos)
            continue;

        numParagraphe++;
        lignes.push_back(std::make_pair(numParagraphe, texteParagraphe));
    }

    return lignes;
}

/**
 * @brief Filtre les lignes CSV/XLS colonne par colonne.
 *
 * Pour chaque ligne :
 *   - Colonne 1 : identifiant principal (doit matcher la regex)
 *   - Colonne 2 : lien de traçabilité si c'est un identifiant valide
 *     (commence par EXIGENCE_ ou SDD_)
 *   - Colonnes suivantes : contenu textuel ignoré
 *
 * @param lignes  Résultat de lireCSV().
 * @param format  "csv" ou "xls".
 * @return Liste des exigences extraites avec métadonnées.
 */
std::vector<ExigenceExtraite> ExtracteurFichier::filtrerLignes(
    const std::vector<std::pair<int, std::string>>& lignes,
    const std::string& format) const
{
    std::vector<ExigenceExtraite> resultats;
    std::vector<std::string> dejaTrouves;
    std::string nomFichier = cheminSource;

    for (size_t i = 0; i < lignes.size(); i++) {
        int numLigne    = lignes[i].first;
        std::string texte = lignes[i].second;

        // Détecter le délimiteur : ';' ou ','
        char delim = (std::count(texte.begin(), texte.end(), ';') >=
                      std::count(texte.begin(), texte.end(), ',')) ? ';' : ',';

        // Découper en colonnes
        std::vector<std::string> colonnes;
        std::istringstream fluxCol(texte);
        std::string col;
        while (std::getline(fluxCol, col, delim)) {
            // Nettoyer espaces et guillemets
            col.erase(0, col.find_first_not_of(" \t\r\n\""));
            size_t last = col.find_last_not_of(" \t\r\n\"");
            if (last != std::string::npos) col = col.substr(0, last + 1);
            colonnes.push_back(col);
        }

        if (colonnes.empty()) continue;

        // Colonne 1 = identifiant principal
        std::string idPrincipal = colonnes[0];
        std::regex reg(motifRegex);
        std::smatch match;

        // Vérifier que la colonne 1 commence bien par un ID valide
        if (!std::regex_search(idPrincipal, match, reg)) continue;
        idPrincipal = match.str(); // Prendre uniquement le match

        // Vérifier doublon
        bool dejaVu = false;
        for (const auto& d : dejaTrouves) {
            if (d == idPrincipal) { dejaVu = true; break; }
        }
        if (dejaVu) continue;

        ExigenceExtraite ex;
        ex.identifiant   = idPrincipal;
        ex.contenuBrut   = texte;
        ex.fichierSource = nomFichier;
        ex.formatSource  = format;
        ex.numeroLigne   = numLigne;

        // Colonne 2 = lien de traçabilité si c'est un ID valide
        if (colonnes.size() >= 2) {
            std::string col2 = colonnes[1];
            std::string upper2 = col2;
            std::transform(upper2.begin(), upper2.end(), upper2.begin(), ::toupper);
            bool estIdValide = upper2.find("EXIGENCE_") == 0
                               || upper2.find("SDD_") == 0;
            std::smatch m;
            if (estIdValide && std::regex_search(col2, m, reg) && col2 != idPrincipal) {
                ex.tracabilite.push_back(col2);
            }
        }

        resultats.push_back(ex);
        dejaTrouves.push_back(idPrincipal);
    }

    return resultats;
}

/**
 * @brief Regroupe les paragraphes docx en blocs et extrait les métadonnées.
 *
 * Un bloc commence sur un identifiant (matché par regex) et se termine sur "#".
 * Extrait : tracabilite, cibles, necessaireA, alloueA, developpe.
 *
 * @param paragraphes Résultat de lireDocx().
 * @return Liste des exigences extraites avec métadonnées.
 */
std::vector<ExigenceExtraite> ExtracteurFichier::parserBlocsDocx(
    const std::vector<std::pair<int, std::string>>& paragraphes) const
{
    std::vector<ExigenceExtraite> resultats;
    std::regex reg(motifRegex);

    ExigenceExtraite exCourante;
    bool dansBloc = false;

    for (const auto& para : paragraphes) {
        std::string texte = para.second;

        // Séparateur de bloc "#"
        if (texte.find("#") != std::string::npos) {
            if (dansBloc) {
                resultats.push_back(exCourante);
                exCourante = ExigenceExtraite();
                dansBloc = false;
            }
            continue;
        }

        // Détection d'un identifiant principal
        std::smatch match;
        if (!dansBloc && std::regex_search(texte, match, reg)) {
            exCourante = ExigenceExtraite();
            exCourante.identifiant   = match.str();
            exCourante.contenuBrut   = texte;
            exCourante.fichierSource = cheminSource;
            exCourante.formatSource  = "docx";
            exCourante.numeroLigne   = para.first;
            dansBloc = true;
            continue;
        }

        if (dansBloc) {
            // Ligne "Traçabilité : EXIGENCE_CLIENT_A_01, ..."
            if (texte.find("Tra") != std::string::npos
                && texte.find("abilit") != std::string::npos) {
                std::string::const_iterator it(texte.cbegin());
                std::smatch m;
                while (std::regex_search(it, texte.cend(), m, reg)) {
                    std::string id = m.str();
                    std::string upper = id;
                    std::transform(upper.begin(), upper.end(), upper.begin(), ::toupper);
                    bool estIdValide = upper.find("EXIGENCE_") == 0
                                       || upper.find("SDD_") == 0;
                    if (estIdValide) {
                        exCourante.tracabilite.push_back(id);
                    }
                    it = m.suffix().first;
                }
            }
            // Ligne "Cible : ..."
            else if (texte.find("Cible") != std::string::npos) {
                exCourante.cibles = decouper(texte.substr(texte.find(":") + 1));
            }
            // Ligne "Nécessaire à : ..."
            else if (texte.find("cessaire") != std::string::npos) {
                exCourante.necessaireA = decouper(texte.substr(texte.find(":") + 1));
            }
            // Ligne "Alloué à : ..."
            else if (texte.find("llou") != std::string::npos) {
                exCourante.alloueA = decouper(texte.substr(texte.find(":") + 1));
            }
            // Ligne "Développé" ou "Non développé"
            else if (texte.find("velopp") != std::string::npos) {
                exCourante.developpe = (texte.find("Non") == std::string::npos);
            }
        }
    }

    // Dernier bloc sans "#" final
    if (dansBloc)
        resultats.push_back(exCourante);

    return resultats;
}

/**
 * @brief Découpe une chaîne "val1, val2, val3" en vecteur de tokens nettoyés.
 * @param ligne La chaîne à découper.
 * @return Vecteur de tokens.
 */
std::vector<std::string> ExtracteurFichier::decouper(const std::string& ligne) const
{
    std::vector<std::string> tokens;
    std::istringstream flux(ligne);
    std::string token;
    while (std::getline(flux, token, ',')) {
        size_t debut = token.find_first_not_of(" \t\r\n");
        size_t fin   = token.find_last_not_of(" \t\r\n");
        if (debut != std::string::npos)
            tokens.push_back(token.substr(debut, fin - debut + 1));
    }
    return tokens;
}
