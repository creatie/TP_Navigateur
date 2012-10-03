/*
    CONVENTIONS:
    private functions are written in french ex : void creerMenu();
    menu variables begins with "menu" ex : QMenu* m_menuFichier;
    action variables begins with "action" ex : QAction* m_actionOuvrir;
*/

#ifndef NAVIGATEUR_H
#define NAVIGATEUR_H

#include <QtGui>
#include <QWebView>

class Navigateur : public QMainWindow
{
    Q_OBJECT
public:
    Navigateur();

signals:

public slots:
    QWebView* ajouterOnglet();
    void allerAPageAddress(); // Appelé quand l'utilisateur appuye sur retours
    void changerPage(int);
    void fermerOnglet(int);
    void debutChargement();
    void finChargement(bool);
    void updateUrl(QUrl);

    void ouvrirHtml();
    void ouvrirLien();
    void sauvegarderPage();
    void sauvegarderPageSous();
    void imprimerPage();

    void rechercher();

    void rafraichir();
    void arreterChargement();

private:
    void creerMenu();
    void creerActions();
    void creerToolBars();
    void creerWebView();
    void creerStatusBar();

    // Helper functions
    void aller(const QString& lien, QWebView* tempWebView=ZERO);
    //void changerOnglet();


    static const int ZERO = 0 ;   // Represents NULL


    // Menu ----------------
    QMenu* m_menuFichier;
    QMenu* m_menuEdition;
    QMenu* m_menuAffichage;
    QMenu* m_menuHistorique;
    QMenu* m_menuOutils;
    QMenu* m_menuAide;

    // Actions -------------
    QToolButton* m_PointeurChargerRafraichir; // Pointe l'action Rafraichir si la page a fini de charger

    QAction* m_actionNouvelOnglet;
    QAction* m_actionNouvelFenetre;
    QAction* m_actionOuvrir;
    QAction* m_actionOuvrirLien;
    QAction* m_actionSauvegarder;
    QAction* m_actionSauvegarderSous;
    QAction* m_actionImprimer;
    QAction* m_actionQuitter;

    QAction* m_actionRechercher;

    QAction* m_actionArreterChargement;
    QAction* m_actionRafraichir;
    QAction* m_actionPleinEcran;

    QAction* m_actionPageInitial;
    QAction* m_actionArriere;
    QAction* m_actionEnAvant;

    QAction* m_actionPreferences;

    QAction* m_actionAbout;

    // Toolbars ------------
    QToolBar* m_toolBarActions;
    QLineEdit* m_barAddress;
    QLineEdit* m_barRecherche;
    QToolBar* m_toolBarOptions;

    // WebView -------------
    QTabWidget* m_tabWidget;
    QWebView* m_currWebView;

    // Statusbar -----------
    QProgressBar* m_statusProgress;

    // Configurations
    QString m_pageAccueil;
    QString m_rechercheDefault;
};

#endif // NAVIGATEUR_H
