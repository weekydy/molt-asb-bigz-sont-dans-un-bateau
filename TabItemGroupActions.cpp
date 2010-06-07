#include "TabItemGroupActions.h"

TabItemGroupActions::TabItemGroupActions(QWidget *parent) : QDialog(parent)
{
    le_group_name = new QLineEdit();

    QFormLayout *fl_data = new QFormLayout;
    fl_data->addRow("Nom:", le_group_name);

    btn_cancel = new QPushButton("&Annuler");
    btn_action = new QPushButton();


    setWindowTitle("Créer un groupe");
    btn_action->setText("&Ajouter !");


    QHBoxLayout *layout_buttons = new QHBoxLayout;
    layout_buttons->setAlignment(Qt::AlignRight);
    layout_buttons->addWidget(btn_action);
    layout_buttons->addWidget(btn_cancel);

    QVBoxLayout *layout_main = new QVBoxLayout;
    layout_main->addLayout(fl_data);
    layout_main->addLayout(layout_buttons);

    setLayout(layout_main);

    connect(btn_cancel, SIGNAL(clicked()), this, SLOT(reject()));
    connect(btn_action, SIGNAL(clicked()), this, SLOT(makeAction()));
}

void TabItemGroupActions::makeAction(){
    QString missingFields("");
    if(le_group_name->text() == "") missingFields += "Nom ; ";

    if (missingFields == ""){ // Si tout a été saisi

        QSqlQuery *req = new QSqlQuery;

        req->prepare("SELECT grp_name FROM grp WHERE grp_name = :name");
        req->bindValue(":name", le_group_name->text());
        req->exec();

        // si il n'y a pas d'entrée similaire
        if(!req->next()){
            req->prepare("INSERT INTO grp(grp_name) VALUES (:name)");
            req->bindValue(":name", le_group_name->text());

            if(req->exec()){
                emit notifyRefreshList();
                QMessageBox::information(this, "Requête exécutée avec succès !", "Le groupe a été ajouté dans la base de données !");
                accept();
            }
            else{
                QMessageBox::warning(this, "Erreur !", "La requête n'a pas pu être exécutée !");
            }
        }
        else{
            QMessageBox::warning(this, "Erreur !", "Le groupe existe déjà dans la base de données !");
        }
    }
    else{
        QMessageBox::warning(this, "Action Impossible", "Veuillez remplir les champs vides :\n"+missingFields);
    }
}
