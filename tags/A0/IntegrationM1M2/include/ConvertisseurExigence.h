/*
 * ConvertisseurExigence.h
 *
 *  Created on: 31 mars 2026
 *      Author: yseulthelle
 */
/*
 * ConvertisseurExigences.h
 *
 * @brief Adaptateur entre le Module 1 (ExtracteurFichier) et le Module 2 (MoteurTracabilite).
 *
 * Convertit les ExigenceExtraite (structs brutes issues du parsing)
 * en objets typés ExigenceSSS / ExigenceSRS / ExigenceSDD.
 *
 * Détection du type depuis l'identifiant :
 *   - contient "CLIENT" → SSS  (ex: EXIGENCE_CLIENT_A_F01)
 *   - contient "PROJET" → SRS  (ex: EXIGENCE_PROJET_A_02)
 *   - contient "SDD"    → SDD  (ex: SDD_FUNC_01)
 *   - sinon             → ignorée
 *
 * Les métadonnées extraites par le Module 1 (tracabilite, cibles,
 * necessaireA, alloueA, developpe) sont automatiquement transférées
 * dans les objets typés correspondants.
 */
#ifndef CONVERTISSEUREXIGENCE_H_
#define CONVERTISSEUREXIGENCE_H_

#include <vector>
#include <string>

// Inclusions du Module extraction (Données brutes)
#include "ExtracteurFichier.h" 

// Inclusions du Module traçabilité (Objets typés)
#include "ExigenceSSS.h"
#include "ExigenceSRS.h"
#include "ExigenceSDD.h"

/**
 * @class ConvertisseurExigences
 * @brief Adaptateur qui transforme les structures brutes (Module 1) 
 * en objets C++ typés (Module 2) prêts pour l'analyse de traçabilité.
 */
class ConvertisseurExigences {
private:
    std::vector<ExigenceSSS> exigencesSSS;  ///< Exigences de niveau SSS converties.
    std::vector<ExigenceSRS> exigencesSRS;  ///< Exigences de niveau SRS converties.
    std::vector<ExigenceSDD> exigencesSDD;  ///< Exigences de niveau SDD converties.
    std::vector<std::string> ignorees;      ///< Identifiants dont le type n'a pas pu être déterminé.
    std::string motifRegex;
    /**
     * @brief Détermine le type d'une exigence depuis son identifiant.
     * @param identifiant Identifiant extrait par le module 1.
     * @return SSS, SRS ou SDD.
     * @note Appeler estTypeConnu() avant pour éviter un résultat invalide.
     */
    TypeExigence detecterType(const std::string& identifiant) const;
    /**
     * @brief Indique si l'identifiant contient un mot-clé de type reconnu.
     * @param identifiant L'identifiant à tester.
     * @return true si "CLIENT", "PROJET" ou "SDD" est présent (insensible à la casse).
     */
    bool estTypeConnu(const std::string& identifiant) const;

public:
    /**
     * @brief Convertit un vecteur de ExigenceExtraite en objets typés.
     * @param extraites Liste issue de ExtracteurFichier::extraire().
     *
     * Réinitialise les résultats précédents à chaque appel.
     * Transfère automatiquement les métadonnées (tracabilite, cibles,
     * necessaireA, alloueA, developpe) dans les objets créés.
     */
    ConvertisseurExigences(const std::string& motif = "EXIGENCE_[A-Z0-9_]+") 
        : motifRegex(motif) {}
    void convertir(const std::vector<ExigenceExtraite>& extraites);
    /// @brief Retourne les exigences SSS converties.
    const std::vector<ExigenceSSS>& getSSS() const;
    /// @brief Retourne les exigences SRS converties.
    const std::vector<ExigenceSRS>& getSRS() const;
    /// @brief Retourne les exigences SDD converties.
    const std::vector<ExigenceSDD>& getSDD() const;
    /// @brief Retourne les identifiants ignorés (type non reconnu).
    const std::vector<std::string>& getIgnorees() const;
};
#endif /* CONVERTISSEUREXIGENCES_H_ */