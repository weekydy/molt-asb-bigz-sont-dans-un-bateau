#include "MeetingActions.h"
#include <iostream>

MeetingActions::MeetingActions(int _action, QWidget *parent, int _id) : QDialog(parent)
{
    engine = new SolutionsEngine();
    action = _action;
    id = _id;

    le_label = new QLineEdit();

    dt_begin = new QDateTimeEdit(QDateTime::currentDateTime());
    dt_begin->setDisplayFormat("dd/MM/yyyy hh:mm");
    dt_begin2 = new QDateTimeEdit(QDateTime::currentDateTime());
    dt_begin2->setDisplayFormat("dd/MM/yyyy hh:mm");
    dt_end = new QDateTimeEdit(QDateTime::currentDateTime());
    dt_end->setDisplayFormat("dd/MM/yyyy hh:mm");

    list_users = new CheckBoxList();
    list_users->addItem("Choisissez :", false);
    QSqlQuery *req = new QSqlQuery;
    req->prepare("SELECT user_id, user_surname, user_name FROM user ORDER BY user_surname");
    req->exec();
    while(req->next()){
        list_users->addItem(req->value(1).toString() + " " + req->value(2).toString(), false);
    }

    cb_room = new QComboBox();
    cb_room->addItem("Automatique", -1);
    req->prepare("SELECT * FROM room ORDER BY room_name");
    req->exec();
    while(req->next()){
        cb_room->addItem(req->value(1).toString() + " (" + req->value(2).toString() + " pers. max)", req->value(0).toInt());
    }

    list_equipments = new CheckBoxList();
    list_equipments->addItem("Choisissez :", false);
    req->prepare("SELECT equip_id, equip_name FROM equipment ORDER BY equip_name");
    req->exec();
    while(req->next()){
        list_equipments->addItem(req->value(1).toString(), false);
    }

    fl_data = new QFormLayout;
    fl_data->addRow("Libellé:", le_label);
    fl_data->addRow("Début:", dt_begin);
    fl_data->addRow("Fin:", dt_end);
    fl_data->addRow("Invité(s):", list_users);
    fl_data->addRow("Salle:", cb_room);
    fl_data->addRow("Equipement(s):", list_equipments);

    btn_findHour = new QPushButton("Chercher");
    btn_findRoom = new QPushButton("Chercher");
    btn_action = new QPushButton("Valider");
    btn_cancel = new QPushButton("&Annuler");

    qgb_room = new QGroupBox("Recherche de salle");
    qgb_hour = new QGroupBox("Recherche de crénau");
    qgb_option = new QGroupBox("Autres options");

    qcb_guest = new QCheckBox("Adapté au nombre d'invités");
    qcb_equipment = new QCheckBox("Ayant comme équipements");
    qcb_extend = new QCheckBox("Recherche extensible sur plusieurs jours");
    qcb_available = new QCheckBox("Disponibilité des invités nécessaire");
    qcb_compulsory = new QCheckBox("Présence obligatoire");
    qcb_recurring = new QCheckBox("Périodique");

    if(action == ADD){
        setWindowTitle("Planifier une réunion");
    }

    QHBoxLayout *layout_buttons = new QHBoxLayout;
    layout_buttons->setAlignment(Qt::AlignRight);
    layout_buttons->addWidget(btn_findHour);
    layout_buttons->addWidget(btn_findRoom);
    layout_buttons->addWidget(btn_action);
    layout_buttons->addWidget(btn_cancel);

    QVBoxLayout *layout_main = new QVBoxLayout;
    layout_main->addLayout(fl_data);
    layout_main->addLayout(layout_buttons);

    setLayout(layout_main);

    connect(btn_cancel, SIGNAL(clicked()), this, SLOT(reject()));
    connect(btn_action, SIGNAL(clicked()), this, SLOT(makeAction()));
    connect(cb_room, SIGNAL(currentIndexChanged(int)), this, SLOT(changeComboBoxEquipment(int)));
    connect(btn_findHour, SIGNAL(clicked()), this, SLOT(findHours()));
    connect(btn_findRoom, SIGNAL(clicked()), this, SLOT(findRoom()));
}

void MeetingActions::findHours () {
    QList<int> liste;
    liste.append(1);
    liste.append(2);
    liste.append(3);
    QDateTime qdt = engine->findHours(QDateTime::currentDateTime(), 40, 2, liste);
}

void MeetingActions::findRoom () {
    int nb = 5;
    int id_room = engine->findRoom(nb);
    std::cout << id_room;
}

void MeetingActions::makeAction(){
    QString missingFields("");
    if(le_label->text() == "") missingFields += "Libellé ; ";
    if(dt_begin->text() == "") missingFields += "Horaire début ; ";
    if(dt_end->text() == "") missingFields += "Horaire fin ; ";

    if (missingFields == ""){ // Si tout a été saisi
        /*
        QMap<QString, bool> map_list;
        for(int i = 0; i < list->count(); ++i){
            map_list[list->itemText(i)] = list->itemData(i).toBool();
            qDebug() << list->itemData(i);
        }


        QSqlQuery *req = new QSqlQuery;




        req->prepare("SELECT room_id, room_name FROM room WHERE room_name = :name");
        req->bindValue(":name", le_room_name->text());
        req->exec();

        if(action == EDIT){
            // si il n'y a pas d'entrée similaire, ou si on modifie notre propre occurence
            if(!req->next() || req->value(0).toInt() == id){

                req->prepare("UPDATE room SET room_name = :name, room_capacity = :capacity WHERE room_id = :id");
                req->bindValue(":id", id);
                req->bindValue(":name", le_room_name->text());
                req->bindValue(":capacity", (int)le_room_capacity->value());

                if(req->exec()){

                    // on supprime tout les equipements
                    req->prepare("DELETE FROM haveequipment WHERE room_id = :id_room");
                    req->bindValue(":id_room", id);
                    req->exec();

                    // on parcours la liste, et on ajoute les equipements au fur et à mesure
                    QMapIterator<QString, bool> it(map_list);
                    while(it.hasNext()){
                        it.next();
                        if(it.value()){ // si l'équipement est coché
                            // on recupere l'id de l'equipement
                            req->prepare("SELECT equip_id FROM equipment WHERE equip_name = :name");
                            req->bindValue(":name", it.key());
                            req->exec();
                            req->first();
                            int id_equipment = req->value(0).toInt();

                            // on ajoute l'equipement
                            req->prepare("INSERT INTO haveequipment VALUES(:id_room, :id_equipment)");
                            req->bindValue(":id_room", id);
                            req->bindValue(":id_equipment", id_equipment);
                            req->exec();
                        }
                    }

                    emit notifyRefreshList();
                    QMessageBox::information(this, "Requête exécutée avec succès !", "La salle a été modifié dans la base de données !");
                    accept();

                }
                else{
                    QMessageBox::warning(this, "Erreur !", "La requête n'a pas pu être exécutée !");
                }
            }
            else{
                QMessageBox::warning(this, "Erreur !", "La salle existe déjà dans la base de données !");
            }
        }
        else if(action == ADD){
            // si il n'y a pas d'entrée similaire
            if(!req->next()){
                req->prepare("INSERT INTO room(room_name, room_capacity) VALUES (:name, :capacity)");
                req->bindValue(":name", le_room_name->text());
                req->bindValue(":capacity", le_room_capacity->value());

                if(req->exec()){
                    // on met à jour l'id
                    id = req->lastInsertId().toInt();


                    // on supprime tout les equipements
                    req->prepare("DELETE FROM haveequipment WHERE room_id = :id_room");
                    req->bindValue(":id_room", id);
                    req->exec();

                    // on parcours la liste, et on ajoute les equipements au fur et à mesure
                    QMapIterator<QString, bool> it(map_list);
                    while(it.hasNext()){
                        it.next();
                        if(it.value()){ // si l'équipement est coché
                            // on recupere l'id de l'equipement
                            req->prepare("SELECT equip_id FROM equipment WHERE equip_name = :name");
                            req->bindValue(":name", it.key());
                            req->exec();
                            req->first();
                            int id_equipment = req->value(0).toInt();

                            // on ajoute l'equipement
                            req->prepare("INSERT INTO haveequipment VALUES(:id_room, :id_equipment)");
                            req->bindValue(":id_room", id);
                            req->bindValue(":id_equipment", id_equipment);
                            req->exec();
                        }
                    }


                    emit notifyRefreshList();
                    QMessageBox::information(this, "Requête exécutée avec succès !", "La salle a été ajouté dans la base de données !");
                    accept();
                }
                else{
                    QMessageBox::warning(this, "Erreur !", "La requête n'a pas pu être exécutée !");
                }
            }
            else{
                QMessageBox::warning(this, "Erreur !", "La salle existe déjà dans la base de données !");
            }
        }*/
    }
    else{
        QMessageBox::warning(this, "Action Impossible", "Veuillez remplir les champs vides :\n"+missingFields);
    }

}

void MeetingActions::changeComboBoxEquipment(int index){
    //qDebug() << index;
    if(cb_room->itemData(cb_room->currentIndex()).toInt() != -1){ // mode manuel
        QComboBox *tmp = list_equipments;
        list_equipments = new QComboBox();
        delete tmp;

        QSqlQuery *req = new QSqlQuery();
        req->prepare("SELECT e.equip_id, e.equip_name FROM equipment e INNER JOIN haveequipment he ON e.equip_id = he.equip_id INNER JOIN room r ON r.room_id = he.room_id WHERE r.room_id = :room_id ORDER BY equip_name");
        qDebug() << list_equipments->itemData(list_equipments->currentIndex()).toInt();
        req->bindValue(":room_id", cb_room->itemData(cb_room->currentIndex()).toInt());
        req->exec();
        while(req->next()){
            list_equipments->addItem(req->value(1).toString(), false);
        }
    }
    else{ // mode automatique
        QComboBox *tmp = list_equipments;
        list_equipments = new CheckBoxList();        
        delete tmp;

        list_equipments->addItem("Choisissez :", false);
        QSqlQuery *req = new QSqlQuery();
        req->prepare("SELECT equip_id, equip_name FROM equipment ORDER BY equip_name");
        req->exec();
        while(req->next()){
            list_equipments->addItem(req->value(1).toString(), false);
        }
    }
    fl_data->setWidget(5, QFormLayout::FieldRole, list_equipments);
}
