/*
 * ThreadManager.cpp
 *
 *  Created on: 23 mars 2026
 *      Author: yseulthelle
 */

#include "ThreadManager.h"
#include <iostream>

/// @brief Initialise la référence au moteur et positionne @c analyseTerminee à @c false.
ThreadManager::ThreadManager(MoteurTracabilite& moteur)
    : moteur(moteur), analyseTerminee(false) {}

/// @brief Joint le thread s'il est encore actif, empêchant l'appel à @c std::terminate().
ThreadManager::~ThreadManager() {
    if (threadAnalyse.joinable()) {
        threadAnalyse.join();
    }
}

/// @brief Crée un thread d'arrière-plan qui exécute @c analyserTracabilite()
/// puis @c calculerTaux(), et positionne @c analyseTerminee à @c true.
/// Sans effet si un thread est déjà actif.
void ThreadManager::lancerAnalyse() {

    if (threadAnalyse.joinable()) {
        return;
    }

    analyseTerminee = false;

    threadAnalyse = std::thread([this]() {
        std::cout << "Analyse en cours..." << std::endl;

        moteur.analyserTracabilite();
        moteur.calculerTaux();

        analyseTerminee = true;

        std::cout << "Analyse terminée" << std::endl;
    });
}

/// @brief Appelle @c std::thread::join() pour bloquer jusqu'à la fin du thread.
/// Sans effet si aucun thread n'est actif.
void ThreadManager::attendreFinAnalyse() {
    if (threadAnalyse.joinable()) {
        threadAnalyse.join();
    }
}

/// @brief Retourne l'état de @c analyseTerminee sans bloquer le thread appelant.
bool ThreadManager::estTerminee() const {
    return analyseTerminee;
}
