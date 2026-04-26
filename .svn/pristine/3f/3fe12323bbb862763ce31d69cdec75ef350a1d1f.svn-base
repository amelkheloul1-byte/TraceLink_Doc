#include "accueil.h"
#include <QVBoxLayout>
#include <QLabel>

/**
 * @brief Constructeur de la fenêtre de démarrage.
 * @param parent Widget parent.
 */
Accueil::Accueil(QWidget *parent)
    : QWidget(parent), progression(0)
{
    setWindowTitle("TraceLink");
    setFixedSize(500, 300);

    // Supprimer la barre titre Windows + coins arrondis style Apple
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    setupUI();

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Accueil::avancerChargement);
    timer->start(30);
}

/**
 * @brief Initialise l'interface de la fenêtre de démarrage.
 */
void Accueil::setupUI()
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(40, 40, 40, 40);
    layout->setSpacing(20);

    // Fond sombre avec coins arrondis
    setStyleSheet(R"(
        Acceuil {
            background-color: #1A1A2E;
            border-radius: 20px;
        }
    )");

    // Nom du logiciel
    QLabel *labelNom = new QLabel("TraceLink");
    labelNom->setAlignment(Qt::AlignCenter);
    labelNom->setStyleSheet(R"(
        font-size: 42px;
        font-weight: bold;
        color: #4A90D9;
        letter-spacing: 4px;
    )");

    // ── Sous-titre ──
    QLabel *labelSous = new QLabel("Outil de traçabilité automatisé — L2G1");
    labelSous->setAlignment(Qt::AlignCenter);
    labelSous->setStyleSheet("color: #AAAACC; font-size: 11px;");

    // ── Barre de chargement ──
    barreChargement = new QProgressBar();
    barreChargement->setRange(0, 100);
    barreChargement->setValue(0);
    barreChargement->setTextVisible(false);
    barreChargement->setFixedHeight(6);
    barreChargement->setStyleSheet(R"(
        QProgressBar {
            background-color: #2A2A4A;
            border-radius: 3px;
            border: none;
        }
        QProgressBar::chunk {
            background-color: #4A90D9;
            border-radius: 3px;
        }
    )");

    // ── Bouton Démarrer ──
    btnDemarrer = new QPushButton("Démarrer");
    btnDemarrer->setEnabled(false);
    btnDemarrer->setFixedHeight(40);
    btnDemarrer->setStyleSheet(R"(
        QPushButton {
            background-color: #4A90D9;
            color: white;
            border-radius: 10px;
            font-size: 13px;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: #357ABD;
        }
        QPushButton:disabled {
            background-color: #2A2A4A;
            color: #555577;
        }
    )");

    layout->addStretch();
    layout->addWidget(labelNom);
    layout->addWidget(labelSous);
    layout->addStretch();
    layout->addWidget(barreChargement);
    layout->addWidget(btnDemarrer);

    connect(btnDemarrer, &QPushButton::clicked, this, &Accueil::demarrerClique);
}

/**
 * @brief Avance la barre de chargement automatiquement.
 * Active le bouton Démarrer quand le chargement est terminé.
 */
void Accueil::avancerChargement()
{
    progression++;
    barreChargement->setValue(progression);
    if (progression >= 100) {
        timer->stop();
        btnDemarrer->setEnabled(true);
    }
}
