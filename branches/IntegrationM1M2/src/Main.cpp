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
/** * @brief Initialisation de l'extracteur.
 * Utilisation d'un chemin RELATIF "test2.csv" pour garantir la portabilité 
 * entre Windows et Mac sans modifier le code.
 * Le motif Regex identifie les IDs commençant par "EXIGENCE_".
 */
ExtracteurFichier extracteur("test2.csv", "EXIGENCE_[A-Z0-9_]+");
// Extraction effective des lignes du fichier dans un vecteur temporaire
std::vector<ExigenceExtraite> listeBrute = extracteur.extraire();
std::cout << "Extraction terminee. " << listeBrute.size() << " elements trouves." << std::endl;

// 2. LE PONT : CONVERSION 
/**
 * @brief Le convertisseur transforme les structures de texte brut en objets 
 * métiers structurés (SSS, SRS, SDD) exploitables par le moteur.
 */
ConvertisseurExigences convertisseur;
convertisseur.convertir(listeBrute);

// Affichage du bilan de la conversion pour le debug
std::cout << "- SSS generees : " << convertisseur.getSSS().size() << std::endl;
std::cout << "- SRS generees : " << convertisseur.getSRS().size() << std::endl;
std::cout << "- SDD generees : " << convertisseur.getSDD().size() << std::endl;


// 3. MOTEUR DE TRAÇABILITÉ (L'analyse)

MoteurTracabilite moteur;
// Injection des listes d'exigences converties dans le moteur d'analyse
moteur.setDonnees(
convertisseur.getSSS(),
convertisseur.getSRS(),
convertisseur.getSDD()
);

/**
 * @brief Gestion des Threads.
 * On utilise le ThreadManager pour exécuter les calculs lourds en arrière-plan.
 *
 */
ThreadManager tm(moteur);
tm.lancerAnalyse();
/**
 * @attention Cette ligne bloque le programme jusqu'à la fin du calcul.
 * Sans cette attente, le rapport afficherait 0% car les résultats ne seraient 
 * pas encore prêts.
 */
tm.attendreFinAnalyse();


// 4. AFFICHAGE DES RÉSULTATS

// Récupération des statistiques finales de l'analyse
RapportTracabilite rapport = moteur.getRapport();
std::cout << "\n=== RAPPORT FINAL ===" << std::endl;
/**
 * Les taux de couverture sont stockés sous forme de décimales (ex: 0.33).
 * On les multiplie par 100 pour un affichage lisible en pourcentage.
 */
std::cout << "Taux de couverture SSS -> SRS : " << rapport.tauxSSS_SRS * 100 << " %" << std::endl;
std::cout << "Taux de couverture SRS -> SDD : " << rapport.tauxSRS_SDD * 100 << " %" << std::endl;
std::cout << "Taux global : " << rapport.tauxGlobal * 100 << " %" << std::endl;

} catch (const std::exception& e) {
    /**
     * Gestion globale des erreurs (Fichier manquant, format corrompu, etc.).
     *
     */
std::cerr << "Erreur fatale : " << e.what() << std::endl;
}

return 0;
}