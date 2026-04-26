/*
 * Main.cpp
 *
 * Created on: 31 mars 2026
 * Author: yseulthelle
 * @brief Point d'entrée — pipeline complet Module 1 → Module 2.
 */

#include <iostream>
#include <vector>
#include "ExtracteurFichier.h"
#include "ConvertisseurExigence.h"
#include "MoteurTracabilite.h"
#include "ThreadManager.h"

int main() {
std::cout << "=== Lancement du Programme ===" << std::endl;

try {
// 1. EXTRACTION (Les données brutes)
// On extrait toutes les exigences de ton fichier test
ExtracteurFichier extracteur("test3.docx", "EXIGENCE_[A-Z0-9_]+");
std::vector<ExigenceExtraite> listeBrute = extracteur.extraire();
std::cout << "Extraction terminee. " << listeBrute.size() << " elements trouves." << std::endl;

// 2. LE PONT : CONVERSION 
ConvertisseurExigences convertisseur;
convertisseur.convertir(listeBrute);

std::cout << "- SSS generees : " << convertisseur.getSSS().size() << std::endl;
std::cout << "- SRS generees : " << convertisseur.getSRS().size() << std::endl;
std::cout << "- SDD generees : " << convertisseur.getSDD().size() << std::endl;


// 3. MOTEUR DE TRAÇABILITÉ (L'analyse)

MoteurTracabilite moteur;
moteur.setDonnees(
convertisseur.getSSS(),
convertisseur.getSRS(),
convertisseur.getSDD()
);

// On lance l'analyse avec les Threads
ThreadManager tm(moteur);
tm.lancerAnalyse();
tm.attendreFinAnalyse();


// 4. AFFICHAGE DES RÉSULTATS
RapportTracabilite rapport = moteur.getRapport();
std::cout << "\n=== RAPPORT FINAL ===" << std::endl;
std::cout << "Taux de couverture SSS -> SRS : " << rapport.tauxSSS_SRS * 100 << " %" << std::endl;
std::cout << "Taux de couverture SRS -> SDD : " << rapport.tauxSRS_SDD * 100 << " %" << std::endl;
std::cout << "Taux global : " << rapport.tauxGlobal * 100 << " %" << std::endl;

} catch (const std::exception& e) {
std::cerr << "Erreur fatale : " << e.what() << std::endl;
}

return 0;
}