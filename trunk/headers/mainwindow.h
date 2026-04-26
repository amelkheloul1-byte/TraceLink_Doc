#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/**
 * @file mainwindow.h
 * @brief Fenêtre principale de TraceLink — Module 3.
 * @author Emilie Huang, Lamia Arrahmane
 * @version 4.0 — ThreadManager intégré
 */

#include <QMainWindow>
#include <QTabWidget>
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QTextEdit>
#include <QListWidget>
#include <QProgressBar>
#include <QStringList>
#include <QTimer>

#include "OngletRapport.h"
#include "VueGrapheTracabilite.h"
#include "GestionnaireFiltresGUI.h"
#include "ExtracteurFichier.h"
#include "ConvertisseurExigence.h"
#include "MoteurTracabilite.h"
#include "ThreadManager.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    /// @brief Sélection multiple fichiers SSS.
    void parcourirSSS();
    /// @brief Sélection multiple fichiers SRS.
    void parcourirSRS();
    /// @brief Sélection multiple fichiers SDD.
    void parcourirSDD();
    /// @brief Active/désactive le bouton Lancer.
    void verifierFichiersSelectionnes();
    /// @brief Vide toutes les zones.
    void reinitialiser();
    /// @brief Lance l'analyse M1 → M2 avec ThreadManager.
    void lancerAnalyse();
    /// @brief Filtre l'aperçu des exigences par ID.
    void filtrerApercu(const QString &texte);

private:
    // ── Onglets principaux ────────────────────────────────────────────────────
    QTabWidget             *tabWidget;
    QWidget                *ongletImport;
    OngletRapport          *ongletRapport;
    VueGrapheTracabilite   *ongletGraphe;
    GestionnaireFiltresGUI *gestionnaireFiltres;
    QWidget                *ongletGrapheConteneur;
    QWidget                *ongletRapportConteneur;

    // ── Onglets fichiers SSS/SRS/SDD ─────────────────────────────────────────
    QTabWidget  *ongletsFichiers;
    QLineEdit   *champSSS;
    QLineEdit   *champSRS;
    QLineEdit   *champSDD;
    QPushButton *btnParcourirSSS;
    QPushButton *btnParcourirSRS;
    QPushButton *btnParcourirSDD;
    QLabel      *badgeSSS;
    QLabel      *badgeSRS;
    QLabel      *badgeSDD;
    QLabel      *erreurSSS;
    QLabel      *erreurSRS;
    QLabel      *erreurSDD;

    // ── Boutons + statut ──────────────────────────────────────────────────────
    QPushButton *btnLancer;
    QPushButton *btnReinitialiser;
    QLabel      *labelStatut;

    // ── Terminal + progression ────────────────────────────────────────────────
    QTextEdit    *terminal;
    QProgressBar *barreProgression;

    // ── Aperçu exigences ─────────────────────────────────────────────────────
    QListWidget *listeExigences;
    QLineEdit   *champRechercheId;
    QLabel      *labelNbExigences;
    QStringList  toutesExigences;

    // ── Chemins ──────────────────────────────────────────────────────────────
    QString cheminSSS;
    QString cheminSRS;
    QString cheminSDD;

    // ── ThreadManager + Moteur ────────────────────────────────────────────────
    MoteurTracabilite *moteurAnalyse = nullptr;
    ThreadManager     *threadManager = nullptr;

    // ── Méthodes ─────────────────────────────────────────────────────────────
    void construireOngletImport();
    void construireOngletGraphe();
    void construireOngletRapport();
    bool formatAccepte(const QString &chemin);
    void mettreAJourOngletFichier(int index, const QString &etat);
    void logTerminal(const QString &message, const QString &couleur = "#D4D4D4");
    void afficherApercuExigences(const std::vector<ExigenceExtraite> &sss,
                                 const std::vector<ExigenceExtraite> &srs,
                                 const std::vector<ExigenceExtraite> &sdd);
};

#endif // MAINWINDOW_H
