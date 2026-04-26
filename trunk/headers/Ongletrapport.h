#ifndef ONGLETRAPPORT_H
#define ONGLETRAPPORT_H

/**
 * @file OngletRapport.h
 * @brief Déclaration de la classe OngletRapport.
 * @author Lamia Arrahmane
 * @date 02/04/2026
 * @version 1.0
 */

#include <QWidget>
#include <QTableWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFrame>
#include <QProgressBar>
#include <QTabWidget>
/// @brief AJOUT : menu déroulant pour l'export
#include <QMenu>
#include <QToolButton>
#include "RapportTracabilite.h"
/// @brief AJOUT : include du gestionnaire de filtres
#include "GestionnaireFiltresGUI.h"
/// @brief AJOUT : includes pour les vraies matrices
#include "ExigenceSRS.h"
#include "ExigenceSDD.h"

/**
 * @class OngletRapport
 * @brief Onglet Qt affichant le rapport de traçabilité complet produit par le Module 2.
 *
 * Affiche :
 * - Les taux de couverture avec barres de progression bleues
 * - Badges résumé (couverts, non couverts, liens brisés)
 * - Matrice SSS×SRS et matrice SRS×SDD avec zoom/dézoom
 * - Panneau détail d'une exigence au clic
 * - Menu déroulant export (CSV, HTML)
 * - Panneau de filtres déroulant à droite
 */
class OngletRapport : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructeur.
     * @param parent Widget parent Qt.
     */
    explicit OngletRapport(QWidget *parent = nullptr);

    /**
     * @brief Charge et affiche un rapport de traçabilité sans données SRS/SDD détaillées.
     * @param rapport Le rapport produit par le MoteurTracabilite.
     */
    void afficherRapport(const RapportTracabilite& rapport);

    /**
     * @brief AJOUT : Charge et affiche le rapport avec les vraies matrices bidimensionnelles.
     * @param rapport Le rapport produit par le MoteurTracabilite.
     * @param srs     Vecteur des exigences SRS analysées.
     * @param sdd     Vecteur des exigences SDD analysées.
     */
    void afficherRapportComplet(const RapportTracabilite& rapport,
                                const std::vector<ExigenceSRS>& srs,
                                const std::vector<ExigenceSDD>& sdd);

private slots:
    /**
     * @brief Exporte le rapport au format CSV.
     */
    void exporterCSV();

    /**
     * @brief Exporte le rapport au format HTML.
     */
    void exporterHTML();

    /**
     * @brief AJOUT : Affiche ou masque le panneau de filtres latéral.
     */
    void toggleFiltres();

public slots:
    /**
     * @brief AJOUT : Applique un filtre sur les matrices.
     * @param filtre Le filtre à appliquer.
     */
    void appliquerFiltre(const FiltreGUI& filtre);

private:
    // ── Taux de couverture ────────────────────────────────────────────────────
    QLabel       *labelTauxSSS_SRS; ///< Label du taux SSS→SRS.
    QLabel       *labelTauxSRS_SDD; ///< Label du taux SRS→SDD.
    QLabel       *labelTauxGlobal;  ///< Label du taux global.
    QProgressBar *barreSSS_SRS;     ///< AJOUT : barre de progression SSS→SRS (bleue).
    QProgressBar *barreSRS_SDD;     ///< AJOUT : barre de progression SRS→SDD (bleue).
    QProgressBar *barreGlobal;      ///< AJOUT : barre de progression globale (bleue).

    // ── Badges résumé ─────────────────────────────────────────────────────────
    QLabel *labelBadgeCouverts;    ///< AJOUT : badge nombre d'exigences couvertes.
    QLabel *labelBadgeNonCouverts; ///< AJOUT : badge nombre d'exigences non couvertes.
    QLabel *labelBadgeLiensBrises; ///< AJOUT : badge nombre de liens brisés.

    // ── Matrices de traçabilité ───────────────────────────────────────────────
    QTableWidget *tableMatriceSSS_SRS; ///< AJOUT : Matrice SSS (lignes) × SRS (colonnes).
    QTableWidget *tableMatriceSRS_SDD; ///< AJOUT : Matrice SRS (lignes) × SDD (colonnes).

    // ── Panneau détail exigence sélectionnée ──────────────────────────────────
    QFrame  *panneauDetail;      ///< AJOUT : panneau affichant le détail d'une exigence.
    QLabel  *labelDetailId;      ///< AJOUT : identifiant de l'exigence sélectionnée.
    QLabel  *labelDetailType;    ///< AJOUT : type (SSS/SRS/SDD) de l'exigence.
    QLabel  *labelDetailStatut;  ///< AJOUT : statut de l'exigence sélectionnée.
    QLabel  *labelDetailLiens;   ///< AJOUT : liens associés à l'exigence sélectionnée.

    // ── Export ────────────────────────────────────────────────────────────────
    QToolButton *boutonExport; ///< AJOUT : bouton export avec menu déroulant.
    QMenu       *menuExport;   ///< AJOUT : menu déroulant avec les formats d'export.

    // ── Filtres ───────────────────────────────────────────────────────────────
    QFrame                 *panneauFiltres;     ///< AJOUT : panneau de filtres déroulant.
    QPushButton            *boutonFiltres;      ///< AJOUT : bouton ouvrir/fermer les filtres.
    GestionnaireFiltresGUI *gestionnaireFiltres; ///< AJOUT : gestionnaire de filtres intégré.

    // ── Données ───────────────────────────────────────────────────────────────
    RapportTracabilite       rapportCourant; ///< Rapport courant pour l'export.
    std::vector<ExigenceSRS> srsAnalyses;   ///< AJOUT : exigences SRS pour les matrices.
    std::vector<ExigenceSDD> sddAnalyses;   ///< AJOUT : exigences SDD pour les matrices.

    //Liens brisés
    QFrame      *panneauLiensBrises;
    QWidget     *listeLiensBrises;
    QVBoxLayout *layoutListeLB;

    /**
     * @brief Initialise la mise en page et tous les widgets de l'onglet.
     */
    void initialiserUI();

    /**
     * @brief AJOUT : Crée une barre de progression stylisée.
     * @param couleur Couleur de la barre en format hex.
     * @return La barre de progression créée.
     */
    QProgressBar* creerBarre(const QString& couleur);

    /**
     * @brief AJOUT : Applique un style cohérent à un QTableWidget.
     * @param table Le tableau à styliser.
     */
    void styliserTableau(QTableWidget* table);
};

#endif // ONGLETRAPPORT_H
