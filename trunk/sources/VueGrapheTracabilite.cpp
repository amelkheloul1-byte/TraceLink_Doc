/**
 * @file VueGrapheTracabilite.cpp
 * @brief Implémentation de la classe VueGrapheTracabilite.
 * @author Lamia Arrahmane
 * @date 02/04/2026
 * @version 1.0
 */

#include "VueGrapheTracabilite.h"
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFontMetrics>
#include <cmath>

/**
 * @brief Constructeur de la vue graphe de traçabilité.
 * @param parent Widget parent Qt.
 */
VueGrapheTracabilite::VueGrapheTracabilite(QWidget *parent)
    : QWidget(parent), niveauZoom(1.0)
{
    setMinimumSize(600, 400);
    setStyleSheet("background-color: #F0F4F8;");

    // ── Bouton filtres ────────────────────────────────────────────────────────
    boutonFiltres = new QPushButton("⚙ Filtres ▶", this);
    boutonFiltres->setStyleSheet(R"(
        QPushButton {
            background-color: #EBF5FB; color: #1A5276;
            border-radius: 6px; padding: 4px 10px;
            font-size: 11px; border: 1px solid #AED6F1;
        }
        QPushButton:hover { background-color: #AED6F1; }
    )");
    boutonFiltres->move(10, 10);
    boutonFiltres->resize(110, 28);
    connect(boutonFiltres, &QPushButton::clicked, this, &VueGrapheTracabilite::toggleFiltres);

    // ── Panneau filtres ───────────────────────────────────────────────────────
    panneauFiltres = new QFrame(this);
    panneauFiltres->setStyleSheet("QFrame { background-color: white; border-left: 2px solid #AED6F1; }");
    panneauFiltres->setVisible(false);

    QVBoxLayout *layoutFiltres = new QVBoxLayout(panneauFiltres);
    layoutFiltres->setContentsMargins(0, 0, 0, 0);
    gestionnaireFiltres = new GestionnaireFiltresGUI(panneauFiltres);
    layoutFiltres->addWidget(gestionnaireFiltres);
    connect(gestionnaireFiltres, &GestionnaireFiltresGUI::filtreModifie,
            this, &VueGrapheTracabilite::appliquerFiltre);

    // ── Panneau détail ────────────────────────────────────────────────────────
    panneauDetail = new QFrame(this);
    panneauDetail->setStyleSheet(R"(
        QFrame {
            background-color: white;
            border: 1px solid #AED6F1;
            border-radius: 8px;
        }
    )");
    panneauDetail->setVisible(false);

    QVBoxLayout *layoutDetail = new QVBoxLayout(panneauDetail);
    layoutDetail->setContentsMargins(12, 8, 12, 8);
    layoutDetail->setSpacing(6);

    QLabel *titreDetail = new QLabel("EXIGENCE SÉLECTIONNÉE", panneauDetail);
    titreDetail->setStyleSheet("color: #888; font-size: 9px; font-weight: bold; letter-spacing: 1px;");
    layoutDetail->addWidget(titreDetail);

    labelDetailId = new QLabel("—", panneauDetail);
    labelDetailId->setStyleSheet("color: #1A5276; font-size: 16px; font-weight: bold;");
    layoutDetail->addWidget(labelDetailId);

    QHBoxLayout *rowDetail = new QHBoxLayout();
    rowDetail->setSpacing(8);
    labelDetailType   = new QLabel("", panneauDetail);
    labelDetailStatut = new QLabel("", panneauDetail);
    labelDetailLiens  = new QLabel("", panneauDetail);
    labelDetailLiens->setStyleSheet("color: #888; font-size: 11px;");
    rowDetail->addWidget(labelDetailType);
    rowDetail->addWidget(labelDetailStatut);
    rowDetail->addWidget(labelDetailLiens);
    rowDetail->addStretch();
    layoutDetail->addLayout(rowDetail);
}

/**
 * @brief Redimensionne le panneau de filtres et le panneau détail avec le widget.
 * @param event Événement de redimensionnement Qt.
 */
void VueGrapheTracabilite::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    panneauFiltres->setGeometry(width() - 220, 0, 220, height());
    boutonFiltres->move(width() - 130, 10);
    panneauDetail->setGeometry(0, height() - 100,
                               width() - (panneauFiltres->isVisible() ? 220 : 0), 100);
}

/**
 * @brief Affiche ou masque le panneau de filtres latéral.
 */
void VueGrapheTracabilite::toggleFiltres()
{
    bool visible = panneauFiltres->isVisible();
    panneauFiltres->setVisible(!visible);
    boutonFiltres->setText(visible ? "⚙ Filtres ▶" : "⚙ Filtres ◀");
    resizeEvent(nullptr);
    update();
}

// ─── Construction du graphe ───────────────────────────────────────────────────

/**
 * @brief Construit le graphe à partir du rapport de traçabilité et des exigences.
 *
 * Crée un nœud par exigence (SSS à gauche, SRS au centre, SDD à droite),
 * et une arête par lien de traçabilité avec le pourcentage de couverture calculé.
 * Le pourcentage sur chaque arête est réparti équitablement entre tous les liens
 * qui couvrent la même exigence source.
 *
 * @param rapport Le rapport produit par le MoteurTracabilite.
 * @param srs     Vecteur des exigences SRS analysées.
 * @param sdd     Vecteur des exigences SDD analysées.
 */
void VueGrapheTracabilite::construireGraphe(const RapportTracabilite& rapport,
                                            const std::vector<ExigenceSRS>& srs,
                                            const std::vector<ExigenceSDD>& sdd)
{
    noeuds.clear();
    aretes.clear();
    srsAnalyses    = srs;
    sddAnalyses    = sdd;
    rapportCourant = rapport;

    int espacementV = 120;
    int xSSS = 170, xSRS = 520, xSDD = 870;

    // ── Nœuds SSS ────────────────────────────────────────────────────────────
    for (int i = 0; i < (int)rapport.exigencesSSS.size(); i++) {
        NoeudGraphe n;
        n.id       = rapport.exigencesSSS[i].getId();
        n.type     = SSS;
        n.statut   = rapport.exigencesSSS[i].getStatut();
        n.position = QPointF(xSSS, 80 + i * espacementV);
        noeuds.push_back(n);
    }

    // ── Nœuds SRS ────────────────────────────────────────────────────────────
    for (int i = 0; i < (int)srs.size(); i++) {
        NoeudGraphe n;
        n.id       = srs[i].getId();
        n.type     = SRS;
        n.statut   = srs[i].getStatut();
        n.position = QPointF(xSRS, 80 + i * espacementV);
        noeuds.push_back(n);
    }

    // ── Nœuds SDD ────────────────────────────────────────────────────────────
    for (int i = 0; i < (int)sdd.size(); i++) {
        NoeudGraphe n;
        n.id       = sdd[i].getId();
        n.type     = SDD;
        n.statut   = Couverte; // Une SDD existe donc elle couvre
        n.position = QPointF(xSDD, 80 + i * espacementV);
        noeuds.push_back(n);
    }

    // ── Arêtes SSS → SRS avec pourcentage ────────────────────────────────────
    for (const ExigenceSRS& s : srs) {
        for (const std::string& ref : s.getTracabilite()) {
            // Compter le nombre total de SRS qui couvrent ce même ref (SSS)
            int nbLiens = 0;
            for (const ExigenceSRS& autre : srs) {
                const auto& trac = autre.getTracabilite();
                if (std::find(trac.begin(), trac.end(), ref) != trac.end())
                    nbLiens++;
            }

            AreteGraphe a;
            a.idSource    = ref;
            a.idCible     = s.getId();
            a.estBrise    = false;
            a.pourcentage = nbLiens > 0 ? (100.0 / nbLiens) : 0.0;

            // Vérifier si lien brisé
            for (const std::string& lb : rapport.liensBrises) {
                if (lb.find(s.getId()) != std::string::npos &&
                    lb.find(ref) != std::string::npos) {
                    a.estBrise    = true;
                    a.pourcentage = 0.0;
                    break;
                }
            }
            aretes.push_back(a);
        }
    }

    // ── Arêtes SRS → SDD avec pourcentage ────────────────────────────────────
    for (const ExigenceSDD& d : sdd) {
        for (const std::string& ref : d.getReferencedSRS()) {
            // Compter le nombre total de SDD qui couvrent ce même ref (SRS)
            int nbLiens = 0;
            for (const ExigenceSDD& autre : sdd) {
                const auto& r = autre.getReferencedSRS();
                if (std::find(r.begin(), r.end(), ref) != r.end())
                    nbLiens++;
            }

            AreteGraphe a;
            a.idSource    = ref;
            a.idCible     = d.getId();
            a.estBrise    = false;
            a.pourcentage = nbLiens > 0 ? (100.0 / nbLiens) : 0.0;

            // Vérifier si lien brisé
            for (const std::string& lb : rapport.liensBrises) {
                if (lb.find(d.getId()) != std::string::npos &&
                    lb.find(ref) != std::string::npos) {
                    a.estBrise    = true;
                    a.pourcentage = 0.0;
                    break;
                }
            }
            aretes.push_back(a);
        }
    }

    update();
}

// ─── Dessin ───────────────────────────────────────────────────────────────────

/**
 * @brief Redessine le graphe complet.
 * @param event Événement de peinture Qt.
 */
void VueGrapheTracabilite::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.scale(niveauZoom, niveauZoom);

    // Fond
    painter.fillRect(QRectF(0, 0, width() / niveauZoom, height() / niveauZoom),
                     QColor("#F0F4F8"));

    // Titres colonnes
    QFont fontTitre;
    fontTitre.setPointSize(10);
    fontTitre.setBold(true);
    painter.setFont(fontTitre);
    painter.setPen(QColor("#1A5276"));
    painter.drawText(QRectF(70,  20, 200, 30), Qt::AlignCenter, "SSS");
    painter.drawText(QRectF(420, 20, 200, 30), Qt::AlignCenter, "SRS");
    painter.drawText(QRectF(770, 20, 200, 30), Qt::AlignCenter, "SDD");

    // Lignes de séparation en pointillés
    painter.setPen(QPen(QColor("#AED6F1"), 1, Qt::DashLine));
    painter.drawLine(QPointF(320, 10), QPointF(320, height() / niveauZoom - 110));
    painter.drawLine(QPointF(670, 10), QPointF(670, height() / niveauZoom - 110));

    // Arêtes d'abord (sous les nœuds)
    for (const AreteGraphe& a : aretes)
        dessinerArete(painter, a);

    // Nœuds ensuite (par-dessus les arêtes)
    for (const NoeudGraphe& n : noeuds)
        dessinerNoeud(painter, n);
}

/**
 * @brief Vérifie si un nœud doit être affiché selon le filtre actif.
 * @param n Le nœud à tester.
 * @return true si le nœud est visible, false sinon.
 */
bool VueGrapheTracabilite::noeudVisible(const NoeudGraphe& n) const
{
    // Filtre par type de document
    if (filtreActif.filtreDocument != TOUS_DOCS) {
        if (filtreActif.filtreDocument == DOC_SSS && n.type != SSS) return false;
        if (filtreActif.filtreDocument == DOC_SRS && n.type != SRS) return false;
        if (filtreActif.filtreDocument == DOC_SDD && n.type != SDD) return false;
    }
    // Filtre par statut
    if (filtreActif.filtreStatut != NonAnalyse && n.statut != filtreActif.filtreStatut)
        return false;
    // Filtre par identifiant
    if (!filtreActif.filtreIdentifiant.isEmpty()) {
        QString id = QString::fromStdString(n.id);
        if (!id.contains(filtreActif.filtreIdentifiant, Qt::CaseInsensitive))
            return false;
    }
    return true;
}

/**
 * @brief Dessine un nœud coloré avec son identifiant et son pourcentage de couverture.
 *
 * Code couleur :
 * - Vert  : exigence couverte (100%)
 * - Rouge : exigence non couverte (0%)
 * - Orange : lien brisé
 *
 * @param painter Le QPainter actif.
 * @param noeud   Le nœud à dessiner.
 */
void VueGrapheTracabilite::dessinerNoeud(QPainter& painter, const NoeudGraphe& noeud)
{
    if (!noeudVisible(noeud)) return;

    QColor couleur = couleurStatut(noeud.statut);
    bool selectionne = (noeud.id == noeudSelectionne);

    // Ombre
    painter.setBrush(QColor(0, 0, 0, 30));
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(QRectF(noeud.position.x() - 98, noeud.position.y() - 23,
                                   196, 46), 10, 10);

    // Nœud principal
    painter.setBrush(QBrush(couleur));
    painter.setPen(QPen(selectionne ? QColor("#F39C12") : couleur.darker(130),
                        selectionne ? 3 : 1));
    painter.drawRoundedRect(QRectF(noeud.position.x() - 100, noeud.position.y() - 25,
                                   200, 50), 10, 10);

    // Texte ID
    painter.setPen(Qt::white);
    QFont font = painter.font();
    font.setPointSize(8);
    font.setBold(true);
    painter.setFont(font);
    QString label = QString::fromStdString(noeud.id);
    painter.drawText(QRectF(noeud.position.x() - 95, noeud.position.y() - 20,
                            190, 28),
                     Qt::AlignCenter | Qt::TextWordWrap, label);

    // Pourcentage de couverture sur le nœud
    QString pct;
    switch (noeud.statut) {
    case Couverte:    pct = "100%"; break;
    case NonCouverte: pct = "0%";   break;
    default:          pct = "0%";   break;
    }

    QFont fontPct = painter.font();
    fontPct.setPointSize(7);
    fontPct.setBold(false);
    painter.setFont(fontPct);
    painter.setPen(QColor(255, 255, 255, 200));
    painter.drawText(QRectF(noeud.position.x() - 95, noeud.position.y() + 8,
                            190, 16),
                     Qt::AlignCenter, pct);
}

/**
 * @brief Dessine une arête avec une flèche et le pourcentage de couverture au milieu.
 *
 * Le pourcentage affiché représente la contribution de ce lien à la couverture
 * de l'exigence source. Si plusieurs liens couvrent la même exigence, le
 * pourcentage est réparti équitablement (ex: 3 liens → 33% chacun).
 * Les liens brisés sont affichés en pointillés orange avec 0%.
 *
 * @param painter Le QPainter actif.
 * @param arete   L'arête à dessiner.
 */
void VueGrapheTracabilite::dessinerArete(QPainter& painter, const AreteGraphe& arete)
{
    // Chercher positions source et cible
    QPointF posSource, posCible;
    bool sourceVisible = false, cibleVisible = false;

    for (const NoeudGraphe& n : noeuds) {
        if (n.id == arete.idSource) { posSource = n.position; sourceVisible = noeudVisible(n); }
        if (n.id == arete.idCible)  { posCible  = n.position; cibleVisible  = noeudVisible(n); }
    }

    if (!sourceVisible || !cibleVisible) return;

    // Style selon lien brisé ou non
    QColor couleurArete = arete.estBrise ? QColor("#E67E22") : QColor("#27AE60");
    Qt::PenStyle style  = arete.estBrise ? Qt::DashLine : Qt::SolidLine;

    // Points de départ/arrivée (sens inversé : cible → source)
    QPointF debut(posCible.x() - 100, posCible.y());
    QPointF fin(posSource.x() + 100, posSource.y());

    painter.setPen(QPen(couleurArete, 2, style));
    painter.drawLine(debut, fin);

    // Flèche à l'arrivée
    double angle = std::atan2(fin.y() - debut.y(), fin.x() - debut.x());
    double taille = 10.0;
    QPointF p1(fin.x() - taille * std::cos(angle - 0.4),
               fin.y() - taille * std::sin(angle - 0.4));
    QPointF p2(fin.x() - taille * std::cos(angle + 0.4),
               fin.y() - taille * std::sin(angle + 0.4));

    painter.setBrush(couleurArete);
    painter.setPen(Qt::NoPen);
    QPolygonF fleche;
    fleche << fin << p1 << p2;
    painter.drawPolygon(fleche);

    // ── Pourcentage au milieu de l'arête ─────────────────────────────────────
    QPointF milieu((debut.x() + fin.x()) / 2,
                   (debut.y() + fin.y()) / 2 - 10);

    QString pourcent = QString("%1%").arg(arete.pourcentage, 0, 'f', 0);

    QFont fontPct = painter.font();
    fontPct.setPointSize(7);
    fontPct.setBold(true);
    painter.setFont(fontPct);

    QFontMetrics fm(fontPct);
    int tw = fm.horizontalAdvance(pourcent) + 8;
    int th = fm.height() + 4;

    // Fond blanc arrondi derrière le texte
    painter.setBrush(Qt::white);
    painter.setPen(QPen(couleurArete, 1));
    painter.drawRoundedRect(QRectF(milieu.x() - tw / 2, milieu.y() - th / 2, tw, th), 4, 4);

    // Texte du pourcentage
    painter.setPen(couleurArete);
    painter.drawText(QRectF(milieu.x() - tw / 2, milieu.y() - th / 2, tw, th),
                     Qt::AlignCenter, pourcent);
}

/**
 * @brief Retourne la couleur d'un nœud selon son statut de couverture.
 * @param statut Le statut de l'exigence.
 * @return La couleur correspondante.
 */
QColor VueGrapheTracabilite::couleurStatut(Statut statut) const
{
    switch (statut) {
    case Couverte:    return QColor("#27AE60");
    case NonCouverte: return QColor("#E74C3C");
    default:          return QColor("#E67E22");
    }
}

/**
 * @brief Gère le zoom à la molette.
 * @param event Événement molette Qt.
 */
void VueGrapheTracabilite::wheelEvent(QWheelEvent *event)
{
    if (event->angleDelta().y() > 0)
        niveauZoom *= 1.1;
    else
        niveauZoom /= 1.1;
    niveauZoom = qBound(0.3, niveauZoom, 3.0);
    update();
}

/**
 * @brief Gère le clic sur un nœud pour afficher ses détails.
 * @param event Événement souris Qt.
 */
void VueGrapheTracabilite::mousePressEvent(QMouseEvent *event)
{
    QPointF posClick = QPointF(event->pos()) / niveauZoom;
    const NoeudGraphe* noeud = noeudAPosition(posClick);

    if (noeud) {
        noeudSelectionne = noeud->id;
        afficherDetailNoeud(*noeud);
        update();
    }
}

/**
 * @brief Affiche les détails d'un nœud dans le panneau de détail.
 *
 * Affiche l'identifiant, le type (SSS/SRS/SDD), le statut et
 * les liens associés (SRS couvrant ce SSS, ou SDD couvrant ce SRS).
 *
 * @param noeud Le nœud sélectionné.
 */
void VueGrapheTracabilite::afficherDetailNoeud(const NoeudGraphe& noeud)
{
    panneauDetail->setVisible(true);
    resizeEvent(nullptr);

    labelDetailId->setText(QString::fromStdString(noeud.id));

    // Badge type
    QString typeStr, styleType;
    if (noeud.type == SSS) {
        typeStr   = " SSS ";
        styleType = "background-color:#D6EAF8; color:#1A5276; border-radius:6px; padding:2px 8px; font-size:11px; font-weight:bold;";
    } else if (noeud.type == SRS) {
        typeStr   = " SRS ";
        styleType = "background-color:#FAE5D3; color:#E67E22; border-radius:6px; padding:2px 8px; font-size:11px; font-weight:bold;";
    } else {
        typeStr   = " SDD ";
        styleType = "background-color:#E8DAEF; color:#8E44AD; border-radius:6px; padding:2px 8px; font-size:11px; font-weight:bold;";
    }
    labelDetailType->setText(typeStr);
    labelDetailType->setStyleSheet(styleType);

    // Badge statut
    QString statut, styleStat;
    switch (noeud.statut) {
    case Couverte:
        statut   = " Couvert ";
        styleStat = "background-color:#27AE60; color:white; border-radius:6px; padding:2px 8px; font-size:11px; font-weight:bold;";
        break;
    case NonCouverte:
        statut   = " Non couvert ";
        styleStat = "background-color:#E74C3C; color:white; border-radius:6px; padding:2px 8px; font-size:11px; font-weight:bold;";
        break;
    default:
        statut   = " Non analysé ";
        styleStat = "background-color:#AAAAAA; color:white; border-radius:6px; padding:2px 8px; font-size:11px; font-weight:bold;";
        break;
    }
    labelDetailStatut->setText(statut);
    labelDetailStatut->setStyleSheet(styleStat);

    // Liens associés
    QStringList liens;
    if (noeud.type == SSS) {
        for (const ExigenceSRS& s : srsAnalyses) {
            const auto& trac = s.getTracabilite();
            if (std::find(trac.begin(), trac.end(), noeud.id) != trac.end())
                liens << QString::fromStdString(s.getId());
        }
        labelDetailLiens->setText(liens.isEmpty() ? "Aucun lien SRS" :
                                      "🔗 " + QString::number(liens.size()) + " lien(s) SRS : " + liens.join(", "));
    } else if (noeud.type == SRS) {
        for (const ExigenceSDD& d : sddAnalyses) {
            const auto& ref = d.getReferencedSRS();
            if (std::find(ref.begin(), ref.end(), noeud.id) != ref.end())
                liens << QString::fromStdString(d.getId());
        }
        labelDetailLiens->setText(liens.isEmpty() ? "Aucun lien SDD" :
                                      "🔗 " + QString::number(liens.size()) + " lien(s) SDD : " + liens.join(", "));
    } else {
        for (const ExigenceSDD& d : sddAnalyses) {
            if (d.getId() == noeud.id) {
                for (const std::string& ref : d.getReferencedSRS())
                    liens << QString::fromStdString(ref);
                break;
            }
        }
        labelDetailLiens->setText(liens.isEmpty() ? "Aucun lien SRS" :
                                      "🔗 Couvre : " + liens.join(", "));
    }
}

/**
 * @brief Retourne le nœud à la position donnée, nullptr si aucun.
 * @param pos Position du clic en coordonnées du graphe.
 * @return Pointeur vers le nœud ou nullptr.
 */
const NoeudGraphe* VueGrapheTracabilite::noeudAPosition(const QPointF& pos) const
{
    for (const NoeudGraphe& n : noeuds) {
        if (!noeudVisible(n)) continue;
        double dx = pos.x() - n.position.x();
        double dy = pos.y() - n.position.y();
        if (std::abs(dx) <= 100 && std::abs(dy) <= 25)
            return &n;
    }
    return nullptr;
}

/**
 * @brief Applique un filtre sur les nœuds et arêtes du graphe.
 * @param filtre Le filtre à appliquer.
 */
void VueGrapheTracabilite::appliquerFiltre(const FiltreGUI& filtre)
{
    filtreActif = filtre;
    update();
}
