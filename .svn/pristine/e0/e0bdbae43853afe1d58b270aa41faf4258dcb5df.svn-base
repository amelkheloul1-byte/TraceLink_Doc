#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>

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

private:
    // Onglets
    QTabWidget *tabWidget;
    QWidget    *ongletImport;
    QWidget    *ongletGraphe;
    QWidget    *ongletRapport;

    // Onglet Import - champs fichiers
    QLineEdit  *champSSS;
    QLineEdit  *champSRS;
    QLineEdit  *champSDD;

    QPushButton *btnParcourirSSS;
    QPushButton *btnParcourirSRS;
    QPushButton *btnParcourirSDD;
    QPushButton *btnLancer;

    // Chemins des fichiers sélectionnés
    QString cheminSSS;
    QString cheminSRS;
    QString cheminSDD;

    // Méthodes de construction de l'UI
    void construireOngletImport();
    void construireOngletGraphe();
    void construireOngletRapport();
};

#endif // MAINWINDOW_H
