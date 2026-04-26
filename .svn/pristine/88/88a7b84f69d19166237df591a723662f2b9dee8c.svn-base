/*
 * RapportTracabilite.h
 *
 *  Created on: 10 mars 2026
 *      Author: yseulthelle
 */

//module qui va permettre de transférer la structure de données conetnant les infos sur la traçabilité au module 3

#ifndef RAPPORTTRACABILITE_H_
#define RAPPORTTRACABILITE_H_

#include "ExigenceSSS.h"
#include <vector>
#include <string>

struct RapportTracabilite {
    std::vector<ExigenceSSS> exigencesSSS;
    std::vector<std::string> liensBrises;

    double tauxSSS_SRS; //taux de SSS par rapport aux SRS qui les couvrent
    double tauxSRS_SDD; //taux de SRS par rapport aux SDD qui les couvrent
    double tauxGlobal; //tau de couverture global des SSS
};

#endif /* RAPPORTTRACABILITE_H_ */
