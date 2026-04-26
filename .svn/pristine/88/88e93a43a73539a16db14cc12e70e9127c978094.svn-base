/**
 * @file GestionnaireFiltresGUI.cpp
 * @brief Implémentation de la classe GestionnaireFiltresGUI.
 * @author Lamia Arrahmane
 * @date 02/04/2026
 * @version 1.0
 */

#include "GestionnaireFiltresGUI.h"
#include <QHBoxLayout>
#include <QFrame>

GestionnaireFiltresGUI::GestionnaireFiltresGUI(QWidget *parent)
    : QWidget(parent)
{
    initialiserUI();
}

QLabel* GestionnaireFiltresGUI::creerLabel(const QString& texte)
{
    QLabel *lbl = new QLabel(texte, this);
    lbl->setStyleSheet("color: #555; font-size: 11px; font-weight: bold; margin-top: 6px;");
    return lbl;
}

void GestionnaireFiltresGUI::initialiserUI()
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(10, 10, 10, 10);
    layout->setSpacing(6);

    // ── Titre ────────────────────────────────────────────────────────────────
    QLabel *titre = new QLabel("Filtres", this);
    titre->setStyleSheet(R"(
        color: #1A5276;
        font-size: 15px;
        font-weight: bold;
        padding-bottom: 6px;
        border-bottom: 2px solid #AED6F1;
    )");
    layout->addWidget(titre);

    // ── Document ─────────────────────────────────────────────────────────────
    layout->addWidget(creerLabel("Document :"));

    comboDocument = new QComboBox(this);
    comboDocument->addItem("Tous", TOUS_DOCS);
    comboDocument->addItem("SSS",  DOC_SSS);
    comboDocument->addItem("SRS",  DOC_SRS);
    comboDocument->addItem("SDD",  DOC_SDD);
    comboDocument->setStyleSheet(R"(
        QComboBox {
            background-color: white;
            color: #333;
            border: 1px solid #AED6F1;
            border-radius: 6px;
            padding: 5px 10px;
            font-size: 11px;
        }
        QComboBox::drop-down { border: none; }
        QComboBox QAbstractItemView {
            background: white;
            color: #333;
            selection-background-color: #AED6F1;
        }
    )");
    layout->addWidget(comboDocument);

    // ── Statut ───────────────────────────────────────────────────────────────
    layout->addWidget(creerLabel("Statut :"));

    comboStatut = new QComboBox(this);
    comboStatut->addItem("Tous",         -1);
    comboStatut->addItem("Couverte",     Couverte);
    comboStatut->addItem("Non couverte", NonCouverte);
    comboStatut->addItem("Non analysé",  NonAnalyse);
    comboStatut->setStyleSheet(comboDocument->styleSheet());
    layout->addWidget(comboStatut);

    // ── Recherche ─────────────────────────────────────────────────────────────
    layout->addWidget(creerLabel("Recherche :"));

    champIdentifiant = new QLineEdit(this);
    champIdentifiant->setPlaceholderText("ID. Exigence");
    champIdentifiant->setStyleSheet(R"(
        QLineEdit {
            background-color: white;
            color: #333;
            border: 1px solid #AED6F1;
            border-radius: 6px;
            padding: 5px 10px;
            font-size: 11px;
        }
        QLineEdit:focus { border: 1px solid #4A90D9; }
    )");
    layout->addWidget(champIdentifiant);

    layout->addSpacing(8);

    // ── Bouton Appliquer ──────────────────────────────────────────────────────
    btnAppliquer = new QPushButton("Appliquer filtres", this);
    btnAppliquer->setStyleSheet(R"(
        QPushButton {
            background-color: #2E86C1;
            color: white;
            border-radius: 8px;
            padding: 8px;
            font-size: 12px;
            font-weight: bold;
        }
        QPushButton:hover { background-color: #1A5276; }
        QPushButton:pressed { background-color: #154360; }
    )");
    connect(btnAppliquer, &QPushButton::clicked, this, &GestionnaireFiltresGUI::appliquerFiltres);
    layout->addWidget(btnAppliquer);

    // ── Séparateur ────────────────────────────────────────────────────────────
    layout->addSpacing(10);
    QFrame *separateur = new QFrame(this);
    separateur->setFrameShape(QFrame::HLine);
    separateur->setStyleSheet("color: #AED6F1;");
    layout->addWidget(separateur);

    // ── Légende ───────────────────────────────────────────────────────────────
    QLabel *titreLeg = new QLabel("LÉGENDE", this);
    titreLeg->setStyleSheet("color: #888; font-size: 10px; font-weight: bold; letter-spacing: 1px;");
    layout->addWidget(titreLeg);

    auto ajouterLegende = [&](const QString& texte, const QString& couleur) {
        QHBoxLayout *row = new QHBoxLayout();
        QLabel *carre = new QLabel(this);
        carre->setFixedSize(14, 14);
        carre->setStyleSheet(QString("background-color: %1; border-radius: 3px;").arg(couleur));
        QLabel *lbl = new QLabel(texte, this);
        lbl->setStyleSheet("color: #555; font-size: 11px;");
        row->addWidget(carre);
        row->addWidget(lbl);
        row->addStretch();
        layout->addLayout(row);
    };

    ajouterLegende("Couvert",     "#27AE60");
    ajouterLegende("Non couvert", "#E74C3C");
    ajouterLegende("Lien brisé",  "#E67E22");

    layout->addStretch();

    // ── Bouton Réinitialiser ──────────────────────────────────────────────────
    btnReinitialiser = new QPushButton("↺ Réinitialiser", this);
    btnReinitialiser->setStyleSheet(R"(
        QPushButton {
            background-color: #EBF5FB;
            color: #2E86C1;
            border: 1px solid #AED6F1;
            border-radius: 6px;
            padding: 5px;
            font-size: 11px;
        }
        QPushButton:hover { background-color: #D6EAF8; }
    )");
    connect(btnReinitialiser, &QPushButton::clicked, this, &GestionnaireFiltresGUI::reinitialiser);
    layout->addWidget(btnReinitialiser);

    setLayout(layout);
}

FiltreGUI GestionnaireFiltresGUI::getFiltreActuel() const
{
    return filtreCourant;
}

void GestionnaireFiltresGUI::appliquerFiltres()
{
    filtreCourant.filtreDocument    = static_cast<TypeDoc>(comboDocument->currentData().toInt());
    filtreCourant.filtreStatut      = static_cast<Statut>(comboStatut->currentData().toInt());
    filtreCourant.filtreIdentifiant = champIdentifiant->text();
    emit filtreModifie(filtreCourant);
}

void GestionnaireFiltresGUI::setFiltreDocument(int index)
{
    filtreCourant.filtreDocument = static_cast<TypeDoc>(
        comboDocument->itemData(index).toInt());
}

void GestionnaireFiltresGUI::setFiltreStatut(int index)
{
    int val = comboStatut->itemData(index).toInt();
    filtreCourant.filtreStatut = static_cast<Statut>(val);
}

void GestionnaireFiltresGUI::setFiltreIdentifiant(const QString& texte)
{
    filtreCourant.filtreIdentifiant = texte;
}

void GestionnaireFiltresGUI::reinitialiser()
{
    comboDocument->setCurrentIndex(0);
    comboStatut->setCurrentIndex(0);
    champIdentifiant->clear();
    filtreCourant = FiltreGUI();
    emit filtreModifie(filtreCourant);
}
