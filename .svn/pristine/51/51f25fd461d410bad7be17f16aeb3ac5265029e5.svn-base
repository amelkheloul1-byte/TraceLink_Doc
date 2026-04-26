#include "mainwindow.h"
#include <QFileDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("L2G1 - Outil de Traçabilité");
    resize(900, 600);

    tabWidget = new QTabWidget(this);
    setCentralWidget(tabWidget);

    construireOngletImport();
    construireOngletGraphe();
    construireOngletRapport();

    tabWidget->addTab(ongletImport,  "Import des fichiers");
    tabWidget->addTab(ongletGraphe,  "Graphe de traçabilité");
    tabWidget->addTab(ongletRapport, "Rapport & Taux");
}
/**
 * @brief Destructeur de la fenêtre principale.
*/

MainWindow::~MainWindow() {}

/**
 * @brief Construit l'onglet Import des fichiers.
 * Crée les zones de sélection SSS, SRS, SDD et le bouton Lancer.
 * Le bouton Lancer est grisé par défaut.
 */

void MainWindow::construireOngletImport()
{
    ongletImport = new QWidget();
    QVBoxLayout *layoutPrincipal = new QVBoxLayout(ongletImport);
    layoutPrincipal->setSpacing(20);
    layoutPrincipal->setContentsMargins(30, 30, 30, 30);

    // ── Zone SSS ──
    QGroupBox   *groupSSS  = new QGroupBox("Fichier SSS (Exigences client)");
    QHBoxLayout *layoutSSS = new QHBoxLayout(groupSSS);
    champSSS       = new QLineEdit();
    champSSS->setPlaceholderText("Aucun fichier sélectionné...");
    champSSS->setReadOnly(true);
    btnParcourirSSS = new QPushButton("Parcourir");
    layoutSSS->addWidget(champSSS);
    layoutSSS->addWidget(btnParcourirSSS);

    // ── Zone SRS ──
    QGroupBox   *groupSRS  = new QGroupBox("Fichier SRS (Exigences projet)");
    QHBoxLayout *layoutSRS = new QHBoxLayout(groupSRS);
    champSRS       = new QLineEdit();
    champSRS->setPlaceholderText("Aucun fichier sélectionné...");
    champSRS->setReadOnly(true);
    btnParcourirSRS = new QPushButton("Parcourir");
    layoutSRS->addWidget(champSRS);
    layoutSRS->addWidget(btnParcourirSRS);

    // ── Zone SDD ──
    QGroupBox   *groupSDD  = new QGroupBox("Fichier SDD (Exigences conception) — optionnel");
    QHBoxLayout *layoutSDD = new QHBoxLayout(groupSDD);
    champSDD       = new QLineEdit();
    champSDD->setPlaceholderText("Aucun fichier sélectionné...");
    champSDD->setReadOnly(true);
    btnParcourirSDD = new QPushButton("Parcourir");
    layoutSDD->addWidget(champSDD);
    layoutSDD->addWidget(btnParcourirSDD);

    // ── Bouton Lancer ──
    btnLancer = new QPushButton("Lancer l'analyse");
    btnLancer->setEnabled(false);   // grisé par défaut
    btnLancer->setMinimumHeight(40);

    layoutPrincipal->addWidget(groupSSS);
    layoutPrincipal->addWidget(groupSRS);
    layoutPrincipal->addWidget(groupSDD);
    layoutPrincipal->addStretch();
    layoutPrincipal->addWidget(btnLancer);

    // ── Connexions ──
    connect(btnParcourirSSS, &QPushButton::clicked, this, &MainWindow::parcourirSSS);
    connect(btnParcourirSRS, &QPushButton::clicked, this, &MainWindow::parcourirSRS);
    connect(btnParcourirSDD, &QPushButton::clicked, this, &MainWindow::parcourirSDD);
}

/**
 * @brief Construit l'onglet Graphe de traçabilité.
 * Placeholder — sera implémenté lors de la connexion avec le Module 2.
 */

void MainWindow::construireOngletGraphe()
{
    ongletGraphe = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(ongletGraphe);
    QLabel *label = new QLabel("Le graphe de traçabilité s'affichera ici après l'analyse.");
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);
}

/**
 * @brief Construit l'onglet Rapport et Taux.
 * Placeholder — sera implémenté lors de la connexion avec le Module 2.
 */

void MainWindow::construireOngletRapport()
{
    ongletRapport = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(ongletRapport);
    QLabel *label = new QLabel("Le rapport et les taux de couverture s'afficheront ici après l'analyse.");
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);
}

/**
 * @brief Ouvre un QFileDialog pour sélectionner le fichier SSS.
 * Met à jour le champ texte et vérifie si le bouton Lancer peut s'activer.
 */

void MainWindow::parcourirSSS()
{
    QString chemin = QFileDialog::getOpenFileName(
        this,
        "Sélectionner le fichier SSS",
        "",
        "Fichiers texte (*.txt *.csv);;Tous les fichiers (*)"
    );
    if (!chemin.isEmpty()) {
        cheminSSS = chemin;
        champSSS->setText(chemin);
        verifierFichiersSelectionnes();
    }
}

/**
 * @brief Ouvre un QFileDialog pour sélectionner le fichier SRS.
 * Met à jour le champ texte et vérifie si le bouton Lancer peut s'activer.
 */

void MainWindow::parcourirSRS()
{
    QString chemin = QFileDialog::getOpenFileName(
        this,
        "Sélectionner le fichier SRS",
        "",
        "Fichiers texte (*.txt *.csv);;Tous les fichiers (*)"
    );
    if (!chemin.isEmpty()) {
        cheminSRS = chemin;
        champSRS->setText(chemin);
        verifierFichiersSelectionnes();
    }
}

/**
 * @brief Ouvre un QFileDialog pour sélectionner le fichier SDD (optionnel).
 * Met à jour le champ texte. Ne bloque pas le bouton Lancer si absent.
 */

void MainWindow::parcourirSDD()
{
    QString chemin = QFileDialog::getOpenFileName(
        this,
        "Sélectionner le fichier SDD",
        "",
        "Fichiers texte (*.txt *.csv);;Tous les fichiers (*)"
    );
    if (!chemin.isEmpty()) {
        cheminSDD = chemin;
        champSDD->setText(chemin);
    }
}

/**
 * @brief Vérifie si SSS et SRS sont sélectionnés pour activer le bouton Lancer.
 * Le fichier SDD est optionnel et n'est pas pris en compte.
 */

void MainWindow::verifierFichiersSelectionnes()
{
    bool pret = !cheminSSS.isEmpty() && !cheminSRS.isEmpty();
    btnLancer->setEnabled(pret);
}
