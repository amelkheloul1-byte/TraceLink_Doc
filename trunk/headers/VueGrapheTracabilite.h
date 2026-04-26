#ifndef VUEGRAPHETRACABILITE_H
#define VUEGRAPHETRACABILITE_H

#include <QWidget>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QFrame>
#include <QPushButton>
#include <QLabel>
#include <vector>
#include <string>
#include "RapportTracabilite.h"
#include "ExigenceSRS.h"
#include "ExigenceSDD.h"
#include "GestionnaireFiltresGUI.h"

struct NoeudGraphe {
    std::string id;
    TypeExigence type;
    Statut statut;
    QPointF position;
};

struct AreteGraphe {
    std::string idSource;
    std::string idCible;
    bool estBrise;
    double pourcentage; // Pourcentage de couverture affiché sur l'arête.
};

class VueGrapheTracabilite : public QWidget
{
    Q_OBJECT

public:
    explicit VueGrapheTracabilite(QWidget *parent = nullptr);

    void construireGraphe(const RapportTracabilite& rapport,
                          const std::vector<ExigenceSRS>& srs,
                          const std::vector<ExigenceSDD>& sdd);

public slots:
    void appliquerFiltre(const FiltreGUI& filtre);
    void toggleFiltres();

protected:
    void paintEvent(QPaintEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    std::vector<NoeudGraphe> noeuds;
    std::vector<AreteGraphe> aretes;
    std::vector<ExigenceSRS> srsAnalyses;
    std::vector<ExigenceSDD> sddAnalyses;
    RapportTracabilite rapportCourant;
    double niveauZoom;
    std::string noeudSelectionne;
    FiltreGUI filtreActif;

    // Panneau détail
    QFrame      *panneauDetail;
    QLabel      *labelDetailId;
    QLabel      *labelDetailType;
    QLabel      *labelDetailStatut;
    QLabel      *labelDetailLiens;

    // Filtres
    QFrame                 *panneauFiltres;
    QPushButton            *boutonFiltres;
    GestionnaireFiltresGUI *gestionnaireFiltres;

    void dessinerNoeud(QPainter& painter, const NoeudGraphe& noeud);
    void dessinerArete(QPainter& painter, const AreteGraphe& arete);
    void afficherDetailNoeud(const NoeudGraphe& noeud);
    QColor couleurStatut(Statut statut) const;
    const NoeudGraphe* noeudAPosition(const QPointF& pos) const;
    bool noeudVisible(const NoeudGraphe& n) const;
};

#endif // VUEGRAPHETRACABILITE_H
