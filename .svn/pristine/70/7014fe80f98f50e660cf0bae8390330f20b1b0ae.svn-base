/*
 * ThreadManager.h
 *
 *  Created on: 23 mars 2026
 *      Author: yseulthelle
 */

#ifndef THREADMANAGER_H_
#define THREADMANAGER_H_

#include "MoteurTracabilite.h"
#include <thread>
#include <functional>
#include <atomic>

/**
 * @class ThreadManager
 * @brief Gère l'exécution asynchrone de l'analyse de traçabilité.
 *
 * Encapsule un @c std::thread qui exécute séquentiellement
 * @c MoteurTracabilite::analyserTracabilite() puis
 * @c MoteurTracabilite::calculerTaux(). Un @c std::atomic<bool>
 * permet de consulter l'état d'avancement sans bloquer le thread principal.
 *
 * Le destructeur garantit que le thread est joint avant la destruction
 * de l'objet, évitant ainsi l'appel à @c std::terminate().
 */
class ThreadManager {

private:
    MoteurTracabilite& moteur;     ///< Référence au moteur de traçabilité (pas de copie).
    std::thread threadAnalyse;     ///< Thread d'arrière-plan exécutant l'analyse.
    std::atomic<bool> analyseTerminee;  ///< Indicateur atomique de fin d'analyse.

public:
    /**
     * @brief Construit le gestionnaire de thread.
     * @param moteur Référence au @c MoteurTracabilite déjà alimenté par @c setDonnees().
     *
     * @c analyseTerminee est initialisé à @c false.
     */
    ThreadManager(MoteurTracabilite& moteur);

    /**
     * @brief Destructeur : joint le thread s'il est encore actif.
     *
     * Empêche l'appel à @c std::terminate() lors de la destruction
     * d'un @c std::thread encore joignable.
     */
    ~ThreadManager();

    /**
     * @brief Lance l'analyse dans un thread séparé.
     *
     * Crée un @c std::thread qui exécute successivement
     * @c MoteurTracabilite::analyserTracabilite() et
     * @c MoteurTracabilite::calculerTaux(), puis positionne
     * @c analyseTerminee à @c true.
     * Sans effet si un thread est déjà en cours d'exécution.
     */
    void lancerAnalyse();

    /**
     * @brief Bloque le thread appelant jusqu'à la fin de l'analyse.
     *
     * Appelle @c std::thread::join() sur le thread d'analyse.
     * Sans effet si aucun thread n'est actif.
     */
    void attendreFinAnalyse();

    /// @brief Indique si l'analyse est terminée, sans bloquer.
    /// @return @c true si @c analyseTerminee vaut @c true, @c false sinon.
    bool estTerminee() const;
};

#endif /* THREADMANAGER_H_ */


