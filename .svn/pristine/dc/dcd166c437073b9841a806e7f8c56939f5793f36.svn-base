#ifndef GESTIONNAIREFILTRESGUI_H
#define GESTIONNAIREFILTRESGUI_H

/**
 * @file GestionnaireFiltresGUI.h
 * @brief Déclaration de la classe GestionnaireFiltresGUI.
 * @author Lamia Arrahmane
 * @date 02/04/2026
 * @version 1.0
 */

#include <QWidget>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include "Exigence.h"

/**
 * @enum TypeDoc
 * @brief Filtre par niveau de document.
 */
enum TypeDoc {
    TOUS_DOCS, ///< Affiche tous les documents.
    DOC_SSS,   ///< Affiche uniquement les SSS.
    DOC_SRS,   ///< Affiche uniquement les SRS.
    DOC_SDD    ///< Affiche uniquement les SDD.
};

/**
 * @struct FiltreGUI
 * @brief Combinaison des trois filtres actifs.
 */
struct FiltreGUI {
    TypeDoc filtreDocument;    ///< Filtre par niveau de document.
    Statut  filtreStatut;      ///< Filtre par statut.
    QString filtreIdentifiant; ///< Filtre par identifiant (texte libre).

    /// @brief Constructeur par défaut — aucun filtre actif.
    FiltreGUI() : filtreDocument(TOUS_DOCS),
        filtreStatut(NonAnalyse),
        filtreIdentifiant("") {}
};

/**
 * @class GestionnaireFiltresGUI
 * @brief Widget Qt gérant les trois filtres d'affichage.
 *
 * Design vertical avec titre, menus déroulants, champ recherche,
 * bouton appliquer et légende colorée.
 */
class GestionnaireFiltresGUI : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructeur.
     * @param parent Widget parent Qt.
     */
    explicit GestionnaireFiltresGUI(QWidget *parent = nullptr);

    /**
     * @brief Retourne la combinaison des trois filtres actifs.
     * @return FiltreGUI contenant les trois filtres courants.
     */
    FiltreGUI getFiltreActuel() const;

    /**
     * @brief Remet les trois filtres à leur valeur par défaut.
     */
    void reinitialiser();

signals:
    /**
     * @brief Émis quand un filtre est appliqué par l'utilisateur.
     * @param filtre Le nouveau filtre actif.
     */
    void filtreModifie(const FiltreGUI& filtre);

public slots:
    /**
     * @brief Déclenché lors du choix d'un niveau de document.
     * @param index Index du QComboBox sélectionné.
     */
    void setFiltreDocument(int index);

    /**
     * @brief Déclenché lors du choix d'un statut.
     * @param index Index du QComboBox sélectionné.
     */
    void setFiltreStatut(int index);

    /**
     * @brief Déclenché à chaque frappe dans le champ de recherche.
     * @param texte Texte saisi par l'utilisateur.
     */
    void setFiltreIdentifiant(const QString& texte);

private slots:
    /**
     * @brief AJOUT : Applique les filtres au clic du bouton.
     */
    void appliquerFiltres();

private:
    QComboBox   *comboDocument;    ///< Menu déroulant filtre par document.
    QComboBox   *comboStatut;      ///< Menu déroulant filtre par statut.
    QLineEdit   *champIdentifiant; ///< Champ de recherche par identifiant.
    QPushButton *btnAppliquer;     ///< AJOUT : bouton appliquer les filtres.
    QPushButton *btnReinitialiser; ///< Bouton réinitialiser les filtres.

    FiltreGUI filtreCourant; ///< Filtre actuellement actif.

    /**
     * @brief Initialise la mise en page et les widgets.
     */
    void initialiserUI();

    /**
     * @brief AJOUT : Crée un label de section stylisé.
     * @param texte Le texte du label.
     * @return Le QLabel créé.
     */
    QLabel* creerLabel(const QString& texte);
};

#endif // GESTIONNAIREFILTRESGUI_H
