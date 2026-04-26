/*
 * ExigenceSSS.h
 *
 *  Created on: 9 mars 2026
 *      Author: yseulthelle
 */

#ifndef EXIGENCESSS_H_
#define EXIGENCESSS_H_

#include "Exigence.h"
#include <vector>

/**
 * @class ExigenceSSS
 * @brief Exigence de niveau SSS (System/Subsystem Specification).
 *
 * Hérite de @c Exigence. Porte deux attributs spécifiques :
 * - un indicateur de développement (@c developpe),
 * - la liste des projets auxquels elle est allouée (@c alloueA).
 */
class ExigenceSSS : public Exigence {

private:
    bool developpe;                    ///< Indique si l'exigence a été développée.
    std::vector<std::string> alloueA;  ///< Liste des identifiants de projets auxquels elle est allouée.

public:
    /**
     * @brief Construit une exigence SSS.
     * @param id      Identifiant unique de l'exigence.
     * @param contenu Texte descriptif de l'exigence.
     *
     * @c developpe est initialisé à @c false par défaut.
     */
    ExigenceSSS(const std::string& id, const std::string& contenu);

    /// @brief Indique si l'exigence a été développée.
    /// @return @c true si développée, @c false sinon.
    bool getDeveloppe() const;

    /// @brief Retourne la liste des projets auxquels cette exigence est allouée.
    /// @return Référence constante sur le vecteur d'identifiants de projets.
    const std::vector<std::string>& getAlloueA() const;

    /// @brief Modifie l'état de développement de l'exigence.
    /// @param developpe Nouvel état (@c true = développée).
    void setDeveloppe(bool developpe);

    /// @brief Modifie la liste des projets auxquels l'exigence est allouée.
    /// @param alloueA Vecteur d'identifiants de projets.
    void setAlloueA(const std::vector<std::string>& alloueA);
};

#endif /* EXIGENCESSS_H_ */


