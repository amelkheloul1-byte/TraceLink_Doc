/*
 * MoteurTracabilite.cpp
 *
 *  Created on: 23 mars 2026
 *      Author: yseulthelle
 */


#include "MoteurTracabilite.h"
#include <iostream> //permettera d'afficher les messages d'erreur

//Constructeur : on initialise les taux à 0.0
MoteurTracabilite::MoteurTracabilite()
    : tauxSSS_SRS(0.0), tauxSRS_SDD(0.0), tauxGlobal(0.0) {}//on initialise les taux à 0.0

//On reçoit les 3 vecteurs du module 1 et on les stock
void MoteurTracabilite::setDonnees(const std::vector<ExigenceSSS>& sss, //le & permet de lire le vecteur sans le modifier, mais avant tout sans en utiliser une copie
                                    const std::vector<ExigenceSRS>& srs,
                                    const std::vector<ExigenceSDD>& sdd) {
    this->exigencesSSS = sss;
    this->exigencesSRS = srs;
    this->exigencesSDD = sdd;
}


//Méthode privée : analyse SSS -> SRS
void MoteurTracabilite::analyserSSS_SRS() {

    for (const auto& srs : exigencesSRS) { //Pour chaque exigence SRS
        for (const auto& ref : srs.getTracabilite()) {
            bool trouve = false;
            for (auto& sss : exigencesSSS) {
                if (sss.getId() == ref) { //on regarde pour chaque exigence SSS si elle correspond à l'exigence que la SRS couvre
                    sss.setStatut(Couverte); //si oui on change son statut et on peut sortir du if
                    trouve = true;
                    break;
                }
            }

            if (!trouve) { //si trouve est faux : alors l'exiegnce SSS n'est couverte par aucune SRS, donc on l'ajoute aux liens brisés
                liensBrises.push_back(
                    std::string("Lien brise : ") + srs.getId() + " reference " + ref + " qui n'existe pas dans SSS"
                );
            }
        }
    }

    //pour les exigences qui n'ont pas été couvertes
    for (auto& sss : exigencesSSS) {
        if (sss.getStatut() == NonAnalyse) {
            sss.setStatut(NonCouverte); //une fois que on a réalisé la traçabilité sur toutes les exigences, on change le statut de celles qui ne sont pas couvertes (seules les couvertes ont été changées
        }
    }
}

//Méthode privée : analyse SRS -> SDD : exactement la même chose pour que l'analyse des SSS -> SRS
void MoteurTracabilite::analyserSRS_SDD() {

	//Exigences couvertes
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

    //Exigences non couvertes
    for (auto& srs : exigencesSRS) {
        if (srs.getStatut() == NonAnalyse) {
            srs.setStatut(NonCouverte);
        }
    }
}


void MoteurTracabilite::analyserTracabilite() { //permet d'appeler à la fois les deux ùéthodes d'analyse de traçabilité : plus facile pour la maniopulation des fichiers

	//Réinitialiser les statuts des exigences
    for (auto& sss : exigencesSSS) {
        sss.setStatut(NonAnalyse);
    }
    for (auto& srs : exigencesSRS) {
        srs.setStatut(NonAnalyse);
    }

    //Vider les liens brisés précédents
    liensBrises.clear();

	analyserSSS_SRS();
    analyserSRS_SDD();
}


void MoteurTracabilite::calculerTaux() {

    if (exigencesSSS.empty()) {
        tauxSSS_SRS = 0.0;
        liensBrises.push_back(std::string("Avertissement : aucune exigence SSS trouvée"));
    } else {
        int nbCouvertes = 0;
        for (const auto& sss : exigencesSSS) {
            if (sss.getStatut() == Couverte){
            	nbCouvertes++;
            }
        }
        tauxSSS_SRS = static_cast<double>(nbCouvertes) / static_cast<double>(exigencesSSS.size());
    }

    if (exigencesSRS.empty()) {
        tauxSRS_SDD = 0.0;
        liensBrises.push_back(std::string("Avertissement : aucune exigence SRS trouvée"));
    } else {
        int nbCouvertes = 0;
        for (const auto& srs : exigencesSRS) {
            if (srs.getStatut() == Couverte){
            	nbCouvertes++;
            }
        }
        tauxSRS_SDD = static_cast<double>(nbCouvertes) / static_cast<double>(exigencesSRS.size());
    }

    tauxGlobal = tauxSSS_SRS * tauxSRS_SDD;
}

//Méthodes de retour
double MoteurTracabilite::getTauxSSS_SRS() const { return tauxSSS_SRS; }
double MoteurTracabilite::getTauxSRS_SDD() const { return tauxSRS_SDD; }
double MoteurTracabilite::getTauxGlobal()  const { return tauxGlobal;  }

const std::vector<std::string>& MoteurTracabilite::getLiensBrises() const {
    return liensBrises;
}

const std::vector<ExigenceSRS>& MoteurTracabilite::getExigencesSRS() const { return exigencesSRS; }
const std::vector<ExigenceSDD>& MoteurTracabilite::getExigencesSDD() const { return exigencesSDD; }

//Retour du rapport pour le module 3
RapportTracabilite MoteurTracabilite::getRapport() const {
    RapportTracabilite rapport;
    rapport.exigencesSSS = exigencesSSS;
    rapport.liensBrises  = liensBrises;
    rapport.tauxSSS_SRS  = tauxSSS_SRS;
    rapport.tauxSRS_SDD  = tauxSRS_SDD;
    rapport.tauxGlobal   = tauxGlobal;
    return rapport;
}
