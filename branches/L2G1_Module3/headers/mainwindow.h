#ifndef MAINWINDOW_H
#define MAINWINDOW_H
/**
 * @file mainwindow.h
 * @brief Fenêtre principale de TraceLink — Module 3.
 * @author Amel Kheloul, Lamia Arrahmane
 * @version 7.1 — Ajout label "Fichiers sélectionnés" + item vide par défaut
 */
#include <QMainWindow>
#include <QTabWidget>
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
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
    void parcourirSSS();
    void parcourirSRS();
    void parcourirSDD();
    void verifierFichiersSelectionnes();
    void reinitialiser();
    void lancerAnalyse();
    void filtrerApercu(const QString &texte);
private:
    // Onglets principaux
    QTabWidget             *tabWidget;
    QWidget                *ongletImport;
    OngletRapport          *ongletRapport;
    VueGrapheTracabilite   *ongletGraphe;
    GestionnaireFiltresGUI *gestionnaireFiltres;
    QWidget                *ongletGrapheConteneur;
    QWidget                *ongletRapportConteneur;
    // Onglets fichiers SSS/SRS/SDD
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
    QListWidget *listeSSS;
    QListWidget *listeSRS;
    QListWidget *listeSDD;
    // Préfixes configurables par l'utilisateur
    QLineEdit   *champPrefixeGlobal; ///< Préfixe commun ex: SPEC_, REQ_, EX_
    QLineEdit   *champPrefixeSSS;    ///< Préfixe identifiant les exigences SSS
    QLineEdit   *champPrefixeSRS;    ///< Préfixe identifiant les exigences SRS
    QLineEdit   *champPrefixeSDD;    ///< Préfixe identifiant les exigences SDD
    // Boutons + statut
    QPushButton *btnLancer;
    QPushButton *btnReinitialiser;
    QLabel      *labelStatut;
    // Terminal + progression
    QTextEdit    *terminal;
    QProgressBar *barreProgression;
    // Aperçu exigences
    QListWidget *listeExigences;
    QLineEdit   *champRechercheId;
    QLabel      *labelNbExigences;
    QStringList  toutesExigences;
    // Chemins : listes pour sélection multiple
    QStringList cheminsSSS;
    QStringList cheminsSRS;
    QStringList cheminsSDD;
    // Thread
    MoteurTracabilite *moteurAnalyse = nullptr;
    ThreadManager     *threadManager = nullptr;
    // Méthodes
    void construireOngletImport();
    void construireOngletGraphe();
    void construireOngletRapport();
    bool formatAccepte(const QString &chemin);
    void mettreAJourOngletFichier(int index, const QString &etat);
    void logTerminal(const QString &message, const QString &couleur = "#D4D4D4");
    void afficherApercuExigences(const std::vector<ExigenceExtraite> &sss,
                                 const std::vector<ExigenceExtraite> &srs,
                                 const std::vector<ExigenceExtraite> &sdd);
    /// @brief Ajoute l'item "Aucun fichier sélectionné" par défaut dans une liste.
    void ajouterItemVide(QListWidget *liste);
};
#endif // MAINWINDOW_H