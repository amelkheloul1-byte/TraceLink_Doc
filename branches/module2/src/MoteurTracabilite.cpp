/*
 * MoteurTracabilite.cpp
 *
 *  Created on: 23 mars 2026
 *      Author: yseulthelle
 */

#include "MoteurTracabilite.h"
#include <iostream>

/// @brief Initialise les trois taux de couverture à 0.0.
MoteurTracabilite::MoteurTracabilite()
    : tauxSSS_SRS(0.0), tauxSRS_SDD(0.0), tauxGlobal(0.0) {}

/// @brief Stocke les trois vecteurs d'exigences transmis par le module 1.
void MoteurTracabilite::setDonnees(const std::vector<ExigenceSSS>& sss,
                                    const std::vector<ExigenceSRS>& srs,
                                    const std::vector<ExigenceSDD>& sdd) {
    this->exigencesSSS = sss;
    this->exigencesSRS = srs;
    this->exigencesSDD = sdd;
}

/// @brief Pour chaque SRS, marque les SSS référencées comme @c Couverte.
/// Les références introuvables alimentent @c liensBrises.
/// Les SSS encore à @c NonAnalyse à la fin sont marquées @c NonCouverte.
void MoteurTracabilite::analyserSSS_SRS() {

    for (const auto& srs : exigencesSRS) {
        for (const auto& ref : srs.getTracabilite()) {
            bool trouve = false;
            for (auto& sss : exigencesSSS) {
                if (sss.getId() == ref) {
                    sss.setStatut(Couverte);
                    trouve = true;
                    break;
                }
            }
            if (!trouve) {
                liensBrises.push_back(
                    std::string("Lien brise : ") + srs.getId() + " reference " + ref + " qui n'existe pas dans SSS"
                );
            }
        }
    }

    for (auto& sss : exigencesSSS) {
        if (sss.getStatut() == NonAnalyse) {
            sss.setStatut(NonCouverte);
        }
    }
}

/// @brief Pour chaque SDD, marque les SRS référencées comme @c Couverte.
/// Même logique que @c analyserSSS_SRS() appliquée aux paires SDD/SRS.
void MoteurTracabilite::analyserSRS_SDD() {

    for (const auto& sdd : exigencesSDD) {
        for (const auto& ref : sdd.getReferencedSRS()) {
            bool trouve = false;
            for (auto& srs : exigencesSRS) {
                if (srs.getId() == ref) {
                    srs.setStatut(Couverte);
                    trouve = true;
                    break;
                }
            }
            if (!trouve) {
                liensBrises.push_back(
                    std::string("Lien brise : ") + sdd.getId() + " reference " + ref + " qui n'existe pas dans SRS"
                );
            }
        }
    }

    for (auto& srs : exigencesSRS) {
        if (srs.getStatut() == NonAnalyse) {
            srs.setStatut(NonCouverte);
        }
    }
}

/// @brief Réinitialise les statuts, vide les liens brisés,
/// puis appelle @c analyserSSS_SRS() et @c analyserSRS_SDD().
void MoteurTracabilite::analyserTracabilite() {

    for (auto& sss : exigencesSSS) {
        sss.setStatut(NonAnalyse);
    }
    for (auto& srs : exigencesSRS) {
        srs.setStatut(NonAnalyse);
    }

    liensBrises.clear();

    analyserSSS_SRS();
    analyserSRS_SDD();
}

/// @brief Calcule tauxSSS_SRS, tauxSRS_SDD et tauxGlobal.
/// Ajoute un avertissement dans @c liensBrises si un vecteur est vide.
void MoteurTracabilite::calculerTaux() {

    if (exigencesSSS.empty()) {
        tauxSSS_SRS = 0.0;
        liensBrises.push_back(std::string("Avertissement : aucune exigence SSS trouvée"));
    } else {
        int nbCouvertes = 0;
        for (const auto& sss : exigencesSSS) {
            if (sss.getStatut() == Couverte) nbCouvertes++;
        }
        tauxSSS_SRS = static_cast<double>(nbCouvertes) / static_cast<double>(exigencesSSS.size());
    }

    if (exigencesSRS.empty()) {
        tauxSRS_SDD = 0.0;
        liensBrises.push_back(std::string("Avertissement : aucune exigence SRS trouvée"));
    } else {
        int nbCouvertes = 0;
        for (const auto& srs : exigencesSRS) {
            if (srs.getStatut() == Couverte) nbCouvertes++;
        }
        tauxSRS_SDD = static_cast<double>(nbCouvertes) / static_cast<double>(exigencesSRS.size());
    }

    tauxGlobal = tauxSSS_SRS * tauxSRS_SDD;
}

/// @brief Retourne le taux de couverture SSS→SRS.
double MoteurTracabilite::getTauxSSS_SRS() const { return tauxSSS_SRS; }

/// @brief Retourne le taux de couverture SRS→SDD.
double MoteurTracabilite::getTauxSRS_SDD() const { return tauxSRS_SDD; }

/// @brief Retourne le taux de couverture global.
double MoteurTracabilite::getTauxGlobal()  const { return tauxGlobal;  }

/// @brief Retourne la liste des liens brisés détectés.
const std::vector<std::string>& MoteurTracabilite::getLiensBrises() const {
    return liensBrises;
}

/// @brief Remplit et retourne la structure @c RapportTracabilite pour le module 3.
RapportTracabilite MoteurTracabilite::getRapport() const {
    RapportTracabilite rapport;
    rapport.exigencesSSS = exigencesSSS;
    rapport.liensBrises  = liensBrises;
    rapport.tauxSSS_SRS  = tauxSSS_SRS;
    rapport.tauxSRS_SDD  = tauxSRS_SDD;
    rapport.tauxGlobal   = tauxGlobal;
    return rapport;
}
