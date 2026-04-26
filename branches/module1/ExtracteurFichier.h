/**@file ExtracteurFichier.h
 * @brief Module 1 : Import & Parsing
 * Convertit un fichier physique (Word/CSV/XLS) en liste d'exigences exploitables par le Moteur de Traçabilité (Module 2).
 * Entrées  : chemin du fichier + motif regex
 * Sorties  : liste d'objets Exigence structurés
 */

#ifndef EXTRACTEURFICHIER_H
#define EXTRACTEURFICHIER_H
#include <string>
#include <vector>
#include <regex>
#include <stdexcept> 

/**Structure représentant une exigence extraite d'un fichier source*/
struct ExigenceExtraite {
    std::string identifiant;   
    std::string contenuBrut;  
    std::string fichierSource; 
    std::string formatSource;  
    int         numeroLigne;   
   
    std::string detecterExtension() const;
};

/** Classe principale : ExtracteurFichier*/
class ExtracteurFichier {
private :
    std::string cheminSource;
    std::regex motifRegex;
    std::vector<std::string> listeIdentifiants;
    int nbDetectees; 

    /** Retourne l'extension du fichier en minuscules. */
    std::string detecterExtension() const;

    /** Vérifie que le fichier existe et est lisible. */
    void verifierExistence() const;

    /** Extrait les lignes de texte d'un fichier .csv ou .xls */
    std::vector<std::pair<int,std::string>> lireCSV(const std::string& extension) const;

    /** Extrait le texte brut d'un fichier .docx. */
    std::vector<std::pair<int, std::string>> lireDocx() const;

    /** Applique le motif regex sur une liste de lignes texte. */
    std::vector<ExigenceExtraite> filtrerLignes(
        const std::vector<std::pair<int, std::string>>& lignes, const std::string& format) const;

public:
    ExtracteurFichier(const std::string& chemin, const std::string& motif);
    std::vector<ExigenceExtraite> extraire();
    int getNombreExigencesDetectees() const;
    
};

/**Levée quand le fichier est introuvable ou inaccessible*/
class FileNotFoundException : public std::runtime_error{
public:
    explicit FileNotFoundException(const std::string& chemin)
        : std::runtime_error("Fichier introuvable :" + chemin) {}
};

/**Levée quand le format du fichier n'est pas supporté*/
class FileFormatException : public std::runtime_error {
public:
    explicit FileFormatException(const std::string& detail)
        : std::runtime_error("Format non supporté : " + detail) {}
};



#endif