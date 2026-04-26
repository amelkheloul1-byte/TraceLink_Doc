/*
 * Main.cpp
 *
 *  Created on: 23 mars 2026
 *      Author: yseulthelle
 */

/**
 * @file Main.cpp
 * @brief Point d'entrée du programme — test de l'analyse de traçabilité sur données fictives.
 *
 * Crée des exigences SSS, SRS et SDD, les injecte dans @c MoteurTracabilite,
 * lance l'analyse via @c ThreadManager, puis affiche les taux de couverture
 * et les liens brisés issus du @c RapportTracabilite.
 */

#include "MoteurTracabilite.h"
#include "ThreadManager.h"
#include <iostream>

int main() {

    // Création des exigences fictives
    ExigenceSSS sss1("SSS-1", "Le système doit authentifier l'utilisateur");
    ExigenceSSS sss2("SSS-2", "Le système doit afficher un tableau de bord");

    ExigenceSRS srs1("SRS-1", "Le logiciel doit afficher un formulaire de login");
    srs1.setTracabilite({"SSS-1"});

    ExigenceSRS srs2("SRS-2", "Le logiciel doit afficher les statistiques");
    srs2.setTracabilite({"SSS-2"});

    ExigenceSDD sdd1("SDD-1", "Conception du formulaire de login");
    sdd1.setReferencedSRS({"SRS-1"});

    // Remplissage des vecteurs
    std::vector<ExigenceSSS> vSSS = {sss1, sss2};
    std::vector<ExigenceSRS> vSRS = {srs1, srs2};
    std::vector<ExigenceSDD> vSDD = {sdd1};

    // Injection dans le moteur
    MoteurTracabilite moteur;
    moteur.setDonnees(vSSS, vSRS, vSDD);

    // Lancement de l'analyse dans un thread d'arrière-plan
    ThreadManager tm(moteur);
    tm.lancerAnalyse();
    tm.attendreFinAnalyse();

    // Affichage du rapport
    RapportTracabilite rapport = moteur.getRapport();
    std::cout << "Taux SSS->SRS : " << rapport.tauxSSS_SRS * 100 << "%" << std::endl;
    std::cout << "Taux SRS->SDD : " << rapport.tauxSRS_SDD * 100 << "%" << std::endl;
    std::cout << "Taux global   : " << rapport.tauxGlobal * 100 << "%" << std::endl;

    std::cout << "\nLiens brises :" << std::endl;
    for (const auto& lien : rapport.liensBrises) {
        std::cout << "  - " << lien << std::endl;
    }

    return 0;
}

