/*
 * RapportTracabilite.h
 *
 *  Created on: 10 mars 2026
 *      Author: yseulthelle
 */

#ifndef RAPPORTTRACABILITE_H_
#define RAPPORTTRACABILITE_H_

#include "ExigenceSSS.h"
#include <vector>
#include <string>

/**
 * @struct RapportTracabilite
 * @brief Structure de transfert des résultats de l'analyse vers le module 3.
 *
 * Construite par @c MoteurTracabilite::getRapport() et consommée par
 * le module 3 pour la génération du rapport final.
 */
struct RapportTracabilite {
    std::vector<ExigenceSSS> exigencesSSS;   ///< Exigences SSS avec leurs statuts mis à jour.
    std::vector<std::string> liensBrises;    ///< Messages décrivant les références invalides détectées.

    double tauxSSS_SRS; ///< Proportion de SSS couvertes par au moins une SRS (0.0 à 1.0).
    double tauxSRS_SDD; ///< Proportion de SRS couvertes par au moins une SDD (0.0 à 1.0).
    double tauxGlobal;  ///< Taux global = @c tauxSSS_SRS × @c tauxSRS_SDD.
};

#endif /* RAPPORTTRACABILITE_H_ */
