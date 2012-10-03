#include "Navigateur.h"



/// [CONSTRUCTEURS] %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
/// ---------------------------------------------------------------------------

Navigateur::Navigateur():
    m_PointeurChargerRafraichir(new QToolButton(this)),
    m_barAddress    (new QLineEdit(this)),
    m_barRecherche  (new QLineEdit(this)),
    m_tabWidget     (new QTabWidget(this->centralWidget())),
    m_currWebView   (new QWebView(this->centralWidget())),
    m_pageAccueil   ("http://www.globo.com"),
    m_rechercheDefault("Google")
{
    // 1. creer Fenetre Principale
    this->resize(800, 600);
    this->setWindowTitle("zNavigo");
    this->setWindowIcon(QIcon("images/icon-browser.png"));
    this->setAnimated(true);

    creerMenu();        // 2. creer menus
    creerActions();     // 3. creer actions
    creerToolBars();    // 4. creer barre d'outils
    creerWebView();     // 5. creer tabBar avec webView
    creerStatusBar();   // 6. creer barre de status

    // 7. Aller a la page d'acueil
    this->aller(m_pageAccueil);

    /// [CONNECTIONS] %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

    // Faire la connection de la barre d'address pour changer la page
    QObject::connect(m_barAddress, SIGNAL(returnPressed()), this, SLOT(allerAPageAddress()));
    QObject::connect(m_currWebView, SIGNAL(loadProgress(int)), m_statusProgress, SLOT(setValue(int)));
    QObject::connect(m_currWebView, SIGNAL(loadStarted()), this, SLOT(debutChargement()));
    QObject::connect(m_currWebView, SIGNAL(loadFinished(bool)), this, SLOT(finChargement(bool)));

    // [MENU FICHIER]__________________________________________________________
    // Faire la connection pour ouvrir un fichier html
    QObject::connect(m_actionOuvrir, SIGNAL(triggered()), this, SLOT(ouvrirHtml()));
    // Faire la connection pour ouvrir un lien
    QObject::connect(m_actionOuvrirLien, SIGNAL(triggered()), this, SLOT(ouvrirLien()));
    // Faire la connection pour sauvegarder page
    QObject::connect(m_actionSauvegarder, SIGNAL(triggered()), this, SLOT(sauvegarderPage()));
    // Faire la connection pour sauvegarder page sous
    QObject::connect(m_actionSauvegarderSous, SIGNAL(triggered()), this, SLOT(sauvegarderPageSous()));
    // Faire la connection pour imprimer page
    QObject::connect(m_actionImprimer, SIGNAL(triggered()), this, SLOT(imprimerPage()));
    // Faire la connection pour Quitter l'app
    QObject::connect(m_actionQuitter, SIGNAL(triggered()), qApp, SLOT(quit()));
    // ________________________________________________________________________


    // [MENU EDITION]__________________________________________________________
    QObject::connect(m_actionRechercher, SIGNAL(triggered()), this, SLOT(rechercher()));
    // ________________________________________________________________________


    // [MENU AFFICHAGE]________________________________________________________
    QObject::connect(m_actionArreterChargement, SIGNAL(triggered()), m_currWebView, SLOT(stop()));
    QObject::connect(m_actionRafraichir, SIGNAL(triggered()), m_currWebView, SLOT(reload()));
    // ________________________________________________________________________


    // [MENU HISTORIQUE]________________________________________________________
    QObject::connect(m_actionArriere, SIGNAL(triggered()), m_currWebView, SLOT(back()));
    QObject::connect(m_actionEnAvant, SIGNAL(triggered()), m_currWebView, SLOT(forward()));
    // ________________________________________________________________________


    /// [END CONNECTIONS] %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


    // 9. keep looping
}



/// [FUNCTIONS DE CREATIONS] %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
/// ---------------------------------------------------------------------------
/**
 * @brief Fonction pour creer les emplacements du menu
*/
void Navigateur::creerMenu()
{
    m_menuFichier = menuBar()->addMenu(tr("&Fichier"));
    m_menuEdition = menuBar()->addMenu(tr("&Edition"));
    m_menuAffichage = menuBar()->addMenu(tr("&Affichage"));
    m_menuHistorique = menuBar()->addMenu(tr("&Historique"));
    m_menuOutils = menuBar()->addMenu(tr("&Outils"));
    m_menuAide = menuBar()->addMenu(tr("&Aide"));
}


/**
 * @brief Fonction pour creer les actions
*/
void Navigateur::creerActions()
{
    // Menu fichier --------------------------------------------------
    m_actionNouvelFenetre = m_menuFichier->addAction(tr("&Nouvel fenetre"));
    m_actionNouvelFenetre->setShortcut(QKeySequence("Ctrl+N"));
    m_actionNouvelFenetre->setIcon(QIcon("images/window-new.png"));

    m_actionNouvelOnglet = m_menuFichier->addAction(tr("&Nouvel onglet"));
    m_actionNouvelOnglet->setShortcut(QKeySequence("Ctrl+shift+N"));
    m_actionNouvelOnglet->setIcon(QIcon("images/tab-new.png"));

    m_actionOuvrir = m_menuFichier->addAction(tr("&Ouvrir"));
    m_actionOuvrir->setShortcut(QKeySequence("Ctrl+O"));
    m_actionOuvrir->setIcon(QIcon("images/document-open.png"));

    m_actionOuvrirLien = m_menuFichier->addAction(tr("&Ouvrir lien"));
    m_actionOuvrirLien->setShortcut(QKeySequence("Ctrl+Shift+O"));

    m_actionSauvegarder = m_menuFichier->addAction(tr("&Sauvegarder"));
    m_actionSauvegarder->setShortcut(QKeySequence("Ctrl+S"));
    m_actionSauvegarder->setIcon(QIcon("images/document-save.png"));

    m_actionSauvegarderSous = m_menuFichier->addAction(tr("Sauvegarder sous ..."));
    m_actionSauvegarderSous->setShortcut(QKeySequence("Ctrl+shift+S"));
    m_actionSauvegarderSous->setIcon(QIcon("images/document-save-as.png"));

    m_actionImprimer = m_menuFichier->addAction(tr("Imprimer"));
    m_actionImprimer->setShortcut(QKeySequence("Ctrl+P"));
    m_actionImprimer->setIcon(QIcon("images/document-print.png"));

    m_actionQuitter = m_menuFichier->addAction(tr("&Quitter"));
    m_actionQuitter->setShortcut(QKeySequence("Ctrl+Q"));
    m_actionQuitter->setIcon(QIcon("images/quitter.png"));

    // Menu edition --------------------------------------------------
    m_actionRechercher = m_menuEdition->addAction(tr("Rechercher..."));
    m_actionRechercher->setShortcut(QKeySequence("Ctrl+F"));
    m_actionRechercher->setIcon(QIcon("images/edit-find.png"));

    // Menu affichage ------------------------------------------------
    m_actionArreterChargement = m_menuAffichage->addAction(tr("Arreter"));
    m_actionArreterChargement->setShortcut(QKeySequence("ESC"));
    m_actionArreterChargement->setIcon(QIcon("images/stop.png"));

    m_actionRafraichir = m_menuAffichage->addAction(tr("Rafraichir la page"));
    m_actionRafraichir->setShortcut(QKeySequence("F5"));
    m_actionRafraichir->setIcon(QIcon("images/refresh.png"));

    m_actionPleinEcran = m_menuAffichage->addAction(tr("Plein ecran"));
    m_actionPleinEcran->setShortcut(QKeySequence("F11"));

    /// @test -----------------------------------------------------
    //m_PointeurChargerRafraichir = m_actionArreterChargement;
    /// -----------------------------------------------------------

    // Menu historique -----------------------------------------------
    m_actionPageInitial = m_menuHistorique->addAction(tr("Page initial"));
    m_actionPageInitial->setIcon(QIcon("images/go-home.png"));

    m_actionArriere = m_menuHistorique->addAction(tr("Arriere"));
    m_actionArriere->setShortcut(QKeySequence("Alt+Left"));
    m_actionArriere->setIcon(QIcon("images/go-previous.png"));

    m_actionEnAvant = m_menuHistorique->addAction(tr("En avant"));
    m_actionEnAvant->setShortcut(QKeySequence("Alt+Right"));
    m_actionEnAvant->setIcon(QIcon("images/go-next.png"));

    // Menu outils ---------------------------------------------------
    m_actionPreferences = m_menuOutils->addAction(tr("&Preferences"));
    m_actionPreferences->setShortcut(QKeySequence("Ctrl+P"));
    m_actionPreferences->setIcon(QIcon("images/preferences.png"));

    // Menu aide -----------------------------------------------------
    m_actionAbout = m_menuAide->addAction(tr("A propos"));
}


/**
 * @brief Fonction pour creer les barres d'outils
*/
void Navigateur::creerToolBars()
{
    m_toolBarActions = this->addToolBar(tr("Barre d'actions"));
    m_toolBarActions->addAction(m_actionArriere);
    m_toolBarActions->addAction(m_actionEnAvant);
    m_toolBarActions->addWidget(m_PointeurChargerRafraichir);
    m_toolBarActions->addAction(m_actionPageInitial);
    m_toolBarActions->addSeparator();
    m_toolBarActions->addWidget(m_barAddress);
    m_toolBarActions->addSeparator();

    m_barAddress->setText(m_rechercheDefault);
    m_toolBarActions->addWidget(m_barAddress);

}


/**
 * @brief Fonction pour creer le view des pages web
*/
void Navigateur::creerWebView()
{
    m_tabWidget->setTabsClosable(true);
    this->setCentralWidget(m_tabWidget);
    m_currWebView = ajouterOnglet();    // pour faire le webView current
}


/**
 * @brief Fonction pour creer la barre de status
*/
void Navigateur::creerStatusBar()
{
    m_statusProgress = new QProgressBar(this);
    m_statusProgress->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    this->statusBar()->addWidget(m_statusProgress);

}



/// [FUNCTIONS D'ACTIONS] %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
/// ---------------------------------------------------------------------------

/**
 * @brief Load page specified by param lien into the current tab
 * @param lien
 */
void Navigateur::aller(const QString& lien, QWebView* tempWebView)
{
    if(tempWebView == ZERO){
        tempWebView = m_currWebView;
    }

    // Parse string to fill missing informations ex : missing "http://" or missing "www"
    QString str = lien;
    if(! str.contains("http://")){
        str = QString("http://")+str;
    }
    tempWebView->load(QUrl(str));
    tempWebView->show();

    // Change le titre de l'onglet aussi que l'icon, quand la page est fini de charger,
    QObject::connect(m_currWebView, SIGNAL(loadFinished(bool)), this, SLOT(finChargement(bool)));
    QObject::connect(m_currWebView, SIGNAL(urlChanged(QUrl)), this, SLOT(updateUrl(QUrl)));
}




/// [SLOTS] %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
/// ---------------------------------------------------------------------------
/**
 * @brief Ajoute un nouvel onglet
 * @return pointeur de QWebView dans le nouvel onglet
 */
QWebView* Navigateur::ajouterOnglet()
{
    QWebView* nouveauWebView = new QWebView(m_tabWidget);
    m_tabWidget->addTab(nouveauWebView, tr("Nouvel onglet"));
    return nouveauWebView;
}


/**
 * @brief Navigateur::allerAPage
 */
void Navigateur::allerAPageAddress()
{
    this->aller(m_barAddress->text());
}

/**
 * @brief Navigateur::changerPage
 */
void Navigateur::changerPage(int)
{
    m_currWebView = dynamic_cast<QWebView*>( m_tabWidget->currentWidget() );
}

/**
 * @brief Navigateur::fermerOnglet
 */
void Navigateur::fermerOnglet(int)
{
    if( m_tabWidget->count() == 1 ){
        qApp->quit();
    }
}

void Navigateur::debutChargement()
{
    m_PointeurChargerRafraichir->setDefaultAction(m_actionArreterChargement);
}

/**
 * @brief Navigateur::personaliserTab
 */
void Navigateur::finChargement(bool)
{
    // 1. Change l'icon de l'onglet
    m_tabWidget->setTabIcon(m_tabWidget->indexOf(m_currWebView), m_currWebView->icon());
    // 2. Change le titre de l'onglet
    m_tabWidget->setTabText(m_tabWidget->indexOf(m_currWebView), m_currWebView->title());
    // 3. Change l'address à la barre d'adress
    m_barAddress->setText(m_currWebView->url().toString());
    // 4. Metre à jour l'action arret/rafraichir en mode rafraichir
    m_PointeurChargerRafraichir->setDefaultAction(m_actionRafraichir);
}

/**
 * @brief Navigateur::updateUrl
 */
void Navigateur::updateUrl(QUrl nouvelUrl)
{
    m_barAddress->setText( nouvelUrl.toString() );
}


/**
 * @brief Navigateur::ouvrirHtml
 */
void Navigateur::ouvrirHtml()
{
    QString fichier = QFileDialog::getOpenFileName(this, tr("Ouvrir un fichier html"), ZERO,
                                                   "Pages web (*.htm *.html *.xml)");
    m_currWebView->setUrl(fichier);
}

/**
 * @brief Navigateur::ouvrirLien
 */
void Navigateur::ouvrirLien()
{
    QString lien = QInputDialog::getText(this, "Ouvrir une url", "Url : ", QLineEdit::Normal, "http://");
    aller(lien);
}

/**
 * @brief Navigateur::sauvegarderPage
 */
void Navigateur::sauvegarderPage()
{
}

/**
 * @brief Navigateur::sauvegarderPageSous
 */
void Navigateur::sauvegarderPageSous()
{
}

/**
 * @brief Navigateur::imprimerPage
 */
void Navigateur::imprimerPage()
{
}

/**
 * @brief Navigateur::rechercher
 */
void Navigateur::rechercher()
{

}

void Navigateur::rafraichir()
{
    m_currWebView->reload();
}

void Navigateur::arreterChargement()
{
    m_currWebView->stop();
}


