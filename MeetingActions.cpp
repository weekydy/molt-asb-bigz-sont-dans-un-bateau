#include "MeetingActions.h"
#include <iostream>

MeetingActions::MeetingActions(int _action, QWidget *parent, int _id) : QDialog(parent)
{
    setFixedWidth(750);
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

    qte_duration = new QTimeEdit();
    qte_duration->setDisplayFormat("hh:mm");

    /*list_users = new CheckBoxList();
    list_users->addItem("Choisissez :", false);
    QSqlQuery *req = new QSqlQuery;
    req->prepare("SELECT user_id, user_surname, user_name FROM user ORDER BY user_surname");
    req->exec();
    while(req->next()){
        list_users->addItem(req->value(1).toString() + " " + req->value(2).toString(), false);
    }*/
    lw_groups = new ListWidget(":group.png", this);
    lw_users = new ListWidget(":person.png", this);
    lw_targets = new ListWidget(":person_not.png", this);

    QSqlQuery *req = new QSqlQuery();
    QSqlRecord rec;
    req->prepare("SELECT * FROM grp ORDER BY grp_name");
    req->exec();
    rec = req->record();
    while(req->next()){
        QListWidgetItem *item = new QListWidgetItem();
        item->setData(Qt::UserRole, req->value(rec.indexOf("grp_id")));
        item->setText("[GRP] " + req->value(rec.indexOf("grp_name")).toString());
        lw_groups->addItem(item);
    }
    req->prepare("SELECT * FROM user ORDER BY user_surname, user_name");
    req->exec();
    rec = req->record();
    while(req->next()){
        QListWidgetItem *item = new QListWidgetItem();
        item->setData(Qt::UserRole, req->value(rec.indexOf("user_id")));
        item->setText("[IND] " + req->value(rec.indexOf("user_surname")).toString() + " " + req->value(rec.indexOf("user_name")).toString());
        lw_users->addItem(item);
    }

    QLabel *lb_group = new QLabel("Groupes :");
    lb_group->setAlignment(Qt::AlignCenter);
    QLabel *lb_user = new QLabel("Personnes :");
    lb_user->setAlignment(Qt::AlignCenter);
    QLabel *lb_user_to = new QLabel("Destinataires :");
    lb_user_to->setAlignment(Qt::AlignCenter);

    QVBoxLayout *layout_target = new QVBoxLayout();
    layout_target->addWidget(lb_user_to);
    layout_target->addWidget(lw_targets);

    QVBoxLayout *layout_sources = new QVBoxLayout();
    layout_sources->addWidget(lb_group);
    layout_sources->addWidget(lw_groups);
    layout_sources->addWidget(lb_user);
    layout_sources->addWidget(lw_users);

    QHBoxLayout *layout_guests = new QHBoxLayout();
    layout_guests->addLayout(layout_sources);
    layout_guests->addLayout(layout_target);

    QWidget *widget_guests = new QWidget();
    widget_guests->setLayout(layout_guests);





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

    qcb_recurring = new QComboBox();
    qcb_recurring->addItem("Ponctuel");
    qcb_recurring->addItem("Hebdomadaire");
    qcb_recurring->addItem("Mensuel");

    btn_findHour = new QPushButton("Chercher");
    btn_findRoom = new QPushButton("Chercher");
    btn_action = new QPushButton("Valider");
    //btn_action->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    btn_cancel = new QPushButton("&Annuler");
    //btn_cancel->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

    qgb_frm = new QGroupBox("Détails de la réunion");
    qgb_room = new QGroupBox("Recherche de salle");
    qgb_hour = new QGroupBox("Recherche de crénau");
    qgb_option = new QGroupBox("Autres options");

    qcb_guest = new QCheckBox("Adapté au nombre d'invités");
    qcb_equipment = new QCheckBox("Adapté aux équipements");
    qcb_extend = new QCheckBox("Recherche extensible sur plusieurs jours");
    qcb_available = new QCheckBox("Disponibilité des invités nécessaire");
    qcb_compulsory = new QCheckBox();

    fl_data = new QFormLayout;
    fl_data->addRow("Libellé :", le_label);
    fl_data->addRow("Début :", dt_begin);
    fl_data->addRow("Fin :", dt_end);
    fl_data->addRow("Invité(s) :", widget_guests);
    fl_data->addRow("Salle :", cb_room);
    fl_data->addRow("Equipement(s) :", list_equipments);
    qgb_frm->setLayout(fl_data);

    fl_hour = new QFormLayout;
    fl_hour->addRow("Début :", dt_begin2);
    fl_hour->addRow("Durée :", qte_duration);

    fl_recurring = new QFormLayout;
    fl_recurring->addRow("Présence obligatoire :", qcb_compulsory);
    fl_recurring->addRow("Périodicité :", qcb_recurring);

    qgb_option->setLayout(fl_recurring);

    if(action == ADD){
        setWindowTitle("Planifier une réunion");
    }

    /** Fieldset de la recherche de salle **/
    QVBoxLayout *vbl_room1 = new QVBoxLayout;
    vbl_room1->addWidget(qcb_guest);
    vbl_room1->addWidget(qcb_equipment);
    QVBoxLayout *vbl_room2 = new QVBoxLayout;
    vbl_room2->addWidget(btn_findRoom);

    QHBoxLayout *hbl_room = new QHBoxLayout;
    hbl_room->setAlignment(Qt::AlignLeft);
    qgb_room->setLayout(hbl_room);
    hbl_room->addLayout(vbl_room1);
    hbl_room->addStretch();
    hbl_room->addLayout(vbl_room2);

    /** Fieldset de la recherche de crénau **/
    QVBoxLayout *vbl_hour1 = new QVBoxLayout;
    vbl_hour1->addLayout(fl_hour);
    vbl_hour1->addWidget(qcb_extend);
    vbl_hour1->addWidget(qcb_available);
    QVBoxLayout *vbl_hour2 = new QVBoxLayout;
    vbl_hour2->setAlignment(Qt::AlignRight);
    vbl_hour2->addWidget(btn_findHour);

    QHBoxLayout *hbl_hour = new QHBoxLayout;
    hbl_hour->setAlignment(Qt::AlignLeft);
    qgb_hour->setLayout(hbl_hour);
    hbl_hour->addLayout(vbl_hour1);
    hbl_hour->addStretch();
    hbl_hour->addLayout(vbl_hour2);

    /** Fieldset de options **/
    QVBoxLayout *vbl_option = new QVBoxLayout;
    vbl_option->addLayout(fl_recurring);

    /*QHBoxLayout *hbl_option = new QHBoxLayout;
    hbl_option->setAlignment(Qt::AlignLeft);
    qgb_option->setLayout(hbl_option);
    hbl_option->addLayout(vbl_option);*/

    /** Layout des boutons **/
    QHBoxLayout *layout_buttons = new QHBoxLayout;
    layout_buttons->setAlignment(Qt::AlignRight);
    layout_buttons->addWidget(btn_action);
    layout_buttons->addWidget(btn_cancel);

    /** Layout des options et des boutons **/
    /*QHBoxLayout *hbl_ob = new QHBoxLayout;
    hbl_ob->addWidget(qgb_option);
    hbl_ob->addLayout(layout_buttons);*/

    /** Layout des salles et crénaux **/
    QVBoxLayout *vbl_rh = new QVBoxLayout;
    vbl_rh->addWidget(qgb_room);
    vbl_rh->addWidget(qgb_hour);
    vbl_rh->addWidget(qgb_option);
    vbl_rh->addLayout(layout_buttons);

    /** Layout contenant les recherches et le formulaire **/
    QHBoxLayout *hbl_frh = new QHBoxLayout;
    hbl_frh->addWidget(qgb_frm);
    hbl_frh->addLayout(vbl_rh);



    QVBoxLayout *layout_main = new QVBoxLayout;
    layout_main->addLayout(hbl_frh);

    setLayout(layout_main);

    connect(btn_cancel, SIGNAL(clicked()), this, SLOT(reject()));
    connect(btn_action, SIGNAL(clicked()), this, SLOT(makeAction()));
    connect(cb_room, SIGNAL(currentIndexChanged(int)), this, SLOT(changeComboBoxEquipment(int)));
    connect(btn_findHour, SIGNAL(clicked()), this, SLOT(findHours()));
    connect(btn_findRoom, SIGNAL(clicked()), this, SLOT(findRoom()));
}

void MeetingActions::findHours () {
    bool available = qcb_available->isChecked();
    bool extend = qcb_extend->isChecked();
    QList<int> liste;
    liste.append(1);
    liste.append(2);
    liste.append(3);
    QDateTime qdt = engine->findHours(dt_begin2->dateTime(), qte_duration->time(), 2, liste, available, extend);
    if (qdt.time().hour() == 0) {
        QString text_result = "Aucune date ne correspond à votre recherche.";
        QMessageBox::information(this, "Résultat de la recherche", text_result);
        dt_begin->setDateTime(qdt);
        dt_end->setDateTime(qdt);
        dt_end->setTime(QTime(qdt.time().hour() + qte_duration->time().hour(), qdt.time().minute() + qte_duration->time().minute(), 0, 0));
    }
    else {
        QString text_result = "La date du " + qdt.date().toString("dd-MM-yyyy") + " à " + qdt.time().toString("hh:mm") + " est faite pour vous.";
        QMessageBox::information(this, "Résultat de la recherche", text_result);
    }
}

void MeetingActions::findRoom () {
    int nb = 5;
    //bool guest = qcb_guest->isChecked();
    //bool equipment = qcb_equipment->isChecked();
    int id_room = engine->findRoom(nb);
    QString text_result = "La salle " + QString::number(id_room) + " est faite pour vous.";
    QMessageBox::information(this, "Résultat de la recherche", text_result);
}

void MeetingActions::makeAction(){

    QString missingFields("");
    if(le_label->text() == "") missingFields += "Libellé ; ";
    if(dt_begin->text() == "") missingFields += "Horaire début ; ";
    if(dt_end->text() == "") missingFields += "Horaire fin ; ";

    if (missingFields == ""){
        //Requette retournant toutes les réunions à un jour donné
        QSqlQuery *req = new QSqlQuery();
        req->prepare("INSERT INTO meeting VALUES (null, :room, :begin, :end, :label, '0')");
        req->bindValue(":begin", dt_begin->dateTime().toString("yyyy-MM-dd hh:mm"));
        req->bindValue(":end", dt_end->dateTime().toString("yyyy-MM-dd hh:mm"));
        req->bindValue(":label", le_label->text());
        req->bindValue(":room", cb_room->currentIndex());
        req->exec();

        req->prepare("SELECT * FROM meeting m ORDER BY m.meeting_id DESC");
        req->exec();
        req->first();

        QSqlQuery *req2 = new QSqlQuery();
        req2->prepare("INSERT INTO havemeeting VALUES (:meeting, :id, '0')");
        req2->bindValue(":meeting", req->value(0).toString());
        req2->bindValue(":id", id);
        req2->exec();

        emit notifyRefreshList();

        accept();
                // Si tout a été saisi
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
    accept();

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

void MeetingActions::moveCurrentItem(ListWidget *source, ListWidget *target)
{
    if (source->currentItem()) {
        QListWidgetItem *newItem = source->currentItem()->clone();
        target->addItem(newItem);
        target->setCurrentItem(newItem);
        delete source->currentItem();
    }
}
