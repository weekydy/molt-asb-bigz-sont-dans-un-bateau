#include "MeetingActions.h"
#include <iostream>

MeetingActions::MeetingActions(int _action, QWidget *parent, int _id) : QDialog(parent)
{
    setFixedWidth(750);
    engine = new SolutionsEngine();
    action = _action;
    id = _id;

    le_label = new QLineEdit();

    dt_begin2 = new QDateTimeEdit(QDateTime::currentDateTime());
    dt_begin2->setDisplayFormat("dd/MM/yyyy hh:mm");
    dt_begin2->setMinimumDate(QDate(QDate::currentDate().addDays(7)));
    dt_begin2->setMinimumTime(QTime(8,0,0,0));
    dt_begin2->setMaximumTime(QTime(19,45,0,0));
    dt_end = new QDateTimeEdit(QDateTime::currentDateTime());
    dt_end->setDisplayFormat("dd/MM/yyyy hh:mm");
    dt_end->setMinimumDate(QDate(QDate::currentDate().addDays(7)));
    dt_end->setMinimumTime(QTime(8,15,0,0));
    dt_end->setMaximumTime(QTime(20,0,0,0));

    qte_duration = new QTimeEdit();
    qte_duration->setDisplayFormat("hh:mm");
    qte_duration->setTimeRange(QTime(0,15,0,0), QTime(12,0,0,0));

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

    qgb_frm = new QGroupBox("D�tails de la r�union");
    qgb_room = new QGroupBox("Recherche de salle");
    qgb_hour = new QGroupBox("Recherche de cr�neau");
    qgb_option = new QGroupBox("Autres options");

    qcb_guest = new QCheckBox("Adapt�e au nombre d'invit�s");
    qcb_equipment = new QCheckBox("Adapt�e aux �quipements");
    qcb_extend = new QCheckBox("Recherche extensible sur plusieurs jours");
    qcb_available = new QCheckBox("Disponibilit� des invit�s n�cessaire");
    qcb_compulsory = new QCheckBox();

    fl_data = new QFormLayout;
    fl_data->addRow("Libell� :", le_label);    
    fl_data->addRow("Salle :", cb_room);
    fl_data->addRow("Equipement(s) :", list_equipments);
    fl_data->addRow("Invit�(s) :", widget_guests);
    qgb_frm->setLayout(fl_data);

    fl_hour = new QFormLayout;
    fl_hour->addRow("D�but :", dt_begin2);
    fl_hour->addRow("Dur�e :", qte_duration);

    cb_colors = new QComboBox();

    cb_colors->addItem(QIcon(":blue.png"), "Bleu", "#668cd9");
    cb_colors->addItem(QIcon(":pink.png"), "Rose", "#dd4477");
    cb_colors->addItem(QIcon(":green.png"), "Vert", "#109618");
    cb_colors->addItem(QIcon(":orange.png"), "Orange", "#dd5511");
    cb_colors->addItem(QIcon(":brown.png"), "Marron", "#b08b59");
    cb_colors->addItem(QIcon(":yellow.png"), "Jaune", "#fad163");

    fl_recurring = new QFormLayout;
    fl_recurring->addRow("Pr�sence obligatoire :", qcb_compulsory);
    fl_recurring->addRow("P�riodicit� :", qcb_recurring);
    fl_recurring->addRow("Couleur :", cb_colors);

    qgb_option->setLayout(fl_recurring);

    if(action == ADD){
        setWindowTitle("Planifier une r�union");
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

    /** Fieldset de la recherche de cr�neau **/
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
    /*QVBoxLayout *vbl_option = new QVBoxLayout;
    vbl_option->addLayout(fl_recurring);
    QHBoxLayout *hbl_option = new QHBoxLayout;
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

    /** Layout des salles et cr�neaux **/
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
    QString missingFields("");
    if (cb_room->currentText() != "Automatique") {
        if (dt_begin2->date() >= QDate::currentDate()) {
            if ((qte_duration->time().hour() != 0 && qte_duration->time().hour() < 12) ||
                (qte_duration->time().hour() == 0 && qte_duration->time().minute() >= 15)) {
                bool available = qcb_available->isChecked();
                bool extend = qcb_extend->isChecked();
                QSqlQuery *req = new QSqlQuery();
                QSet<int> list_users_id;
                for(int i = 0; i < lw_targets->count(); i++)
                {
                    if(lw_targets->item(i)->text().contains("[GRP] "))
                    {

                        int grp_id = lw_targets->item(i)->data(Qt::UserRole).toInt();
                        req->prepare("SELECT user_id FROM belongtogroup WHERE grp_id = :grp_ip");
                        req->bindValue(":grp_id", grp_id);
                        req->exec();

                        while(req->next())
                        {
                            if(!list_users_id.contains(req->value(0).toInt()))
                                list_users_id.insert(req->value(0).toInt());
                        }
                    }
                    else
                    {
                        if(!list_users_id.contains(lw_targets->item(i)->data(Qt::UserRole).toInt()))
                            list_users_id.insert(lw_targets->item(i)->data(Qt::UserRole).toInt());
                    }
                }
                QDateTime qdt = engine->findHours(dt_begin2->dateTime(), qte_duration->time(), cb_room->currentIndex(), list_users_id, available, extend);
                if (qdt.time().hour() == 0) {
                    QString text_result = "Aucune date ne correspond � votre recherche.";
                    QMessageBox::information(this, "R�sultat de la recherche", text_result);
                }
                else {
                    dt_begin2->setDateTime(qdt);
                    dt_end->setDateTime(qdt);
                    dt_end->setTime(QTime(qdt.time().hour() + qte_duration->time().hour(), qdt.time().minute() + qte_duration->time().minute(), 0, 0));
                    QString text_result = "La date du " + qdt.date().toString("dd-MM-yyyy") + " � " + qdt.time().toString("hh:mm") + " est faite pour vous.";
                    QMessageBox::information(this, "R�sultat de la recherche", text_result);
                }
            }
            else {
                QMessageBox::warning(this, "Action Impossible", "Attention, une r�union doit durer entre 15 minutes et 12h");
            }
        }
        else {
            QMessageBox::warning(this, "Action Impossible", "Attention, vous ne pouvez choisir une date ant�rieure � la date courrante.");
        }
    }
    else {
        QMessageBox::warning(this, "Action Impossible", "Veuillez choisir une salle ou en chercher une depuis le module de recherche.");
    }
}


void MeetingActions::findRoom () {
    bool guest = qcb_guest->isChecked();
    bool equipment = qcb_equipment->isChecked();
    QList<int> list_id_equipment;
    QSqlQuery *req = new QSqlQuery();
    QSet<int> list_users_id;
    for(int i = 0; i < lw_targets->count(); i++)
    {
        if(lw_targets->item(i)->text().contains("[GRP] "))
        {

            int grp_id = lw_targets->item(i)->data(Qt::UserRole).toInt();
            req->prepare("SELECT user_id FROM belongtogroup WHERE grp_id = :grp_ip");
            req->bindValue(":grp_id", grp_id);
            req->exec();

            while(req->next())
            {
                if(!list_users_id.contains(req->value(0).toInt()))
                    list_users_id.insert(req->value(0).toInt());
            }
        }
        else
        {
            if(!list_users_id.contains(lw_targets->item(i)->data(Qt::UserRole).toInt()))
                list_users_id.insert(lw_targets->item(i)->data(Qt::UserRole).toInt());
        }
    }
    for (int i = 0; i < list_equipments->count(); ++i) {
        if (list_equipments->itemData(i).toBool()) {
            // on recupere l'id de l'equipement
            req->prepare("SELECT equip_id FROM equipment WHERE equip_name = :name");
            req->bindValue(":name", list_equipments->itemText(i));
            req->exec();
            req->first();
            int id_equipment = req->value(0).toInt();
            list_id_equipment.append(id_equipment);
        }
    }

    int id_room = engine->findRoom(list_users_id.size(), guest, list_id_equipment, equipment);
    if (id_room) {
        cb_room->setCurrentIndex(id_room);
        QString text_result = "La salle " + cb_room->currentText() + " est faite pour vous.";
        QMessageBox::information(this, "R�sultat de la recherche", text_result);
    }
    else
        QMessageBox::warning(this, "R�sultat de la recherche", "Aucune salle ne correspond � vos crit�res de recherche.");
}

void MeetingActions::makeAction(){
     bool available = qcb_available->isChecked();
     bool guest = qcb_guest->isChecked();

    QString missingFields("");
    if(le_label->text() == "") missingFields += "Libell� ; ";
    if(cb_room->currentText() == "Automatique") missingFields += "Salles";

    QString periodicity = "0";
    if (qcb_recurring->currentText() == "Hebdomadaire")
        periodicity = "1";
    else if (qcb_recurring->currentText() == "Mensuel")
        periodicity = "2";

    QSqlQuery *req1 = new QSqlQuery();
    QSet<int> list_users_id;
    for(int i = 0; i < lw_targets->count(); i++)
    {
        if(lw_targets->item(i)->text().contains("[GRP] "))
        {
            int grp_id = lw_targets->item(i)->data(Qt::UserRole).toInt();
            req1->prepare("SELECT user_id FROM belongtogroup WHERE grp_id = :grp_ip");
            req1->bindValue(":grp_id", grp_id);
            req1->exec();

            while(req1->next())
            {
                if(!list_users_id.contains(req1->value(0).toInt()))
                    list_users_id.insert(req1->value(0).toInt());
            }
        }
        else
        {
            if(!list_users_id.contains(lw_targets->item(i)->data(Qt::UserRole).toInt()))
                list_users_id.insert(lw_targets->item(i)->data(Qt::UserRole).toInt());
        }
    }

    if (missingFields == "" && engine->verif(dt_begin2->dateTime(), dt_end->dateTime(), cb_room->currentIndex(), list_users_id, available, guest) == 1){

        QString state = "0";
        if (qcb_compulsory->isChecked())
            state = "1";
        //Requette retournant toutes les r�unions � un jour donn�
        QSqlQuery *req = new QSqlQuery();
        req->prepare("INSERT INTO meeting VALUES (null, :room, :begin, :end, :label, :periodicity, :state, :color)");
        req->bindValue(":begin", dt_begin2->dateTime().toString("yyyy-MM-dd hh:mm"));
        req->bindValue(":end", dt_end->dateTime().toString("yyyy-MM-dd hh:mm"));
        req->bindValue(":label", le_label->text());
        req->bindValue(":room", cb_room->currentIndex());
        req->bindValue(":periodicity", periodicity);
        req->bindValue(":state", state);
        req->bindValue(":color", cb_colors->itemData(cb_colors->currentIndex(), Qt::UserRole));
        req->exec();

        req->prepare("SELECT * FROM meeting m ORDER BY m.meeting_id DESC");
        req->exec();
        req->first();

        foreach (int user_id_to, list_users_id)
        {
            if (user_id_to != id) {
                QSqlQuery *req2 = new QSqlQuery();
                req2->prepare("INSERT INTO havemeeting VALUES (:meeting, :id, :state)");
                req2->bindValue(":meeting", req->value(0).toString());
                req2->bindValue(":id", user_id_to);
                req2->bindValue(":state", state);
                req2->exec();
            }
        }

        QSqlQuery *req2 = new QSqlQuery();
        req2->prepare("INSERT INTO havemeeting VALUES (:meeting, :id, '1')");
        req2->bindValue(":meeting", req->value(0).toString());
        req2->bindValue(":id", id);
        req2->exec();

        req2->prepare(("INSERT INTO organizemeeting VALUES(:meeting, :user)"));
        req2->bindValue(":meeting", req->value(0).toString());
        req2->bindValue(":id", id);
        req2->exec();

        emit notifyRefreshList();
        accept();
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
    fl_data->setWidget(2, QFormLayout::FieldRole, list_equipments);
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
