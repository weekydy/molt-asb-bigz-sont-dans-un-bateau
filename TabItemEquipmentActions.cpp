#include "TabItemEquipmentActions.h"

TabItemEquipmentActions::TabItemEquipmentActions(QWidget *parent) : QDialog(parent)
{
    le_equip_name = new QLineEdit();

    QFormLayout *fl_data = new QFormLayout;
    fl_data->addRow("Designation:", le_equip_name);

    btn_cancel = new QPushButton("&Annuler");
    btn_action = new QPushButton();


    setWindowTitle("Ajouter un l'équipement");
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

void TabItemEquipmentActions::makeAction(){
    QString missingFields("");
    if(le_equip_name->text() == "") missingFields += "Désignation ; ";

    if (missingFields == ""){ // Si tout a été saisi

        QSqlQuery *req = new QSqlQuery;

        req->prepare("SELECT name FROM equipment WHERE name = :name");
        req->bindValue(":name", le_equip_name->text());
        req->exec();

        // si il n'y a pas d'entrée similaire
        if(!req->next()){
            req->prepare("INSERT INTO equipment(name) VALUES (:name)");
            req->bindValue(":name", le_equip_name->text());

            if(req->exec()){

                emit notifyRefreshList();
                QMessageBox::information(this, "Requête exécutée avec succès !", "L'équipement a été ajouté dans la base de données !");
                accept();
            }
            else{
                QMessageBox::warning(this, "Erreur !", "La requête n'a pas pu être exécutée !");
            }
        }
        else{
            QMessageBox::warning(this, "Erreur !", "L'équipement existe déjà dans la base de données !");
        }
    }
    else{
        QMessageBox::warning(this, "Action Impossible", "Veuillez remplir les champs vides :\n"+missingFields);
    }
}
