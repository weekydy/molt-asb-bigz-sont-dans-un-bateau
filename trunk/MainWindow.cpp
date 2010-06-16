#include "MainWindow.h"


MainWindow::MainWindow()
{
    setWindowIcon(QPixmap(":trophe.png"));

    user_id = -1;
    access = OFFLINE;

    setWindowTitle("Gestionnaire d'emploi du temps");
    setMinimumSize(800, 600);

    QDesktopWidget desktop;
    QRect dim = desktop.screenGeometry();
    int x = dim.width()/2 - width()/2;
    int y = dim.height()/2 - height()/2;
    move(x,y);

    createActions();
    createMenus();
    createStatusBar();
    connectionDB();
    createWidgets();
    createSignalsSlots();
}


MainWindow::~MainWindow(){
    DbAccess::getInstance()->kill();
}

void MainWindow::createActions(){
    action_quit = new QAction("&Quitter", this);
    action_quit->setStatusTip("Quitter l'application");

    action_about = new QAction("&A Propos", this);
}

void MainWindow::createMenus(){
    menu_file = menuBar()->addMenu("&Fichier");
    menu_file->addAction(action_quit);

    menu_help = menuBar()->addMenu("&?");
    menu_help->addAction(action_about);
}

void MainWindow::createStatusBar(){
    myStatusBar = statusBar();
}

void MainWindow::connectionDB(){
    if(DbAccess::getInstance()->openDB()){
        myStatusBar->showMessage("Connexion à la BDD réussie !", 2000);
    }
    else{
        myStatusBar->showMessage("Connexion à la BDD échouée !");
        QMessageBox::warning(this, "Erreur !", "La connexion à la BDD a échoué... L'application va se fermer.");
        exit(0);
    }
}

void MainWindow::createWidgets(){
    if(access > OFFLINE){
        QSqlQuery req;
        req.prepare("SELECT user_name, user_surname FROM user WHERE user_id = :user_id");
        req.bindValue(":user_id", user_id);
        req.exec();
        req.first();

        QLabel *label_welcome = new QLabel;
        label_welcome->setText("Bonjour "+req.value(0).toString()+" "+req.value(1).toString()+" !");

        QPushButton *button_logout = new QPushButton("Déconnexion");
        QPushButton *button_delele = new QPushButton("Supprimer mon compte");

        QHBoxLayout *layout_top = new QHBoxLayout();
        layout_top->addWidget(label_welcome);
        layout_top->addStretch();
        layout_top->addWidget(button_delele);
        layout_top->addWidget(button_logout);

        QTabWidget *tabWidget = new QTabWidget(this);
        tabWidget->setMovable(true);

        TabItemNews *tabItemNews = new TabItemNews(user_id);
        TabItemPlanning *tabItemPlanning = new TabItemPlanning(user_id);

        tabWidget->addTab(tabItemNews, "Mes notifications");
        tabWidget->addTab(tabItemPlanning, "Mon planning");
        tabWidget->addTab(new TabItemProfile(user_id), "Mon profil");

        connect(tabItemNews, SIGNAL(notifyRefreshList()), tabItemPlanning, SLOT(refreshList()));

        if(access == ONLINE_USER){
            myStatusBar->showMessage("Connecté");

        }
        else if(access == ONLINE_ROOT){
            myStatusBar->showMessage("Connecté (Admin)");
            tabWidget->addTab(new TabItemUser(user_id), "Utilisateurs");
            tabWidget->addTab(new TabItemGroup(), "Groupes");
            tabWidget->addTab(new TabItemRoom(), "Salles");
            tabWidget->addTab(new TabItemEquipment(), "Equipements");
            tabWidget->addTab(new TabItemTools(), "Outils d'administration");
        }

        QVBoxLayout *layout_central = new QVBoxLayout();
        layout_central->addLayout(layout_top);
        layout_central->addWidget(tabWidget);

        QWidget *widget_central = new QWidget(this);
        widget_central->setLayout(layout_central);

        connect(button_logout, SIGNAL(clicked()), this, SLOT(disconnected()));
        connect(button_delele, SIGNAL(clicked()), this, SLOT(deleteAccount()));

        setCentralWidget(widget_central);

    }
    else{ // status == OFFLINE
        myStatusBar->showMessage("Non connecté");

        LoginWidget *loginScreen = new LoginWidget(this);

        QHBoxLayout *layout_line = new QHBoxLayout();
        layout_line->addStretch();
        layout_line->addWidget(loginScreen);
        layout_line->addStretch();

        QVBoxLayout *layout_central = new QVBoxLayout();
        layout_central->addStretch();
        layout_central->addLayout(layout_line);
        layout_central->addStretch();

        QWidget *widget_central = new QWidget(this);
        widget_central->setLayout(layout_central);

        setCentralWidget(widget_central);
    }
}

void MainWindow::about(){
    QMessageBox::about(this, "A propos...", "<b>Major Agenda©</b>, du moins ça serait bien. <br/><br/> Version Beta 0.76 (32bits)<br/> Copyright 2010 Dream Team UTBM. Tous droits réservés.<br/><br/>Développé à l'aide de Qt 4.6.2, SQLite 3.2 et compilé par MinGW gcc 4.4. <br/><br/> Développement par : Adrien GAVIGNET / Bastien CRAMILLET / Jérémy MALTIS <br/><br/> Projet encouragé par Google Agenda (papa comme on l'appel). <br/><br/> Nous tenons à remercier nos parents, sans eux, tout ce talent ne serait pas ici aujourd'hui, ainsi que nos fans, qui se battent désormais par millier pour avoir une version dédicacée de cette application.");
}

void MainWindow::updateConnected(int _user_id, int _access){
    access = _access;
    user_id = _user_id;

    createWidgets();
}

void MainWindow::disconnected(){
    user_id = -1;
    access = OFFLINE;

    createWidgets();
}

void MainWindow::deleteAccount(){
    QSqlQuery *req = new QSqlQuery();
    req->prepare("DELETE FROM user WHERE user_id = :user_id");
    req->bindValue(":user_id", user_id);

    int rep = QMessageBox::question(this, "Confirmation", "Etes-vous sûr de vouloir supprimer votre compte ?", QMessageBox::Yes | QMessageBox::No);
    if(rep == QMessageBox::Yes){
        if (req->exec()){
            user_id = -1;
            access = OFFLINE;

            createWidgets();
            QMessageBox::information(this, "Requête exécutée avec succès !", "Votre compte a été supprimé, vous êtes déconnecté !");
        }
        else
            QMessageBox::warning(this, "Erreur !", "La requête n'a pas pu être exécutée !");
    }
}



void MainWindow::createSignalsSlots(){
    connect(action_quit, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(action_about, SIGNAL(triggered()), this, SLOT(about()));
}

void MainWindow::closeEvent(QCloseEvent* event) {
    int rep = QMessageBox::information(this, "Confirmation", "Etes-vous sûr de vouloir quitter l'application ?", QMessageBox::Yes | QMessageBox::No);
    if (rep == QMessageBox::Yes){
        event->accept();
    }
    else{
        event->ignore();
    }
}


