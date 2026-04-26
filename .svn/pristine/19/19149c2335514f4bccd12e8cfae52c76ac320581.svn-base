/** 
*@file ExtracteurFichier.cpp
* @brief Module 1 : Import & Parsing
*/

#include "ExtracteurFichier.h"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <zip.h>
#include <pugixml.hpp>
#include <cstdint>
#include <cctype>    // Pour std::tolower
#include <sstream>    

//Constructeur
/**
 * @param chemin  Chemin du fichier à analyser.
 * @param motif   Regex pour reconnaître les identifiants d'exigences.
 *              
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

//Méthode extraire()
/**
 * @brief Orchestre l'extraction complète :
 *        1. Vérifie l'existence du fichier
 *        2. Détecte l'extension
 *        3. Appelle le parseur adapté
 *        4. Filtre les identifiants par regex
 *        5. Retourne la liste structurée
 */
std::vector<ExigenceExtraite> ExtracteurFichier::extraire(){

    std::vector<ExigenceExtraite> resultats;

    verifierExistence();

    std::string ext = detecterExtension(); 

    std::vector<std::pair<int,std::string>> lignes;

    if (ext == "csv" || ext == "xls") {
        lignes = lireCSV(ext);
    }else if (ext == "docx") {
        lignes = lireDocx();
    }else {
        throw FileFormatException("Format non supporté : '." + ext + "'. "
            "Formats acceptés : .docx, .csv, .xls");
    }
    resultats = filtrerLignes(lignes, ext);
    nbDetectees = resultats.size();  
    std::cout << "[Module extraction] " << nbDetectees
              << " exigence(s) détectée(s) dans « "
              << cheminSource<< " »." << std::endl;

    return resultats;

}

int ExtracteurFichier::getNombreExigencesDetectees() const
{
    return nbDetectees;
}

//méthodes privées

/**
 * @brief Retourne l'extension du fichier en minuscules.
 */
std::string ExtracteurFichier::detecterExtension() const
{
    size_t pos = cheminSource.rfind('.'); //On cherche où se trouve le dernier point dans le nom du fichier.
    if (pos == std::string::npos) return "";//Si on n'a pas trouvé de point dans le nom du fichier, on retourne une chaîne vide.

    std::string ext = cheminSource.substr(pos + 1); //extrait une partie de la chaine de caractère de pour créer une nouvelle :ext
    for (size_t i = 0; i < ext.length(); ++i) { // On parcourt chaque caractère de la chaîne 'ext'
        ext[i] = std::tolower(static_cast<unsigned char>(ext[i]));  // On transforme le caractère à la position 'i' en minuscule
    }
return ext;
}





/**
 * @brief Vérifie si le fichier source est présent sur le disque.
 * @throw FileNotFoundException Si le fichier n'existe pas ou n'est pas accessible.
 */
void ExtracteurFichier::verifierExistence() const {
    
    std::ifstream test(cheminSource); //on essaye d'ouvrir le fichier en lecture 
    if (!test.is_open()) { //on vérifié si l'ouverture s'est bien passée 
        throw FileNotFoundException(cheminSource); //On lance une alerte avec le nom du fichier problématique.
    }
}

 //Parseur CSV/XLS
 /**
 * @brief Lit un fichier texte tabulaire (.csv ou .xls).
 *
 * Algorithme :
 *   - Ouvre le fichier en mode texte.
 *   - Lit ligne par ligne avec std::getline.
 *   - Pour chaque ligne, teste les délimiteurs ';' et ',' et choisit
 *     celui qui génère le plus de colonnes (auto-détection).
 *   - Concatène toutes les cellules en une chaîne unique par ligne.
 *   - Ignore les lignes entièrement vides.
 *
 * @return Vecteur de (numéroLigne, texte concaténé).
 */
 std::vector<std::pair<int, std::string>> ExtracteurFichier::lireCSV(const std::string&) const{
  
    std::ifstream fichier (cheminSource);
    if(!fichier.is_open()){
        throw FileNotFoundException(cheminSource);
    }

    std::vector<std::pair<int, std::string>> lignes;
    std::string ligneBrute;
    int numeroLigne = 0;

    while (std::getline(fichier, ligneBrute)){ 
        numeroLigne++;
        // Ignorer les lignes vides 
         if (ligneBrute.find_first_not_of(" \t\r\n") == std::string::npos) {
            continue;
        }
        // Déclaration du délimiteur : ";" ou ","
        char delimiteur = ';';
        int comptePointVirgule = std::count(ligneBrute.begin(), ligneBrute.end(), ';'); //parcous toute la ligne et compte ; 
        int compteVirgule = std::count(ligneBrute.begin(), ligneBrute.end(), ',');
        if (compteVirgule > comptePointVirgule) {
            delimiteur = ',';
        }      
       // Découpage en colonnes et concaténation 
        // On utilise un flux pour découper la ligne
        std::istringstream flux(ligneBrute);
        std::string cellule;
        std::string ligneConcatenee = ""; 

        // On lit chaque case (cellule) séparée par le délimiteur
        while (std::getline(flux, cellule, delimiteur)) {
            
            // Nettoyage des guillemets
            if (cellule.length() >= 2) { // Vérifier que le mot a au moins 2 caractères
                
                if (cellule[0] == '"' && cellule[cellule.length() - 1] == '"') { // Regarder si la cellule est emballé dans des guillemets 
                    cellule = cellule.substr(1, cellule.length() - 2);
                } // On garde le texte sans les guillemets 
            }

            // Recomposition de la ligne
            if (!cellule.empty()) { // Vérifier que la cellule n'est pas vide
                if (ligneConcatenee != "") { // Vérifier si la grande ligne n'est pas vide
                    ligneConcatenee = ligneConcatenee + " "; //On ajoute un espace
                }
                ligneConcatenee = ligneConcatenee + cellule; // On ajoute le mot
            }
        }

        // Sauvegarde de la ligne avec son numéro
        if (ligneConcatenee != "") {
            lignes.push_back(std::make_pair(numeroLigne, ligneConcatenee));
        }
    }
    fichier.close();
    return lignes;
}
 

/**
 * @brief Extrait le texte brut d'un fichier .docx.
 *
 * Un fichier .docx est en réalité une archive ZIP.
 * À l'intérieur, il y a un fichier word/document.xml qui contient tout le texte.
 *
 * Algorithme :
 *   - Ouvre l'archive ZIP avec libzip
 *   - Localise word/document.xml dans l'archive
 *   - Lit le contenu XML et on le met en mémoire
 *   - Parse le XML avec pugixml
 *   - Extrait le texte de chaque paragraphe
 */
std::vector<std::pair<int, std::string>> ExtracteurFichier::lireDocx() const
{
    std::vector<std::pair<int, std::string>> lignes; 

    //Ouverture de l'archive ZIP 
    int erreur = 0;
    zip_t* archive = zip_open(cheminSource.c_str(), ZIP_RDONLY, &erreur );

    if (archive == nullptr){
        throw FileFormatException( "Impossible d'ouvrir l'archive ZIP : " + cheminSource);
    }

    //Localiser document.xml
    zip_file_t* xmlFichier = zip_fopen(archive, "word/document.xml", 0);

    if (xmlFichier == nullptr) { 
        zip_close(archive); 
        throw FileFormatException("word/document.xml introuvable dans : " + cheminSource);
    }

    //Lecture du contenu XML en mémoire 
    std::string xmlContenu = "";
    char buffer[4096]; //on lir le fichier XML par morceaux de 4096 carcatères 
    zip_int64_t nbLus = 0;

    while ((nbLus=zip_fread(xmlFichier, buffer, sizeof(buffer)))>0){ //Tant qu'il reste des caractères à lire
        xmlContenu.append(buffer, static_cast<size_t>(nbLus));
    }

    zip_fclose(xmlFichier);
    zip_close(archive);

     // Si le XML est vide 
    if (xmlContenu.empty()) {
        throw FileFormatException("word/document.xml est vide dans : " + cheminSource);
    }

    //Parser le XML avec pugixml 
    pugi::xml_document doc;
    pugi::xml_parse_result resultat = doc.load_string(xmlContenu.c_str());

    //Extraire le texte des paragraphes 
    int numParagraphe = 0;
    for (int i = 0; i < (int)doc.select_nodes("//w:p").size(); i++) {
        pugi::xml_node para = doc.select_nodes("//w:p")[i].node();

        // Pour chaque paragraphe on récupère tous les morceaux de texte <w:t>
        std::string texteParagraphe = "";

        for (int j = 0; j < (int)para.select_nodes(".//w:t").size(); j++) {
            pugi::xml_node t = para.select_nodes(".//w:t")[j].node();
            texteParagraphe = texteParagraphe + t.child_value(); // on colle les morceaux
        }

        // On ignore les paragraphes vides
        if (texteParagraphe.find_first_not_of(" \t\r\n") == std::string::npos) {
            continue;
        }

        // On ajoute le paragraphe avec son numéro
        numParagraphe++;
        lignes.push_back(std::make_pair(numParagraphe, texteParagraphe));
    }

    return lignes;   
}

//Filtrage par expression régulière 
/**
 * @brief Cherche toutes les occurrences du motif regex dans chaque ligne.
 *
 * Algorithme :
 *   - Pour chaque ligne du vecteur d'entrée, lance std::sregex_search pour trouver le premier identifiant. 
 *   - Vérifie si l'identifiant trouvé existe déjà dans une liste de mémorisation afin de bloquer les doublons.
 *   - Chaque match génère un objet ExigenceExtraite renseigné avec :
 *     identifiant  = chaîne matchée
 *     contenuBrut  = ligne complète
 *     fichierSource, formatSource, numeroLigne
 *
 * @param lignes  Résultat de lireCSV() ou lireDocx().
 * @param format  "csv", "xls" ou "docx".
 * @return Liste de toutes les exigences détectées.
 */

 std::vector<ExigenceExtraite> ExtracteurFichier::filtrerLignes(
    const std::vector<std::pair<int, std::string>>& lignes, const std::string& format) const {

    std::vector<ExigenceExtraite> resultats;
    std::vector<std::string> dejaTrouves; // Ta liste de mémorisation !

    std::string nomFichier = cheminSource; 

    for (size_t i = 0; i < lignes.size(); i++) {
        int numLigne = lignes[i].first;   
        std::string texte  = lignes[i].second;  

        std::smatch match;

        // regex_search s'arrête au PREMIER identifiant de la ligne
        if (std::regex_search(texte, match, motifRegex)) {
            
            std::string idTrouve = match.str();

            // On vérifie si on a DÉJÀ créé une exigence pour cet ID
            bool dejaVu = false;
            for (size_t j = 0; j < dejaTrouves.size(); j++) {
                if (dejaTrouves[j] == idTrouve) {
                    dejaVu = true;
                    break;
                }
            }

            // Si c'est la toute première fois qu'on voit cet ID (peu importe sa position)
            if (dejaVu == false) {
                
                ExigenceExtraite ex;
                ex.identifiant   = idTrouve;       
                ex.contenuBrut   = texte;          
                ex.fichierSource = nomFichier;     
                ex.formatSource  = format;         
                ex.numeroLigne   = numLigne;       

                resultats.push_back(ex);
                dejaTrouves.push_back(idTrouve); // On l'ajoute à la liste pour bloquer les futurs doublons
            }
        }
    }

    return resultats;
}
      

