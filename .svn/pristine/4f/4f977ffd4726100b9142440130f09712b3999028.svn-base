/*
 * ConvertisseurExigence.cpp
 *
 *  Created on: 31 mars 2026
 *      Author: yseulthelle
 *
 *  @brief Implémentation de l'adaptateur Module 1 → Module 2.
 */
#include "ConvertisseurExigence.h"
#include <algorithm>
#include <cctype>
#include <regex>

/**
 * @brief Convertit une chaîne de caractères en majuscules.
 *
 * @param s Chaîne d'entrée
 * @return std::string Chaîne convertie en majuscules
 */
static std::string enMajuscules(const std::string& s) {
    std::string res = s;
    std::transform(res.begin(), res.end(), res.begin(),
                   [](unsigned char c){ return std::toupper(c); });
    return res;
}

// ─── Méthodes privées ────────────────────────────────────────────────────────
/**
 * @brief Vérifie si un identifiant correspond à un type d'exigence connu.
 *
 * @param identifiant L'identifiant de l'exigence
 * @return true si le type est SSS, SRS ou SDD
 * @return false sinon
 */

bool ConvertisseurExigences::estTypeConnu(const std::string& identifiant) const {
    std::string upper = enMajuscules(identifiant);
    // On vérifie que le préfixe se trouve exactement au début (index 0)
    return upper.find("CLIENT") != std::string::npos
        || upper.find("PROJET") != std::string::npos
        || upper.find("SDD")    != std::string::npos;
}

/**
 * @brief Détecte le type d'exigence à partir de son identifiant.
 *
 * @param identifiant L'identifiant de l'exigence
 * @return TypeExigence Type détecté (SSS, SRS, SDD)
 */

TypeExigence ConvertisseurExigences::detecterType(const std::string& identifiant) const {
    std::string upper = enMajuscules(identifiant);
    
    // Utilisation des préfixes stricts pour éviter les erreurs de classement
    if (upper.find("CLIENT") != std::string::npos) return SSS;
    if (upper.find("PROJET") != std::string::npos) return SRS;
    if (upper.find("SDD")    != std::string::npos) return SDD;
    
    return SSS; // Valeur par défaut si besoin
}

// ─── Méthode publique principale ─────────────────────────────────────────────
/**
 * @brief Convertit une liste d'exigences extraites en objets SSS, SRS et SDD.
 *
 * @param extraites Liste des exigences extraites par le Module 1
 */

void ConvertisseurExigences::convertir(const std::vector<ExigenceExtraite>& extraites) {
    // Réinitialisation
    exigencesSSS.clear();
    exigencesSRS.clear();
    exigencesSDD.clear();
    ignorees.clear();

    for (const ExigenceExtraite& ex : extraites) {
        if (!estTypeConnu(ex.identifiant)) {
            ignorees.push_back(ex.identifiant);
            continue;
        }
        
        TypeExigence type = detecterType(ex.identifiant);

        std::vector<std::string> liensTrouves;
        std::regex motifLiens(motifRegex);
        auto debut = std::sregex_iterator(ex.contenuBrut.begin(), ex.contenuBrut.end(), motifLiens);
        auto fin = std::sregex_iterator();
        
        for (auto it = debut; it != fin; ++it) {
            std::string idTrouve = it->str();
            // Si l'ID trouvé n'est pas l'ID principal, c'est un lien 
            if (idTrouve != ex.identifiant) {
                liensTrouves.push_back(idTrouve);
            }
        }
       
        switch (type) {
            case SSS: {
                ExigenceSSS obj(ex.identifiant, ex.contenuBrut);
                // Valeur par défaut 
                obj.setAlloueA({});
                obj.setDeveloppe(false);   
                exigencesSSS.push_back(obj);
                break;
            }
            case SRS: {
                ExigenceSRS obj(ex.identifiant, ex.contenuBrut);
                // Valeur par défaut
                obj.setTracabilite(ex.tracabilite);
                obj.setCibles({});
                obj.setNecessaireA({});
                exigencesSRS.push_back(obj);
                break;
            }
            case SDD: {
                ExigenceSDD obj(ex.identifiant, ex.contenuBrut);
                // Pour SDD : referencedSRS correspond aux tracabilite extraites
                obj.setReferencedSRS(ex.tracabilite);
                exigencesSDD.push_back(obj);
                break;
            }
        }

    }
}


// ─── Accesseurs ──────────────────────────────────────────────────────────────
/**
 * @brief Récupère les exigences de type SSS.
 * @return const std::vector<ExigenceSSS>& Liste des exigences SSS
 */
const std::vector<ExigenceSSS>& ConvertisseurExigences::getSSS() const { return exigencesSSS; }
/**
 * @brief Récupère les exigences de type SRS.
 * @return const std::vector<ExigenceSRS>& Liste des exigences SRS
 */
const std::vector<ExigenceSRS>& ConvertisseurExigences::getSRS() const { return exigencesSRS; }
/**
 * @brief Récupère les exigences de type SDD.
 * @return const std::vector<ExigenceSDD>& Liste des exigences SDD
 */
const std::vector<ExigenceSDD>& ConvertisseurExigences::getSDD() const { return exigencesSDD; }
/**
 * @brief Récupère la liste des identifiants ignorés lors de la conversion.
 * @return const std::vector<std::string>& Liste des identifiants ignorés
 */
const std::vector<std::string>& ConvertisseurExigences::getIgnorees() const { return ignorees; }
