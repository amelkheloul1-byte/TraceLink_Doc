/*
 * MoteurTracabilite.h
 *
 *  Created on: 10 mars 2026
 *      Author: yseulthelle
 */

#ifndef MOTEURTRACABILITE_H_
#define MOTEURTRACABILITE_H_

#include "ExigenceSSS.h"
#include "ExigenceSRS.h"
#include "ExigenceSDD.h"
#include "RapportTracabilite.h"
#include <vector>
#include <string>

/**
 * @class MoteurTracabilite
 * @brief Orchestre l'analyse de traçabilité sur les trois niveaux d'exigences.
 *
 * Reçoit les vecteurs d'exigences du module 1 via @c setDonnees(),
 * exécute l'analyse en deux passes (SSS→SRS puis SRS→SDD),
 * calcule les taux de couverture et expose les résultats sous forme
 * d'un @c RapportTracabilite destiné au module 3.
 *
 * Peut être piloté de manière asynchrone via @c ThreadManager.
 */
class MoteurTracabilite {

private:
    std::vector<ExigenceSSS> exigencesSSS;  ///< Exigences de niveau SSS reçues du module 1.
    std::vector<ExigenceSRS> exigencesSRS;  ///< Exigences de niveau SRS reçues du module 1.
    std::vector<ExigenceSDD> exigencesSDD;  ///< Exigences de niveau SDD reçues du module 1.

    std::vector<std::string> liensBrises;  ///< Références invalides détectées pendant l'analyse.

    double tauxSSS_SRS;  ///< Taux de couverture SSS→SRS (0.0 à 1.0).
    double tauxSRS_SDD;  ///< Taux de couverture SRS→SDD (0.0 à 1.0).
    double tauxGlobal;   ///< Taux de couverture global (0.0 à 1.0).

    /**
     * @brief Analyse la traçabilité SRS → SSS.
     *
     * Pour chaque SRS, parcourt ses identifiants de traçabilité et
     * marque les SSS correspondantes comme @c Couverte. Les références
     * introuvables sont enregistrées dans @c liensBrises. Les SSS
     * encore à @c NonAnalyse à la fin sont marquées @c NonCouverte.
     */
    void analyserSSS_SRS();

    /**
     * @brief Analyse la traçabilité SDD → SRS.
     *
     * Même logique que @c analyserSSS_SRS() appliquée aux paires SDD/SRS.
     */
    void analyserSRS_SDD();

public:
    /**
     * @brief Initialise le moteur avec tous les taux à 0.0.
     */
    MoteurTracabilite();

    /**
     * @brief Injecte les trois vecteurs d'exigences issus du module 1.
     * @param sss Vecteur d'exigences de niveau SSS.
     * @param srs Vecteur d'exigences de niveau SRS.
     * @param sdd Vecteur d'exigences de niveau SDD.
     */
    void setDonnees(const std::vector<ExigenceSSS>& sss,
                    const std::vector<ExigenceSRS>& srs,
                    const std::vector<ExigenceSDD>& sdd);

    /**
     * @brief Lance l'analyse de traçabilité complète (SSS→SRS puis SRS→SDD).
     *
     * Réinitialise les statuts de toutes les exigences à @c NonAnalyse,
     * vide les liens brisés, puis appelle @c analyserSSS_SRS() et @c analyserSRS_SDD().
     */
    void analyserTracabilite();

    /**
     * @brief Calcule les taux de couverture après l'analyse.
     *
     * - @c tauxSSS_SRS = nb SSS couvertes / nb SSS total
     * - @c tauxSRS_SDD = nb SRS couvertes / nb SRS total
     * - @c tauxGlobal  = @c tauxSSS_SRS × @c tauxSRS_SDD
     *
     * Des avertissements sont ajoutés aux liens brisés si un vecteur est vide.
     */
    void calculerTaux();

    /// @brief Retourne le taux de couverture SSS→SRS.
    /// @return Valeur entre 0.0 et 1.0.
    double getTauxSSS_SRS() const;

    /// @brief Retourne le taux de couverture SRS→SDD.
    /// @return Valeur entre 0.0 et 1.0.
    double getTauxSRS_SDD() const;

    /// @brief Retourne le taux de couverture global.
    /// @return Valeur entre 0.0 et 1.0.
    double getTauxGlobal() const;

    /// @brief Retourne la liste des liens brisés détectés.
    /// @return Référence constante sur le vecteur de messages d'erreur.
    const std::vector<std::string>& getLiensBrises() const;

    /**
     * @brief Construit et retourne le rapport complet à destination du module 3.
     * @return Structure @c RapportTracabilite remplie avec les exigences SSS,
     *         les liens brisés et les trois taux de couverture.
     */
    RapportTracabilite getRapport() const;
};

#endif /* MOTEURTRACABILITE_H_ */


