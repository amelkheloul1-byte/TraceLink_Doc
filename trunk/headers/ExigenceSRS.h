/*
 * ExigenceSRS.h
 *
 *  Created on: 9 mars 2026
 *      Author: yseulthelle
 */

#ifndef EXIGENCESRS_H_
#define EXIGENCESRS_H_

#include "Exigence.h"
#include <vector>

/**
 * @class ExigenceSRS
 * @brief Exigence de niveau SRS (Software Requirements Specification).
 *
 * Hérite de @c Exigence. Contient :
 * - @c tracabilite : identifiants des exigences SSS qu'elle couvre,
 * - @c cibles      : composants ou modules ciblés par l'exigence,
 * - @c necessaireA : projets concernés par cette exigence.
 */
class ExigenceSRS : public Exigence {

private:
    std::vector<std::string> tracabilite;  ///< Identifiants des exigences SSS couvertes.
    std::vector<std::string> cibles;       ///< Composants ou modules ciblés.
    std::vector<std::string> necessaireA;  ///< Projets concernés par l'exigence.

public:
    /**
     * @brief Construit une exigence SRS.
     * @param id      Identifiant unique de l'exigence.
     * @param contenu Texte descriptif de l'exigence.
     *
     * Les trois vecteurs sont vides par défaut.
     */
    ExigenceSRS(const std::string& id, const std::string& contenu);

    /// @brief Retourne les identifiants SSS couverts par cette exigence.
    /// @return Référence constante sur le vecteur de traçabilité.
    const std::vector<std::string>& getTracabilite() const;

    /// @brief Retourne les cibles de cette exigence.
    /// @return Référence constante sur le vecteur de cibles.
    const std::vector<std::string>& getCibles() const;

    /// @brief Retourne les projets concernés par cette exigence.
    /// @return Référence constante sur le vecteur de projets.
    const std::vector<std::string>& getNecessaireA() const;

    /// @brief Modifie la liste de traçabilité vers les SSS.
    /// @param tracabilite Vecteur d'identifiants SSS.
    void setTracabilite(const std::vector<std::string>& tracabilite);

    /// @brief Modifie la liste des cibles.
    /// @param cibles Vecteur d'identifiants de cibles.
    void setCibles(const std::vector<std::string>& cibles);

    /// @brief Modifie la liste des projets concernés.
    /// @param necessaireA Vecteur d'identifiants de projets.
    void setNecessaireA(const std::vector<std::string>& necessaireA);
};

#endif /* EXIGENCESRS_H_ */



