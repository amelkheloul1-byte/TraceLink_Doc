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

    setupUI();

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Accueil::avancerChargement);
    timer->start(30);
}

/**
 * @brief Initialise l'interface de la fenêtre de démarrage.
 * Thème clair gris professionnel.
 */
void Accueil::setupUI()
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(40, 40, 40, 40);
    layout->setSpacing(20);

    // Fond clair avec coins arrondis
    setStyleSheet(R"(
        Accueil {
            background-color: #EBF5FB;
            border-radius: 20px;
            border: 1px solid #AED6F1;
        }
    )");

    // ── Nom du logiciel ──
    QLabel *labelNom = new QLabel("TraceLink");
    labelNom->setAlignment(Qt::AlignCenter);
    labelNom->setStyleSheet(R"(
        font-size: 42px;
        font-weight: bold;
        color: #2E86C1;
        letter-spacing: 4px;
    )");

    // ── Sous-titre ──
    QLabel *labelSous = new QLabel("Outil de traçabilité automatisé — L2G1");
    labelSous->setAlignment(Qt::AlignCenter);
    labelSous->setStyleSheet("color: #888888; font-size: 11px;");

    // ── Barre de chargement ──
    barreChargement = new QProgressBar();
    barreChargement->setRange(0, 100);
    barreChargement->setValue(0);
    barreChargement->setTextVisible(false);
    barreChargement->setFixedHeight(6);
    barreChargement->setStyleSheet(R"(
        QProgressBar {
            background-color: #DDDDDD;
            border-radius: 3px;
            border: none;
        }
        QProgressBar::chunk {
            background-color: #2E86C1;
            border-radius: 3px;
        }
    )");

    // ── Bouton Démarrer ──
    btnDemarrer = new QPushButton("Démarrer");
    btnDemarrer->setEnabled(false);
    btnDemarrer->setFixedHeight(40);
    btnDemarrer->setStyleSheet(R"(
        QPushButton {
            background-color: #2E86C1;
            color: white;
            border-radius: 10px;
            font-size: 13px;
            font-weight: bold;
            border: none;
        }
        QPushButton:hover {
            background-color: #1A5276;
        }
        QPushButton:disabled {
            background-color: #DDDDDD;
            color: #999999;
            border: none;
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