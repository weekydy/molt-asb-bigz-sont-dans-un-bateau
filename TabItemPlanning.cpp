#include "TabItemPlanning.h"



class Meeting{
public:
    Meeting(){ _id = -1; _pos = -1; _size = -1; }
    Meeting(int id, int pos, int size) : _id(id), _pos(pos), _size(size){}
    ~Meeting(){}
    int id(){ return _id; }
    int pos(){ return _pos; }
    int size(){ return _size; }
private:
    int _id;
    int _pos;
    int _size;
};

Q_DECLARE_METATYPE(Meeting)

//qRegisterMetaType<Meeting>("Meeting");


TabItemPlanning::TabItemPlanning(int _user_id, QWidget *parent) : QWidget(parent)
{
    user_id = _user_id;
    model = new QStandardItemModel();
    view = new QTableView();
    view->setEditTriggers(QAbstractItemView::NoEditTriggers); // pas d'édition
    view->setFocusPolicy(Qt::NoFocus); // pas de sélection bleue
    view->setSelectionMode(QAbstractItemView::SingleSelection);



    calendar = new QCalendarWidget();
    calendar->setGridVisible(true);
    calendar->setMaximumWidth(280);
    calendar->setMaximumHeight(200);
    calendar->setFirstDayOfWeek(Qt::Monday);

    cb_view = new QComboBox();
    cb_view->addItem("Jour");
    cb_view->addItem("Semaine");
    cb_view->addItem("Mois");
    cb_view->setCurrentIndex(1); // par défaut on affiche la semaine

    // on prépare le header vertical
    for(int i = 8; i < 20; ++i){ // 8h - 20h
        QString hour = QString::number(i);
        hours   << hour + "h00"
                << hour + "h15"
                << hour + "h30"
                << hour + "h45";
    }

    colours = new QList<QColor>();
    colours->push_back(QColor(102, 140, 217)); // bleu
    colours->push_back(QColor(221, 68, 119)); // rose
    colours->push_back(QColor(16, 150, 24)); //vert
    colours->push_back(QColor(221, 85, 17)); // orange
    colours->push_back(QColor(176, 139, 89)); // marron
    colours->push_back(QColor(250, 209, 99)); // jaune

    refreshList();

    btn_add = new QPushButton("Organiser une réunion");

    QVBoxLayout *layout_tools = new QVBoxLayout();
    layout_tools->addWidget(calendar);
    layout_tools->addWidget(cb_view);
    layout_tools->addWidget(btn_add);
    layout_tools->addStretch();


    QHBoxLayout *layout_main = new QHBoxLayout();
    layout_main->addLayout(layout_tools);
    layout_main->addWidget(view);

    setLayout(layout_main);

    connect(view, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(createMenu(QPoint)));
    connect(cb_view, SIGNAL(currentIndexChanged(int)), this, SLOT(refreshList()));
    connect(calendar, SIGNAL(clicked(QDate)), this, SLOT(refreshList()));
    connect(calendar, SIGNAL(currentPageChanged(int,int)), this, SLOT(refreshList()));
    connect(view, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(displayInfo()));
    connect(btn_add, SIGNAL(clicked()), this, SLOT(addMeeting()));


}

void TabItemPlanning::refreshList(){
    int index = cb_view->currentIndex();
    QDate date = calendar->selectedDate();


    QStandardItemModel *tmp = model;
    model = new QStandardItemModel();
    delete tmp;

    view->reset();
    view->clearSpans(); // reset les cellules fusionnées
    //view->horizontalHeader()->setStretchLastSection(true);




    if(index == 0){ // day
        view->verticalHeader()->setDefaultSectionSize(20);
        view->verticalHeader()->show();
        //view->setColumnWidth(0, 300);

        QStringList days;
        switch(date.dayOfWeek()){
            case 1: days << "Lundi"; break;
            case 2: days << "Mardi"; break;
            case 3: days << "Mercredi"; break;
            case 4: days << "Jeudi"; break;
            case 5: days << "Vendredi"; break;
            case 6: days << "Samedi"; break;
            case 7: days << "Dimanche"; break;
            default: days << "Lundi"; break;
        }
        model->setHorizontalHeaderLabels(days);
        model->setVerticalHeaderLabels(hours);


        QStringList date_y_m_d = date.toString("yyyy-MM-dd").split("-");

        QSqlQuery *req = new QSqlQuery();
        QSqlRecord rec;
        req->prepare("SELECT * FROM user u INNER JOIN havemeeting hm ON u.user_id = hm.user_id INNER JOIN meeting m ON m.meeting_id = hm.meeting_id INNER JOIN room r ON r.room_id = m.room_id WHERE hm.hm_state = :state AND u.user_id = :user_id AND ((strftime('%Y', m.meeting_begin) = :year AND strftime('%m', m.meeting_begin) = :month AND strftime('%d', m.meeting_begin) = :day) OR (m.meeting_periodic > 0)) ORDER BY meeting_begin");
        req->bindValue(":user_id", user_id);
        req->bindValue(":year", date_y_m_d.at(0));
        req->bindValue(":month", date_y_m_d.at(1));
        req->bindValue(":day", date_y_m_d.at(2));
        req->bindValue(":state", 1);
        req->exec();
        rec = req->record();

        int colour = 0;
        while(req->next()){

            // Si c'est un rendez-vous periodique hebdomadaire.
            if(req->value(rec.indexOf("meeting_periodic")).toInt() == 1)
            {
                QDateTime meeting_begin = QDateTime::fromString(req->value(rec.indexOf("meeting_begin")).toString(), "yyyy-MM-dd hh:mm");
                if(meeting_begin.date().dayOfWeek() != date.dayOfWeek() || meeting_begin.date().daysTo(date) < 0)
                    continue;
            }
            // Si c'est un rendez-vous periodique mensuel.
            else if(req->value(rec.indexOf("meeting_periodic")).toInt() == 2)
            {
                QDateTime meeting_begin = QDateTime::fromString(req->value(rec.indexOf("meeting_begin")).toString(), "yyyy-MM-dd hh:mm");
                if(meeting_begin.date().daysTo(date) % 28 != 0 || meeting_begin.date().daysTo(date) < 0)
                    continue;
            }


            QString datetime_b = req->value(rec.indexOf("meeting_begin")).toString();
            QString datetime_e = req->value(rec.indexOf("meeting_end")).toString();

            QStringList time_b = datetime_b.split(" ").at(1).split(":"); // ["hh", "mm"]
            QStringList time_e = datetime_e.split(" ").at(1).split(":");


            int noRow_b = (time_b.at(0).toInt() - 8) * 4 + time_b.at(1).toInt() / 15; // numéro de la ligne où le RV commence
            int noRow_e = ((time_e.at(0).toInt() - 8) * 4 + time_e.at(1).toInt() / 15) - 1; // numéro de la ligne où le RV termine, -1 : [8h00 - 10h30] => 10h30 - 10h45 non compris

            int duration = noRow_e - noRow_b + 1; // +1 car l'indice des lignes commence à 0

            for(int pos = 0; pos < duration; pos++){
                Meeting m(req->value(rec.indexOf("meeting_id")).toInt(), pos, duration);

                qDebug() << m.id() << m.pos() << m.size();

                QStandardItem *item = new QStandardItem();
                if(pos == 0){ item->setText(req->value(rec.indexOf("meeting_label")).toString()); }

                QVariant data;
                data.setValue(m);
                item->setData(data);
                item->setBackground(colours->at(colour));

                model->setItem(noRow_b + pos, 0, item);
            }

            view->setSpan(noRow_b, 0, duration, 1);

            colour++;
        }
    }
    else if(index == 1){ // week
        view->verticalHeader()->setDefaultSectionSize(20);

        QStringList days;
        days << "Lundi" << "Mardi" << "Mercredi" << "Jeudi" << "Vendredi" << "Samedi" << "Dimanche";
        model->setHorizontalHeaderLabels(days);
        model->setVerticalHeaderLabels(hours);
        view->verticalHeader()->show();
        view->verticalHeader()->setDefaultSectionSize(20);

        // recherche de la date de début de semaine
        while(date.dayOfWeek() != 1){ // on cherche le lundi de la semaine
            date = date.addDays(-1);
        }

        bool ok = true;
        while(date.dayOfWeek() <= 7 && ok){ // on parcourt la semaine (1 à 7)
            qDebug() << date.toString("yyyy-MM-dd");

            QStringList date_y_m_d = date.toString("yyyy-MM-dd").split("-");

            QSqlQuery *req = new QSqlQuery();
            QSqlRecord rec;
            req->prepare("SELECT * FROM user u INNER JOIN havemeeting hm ON u.user_id = hm.user_id INNER JOIN meeting m ON m.meeting_id = hm.meeting_id INNER JOIN room r ON r.room_id = m.room_id WHERE hm.hm_state = :state AND u.user_id = :user_id AND ((strftime('%Y', m.meeting_begin) = :year AND strftime('%m', m.meeting_begin) = :month AND strftime('%d', m.meeting_begin) = :day) OR (m.meeting_periodic > 0)) ORDER BY meeting_begin");
            req->bindValue(":user_id", user_id);
            req->bindValue(":year", date_y_m_d.at(0));
            req->bindValue(":month", date_y_m_d.at(1));
            req->bindValue(":day", date_y_m_d.at(2));
            req->bindValue(":state", 1);
            req->exec();
            rec = req->record();

            int colour = 0;
            int noCol = date.dayOfWeek() - 1;
            while(req->next()){
                // Si c'est un rendez-vous periodique hebdomadaire.
                if(req->value(rec.indexOf("meeting_periodic")).toInt() == 1)
                {
                    QDateTime meeting_begin = QDateTime::fromString(req->value(rec.indexOf("meeting_begin")).toString(), "yyyy-MM-dd hh:mm");
                    if(meeting_begin.date().dayOfWeek() != date.dayOfWeek() || meeting_begin.date().daysTo(date) < 0)
                        continue;
                }
                // Si c'est un rendez-vous periodique mensuel.
                else if(req->value(rec.indexOf("meeting_periodic")).toInt() == 2)
                {
                    QDateTime meeting_begin = QDateTime::fromString(req->value(rec.indexOf("meeting_begin")).toString(), "yyyy-MM-dd hh:mm");
                    if(meeting_begin.date().daysTo(date) % 28 != 0 || meeting_begin.date().daysTo(date) < 0)
                        continue;
                }



                QString datetime_b = req->value(rec.indexOf("meeting_begin")).toString();
                QString datetime_e = req->value(rec.indexOf("meeting_end")).toString();

                QStringList time_b = datetime_b.split(" ").at(1).split(":"); // ["hh", "mm"]
                QStringList time_e = datetime_e.split(" ").at(1).split(":");


                int noRow_b = (time_b.at(0).toInt() - 8) * 4 + time_b.at(1).toInt() / 15; // numéro de la ligne où le RV commence
                int noRow_e = ((time_e.at(0).toInt() - 8) * 4 + time_e.at(1).toInt() / 15) - 1; // numéro de la ligne où le RV termine

                int duration = noRow_e - noRow_b + 1; // +1 car l'indice des lignes commence à 0

                for(int pos = 0; pos < duration; pos++){
                    Meeting m(req->value(rec.indexOf("meeting_id")).toInt(), pos, duration);

                    qDebug() << m.id() << m.pos() << m.size();

                    QStandardItem *item = new QStandardItem();
                    if(pos == 0){ item->setText(req->value(rec.indexOf("meeting_label")).toString()); }

                    QVariant data;
                    data.setValue(m);
                    item->setData(data);
                    item->setBackground(colours->at(colour));

                    model->setItem(noRow_b + pos, noCol, item);
                }

                view->setSpan(noRow_b, noCol, duration, 1);

                colour++;
            }
            if(date.dayOfWeek() == 7){ ok = false; } // incrementation
            else date = date.addDays(1);
        }
    }
    else{ // month

        QStringList days;
        days << "Lundi" << "Mardi" << "Mercredi" << "Jeudi" << "Vendredi" << "Samedi" << "Dimanche";
        model->setHorizontalHeaderLabels(days);
        view->verticalHeader()->hide();
        //view->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
        view->verticalHeader()->setResizeMode(QHeaderView::Stretch);
        //model->setVerticalHeaderLabels(hours);
        QDate date_backup = date;
        // recherche de la date de début de semaine de début de mois
        while(date.dayOfWeek() != 1){ // on cherche le lundi de la semaine
            date = date.addDays(-1);
        }
        //qDebug() << date.toString("yyyy-MM-dd");
        while(date.day() != 1 && date.month() == date_backup.month()){
            date = date.addDays(-7);
        }
        //qDebug() << date.toString("yyyy-MM-dd");

        int nbDaysInMonth = date.daysInMonth();
        int i = 1;
        int nbDaysUseless = 0;
        while(date.dayOfWeek() != 7 || i < nbDaysInMonth + nbDaysUseless){ // on parcours le mois





            qDebug() << date.dayOfWeek() << " " << i << " " << nbDaysInMonth + nbDaysUseless;
            if(date.month() != date_backup.month()){ nbDaysUseless++; }
            qDebug() << date.toString("yyyy-MM-dd");

            QStringList date_y_m_d = date.toString("yyyy-MM-dd").split("-");

            QSqlQuery *req = new QSqlQuery();
            QSqlRecord rec;
            req->prepare("SELECT * FROM user u INNER JOIN havemeeting hm ON u.user_id = hm.user_id INNER JOIN meeting m ON m.meeting_id = hm.meeting_id INNER JOIN room r ON r.room_id = m.room_id WHERE hm.hm_state = :state AND u.user_id = :user_id AND ((strftime('%Y', m.meeting_begin) = :year AND strftime('%m', m.meeting_begin) = :month AND strftime('%d', m.meeting_begin) = :day) OR (m.meeting_periodic > 0)) ORDER BY meeting_begin");
            req->bindValue(":user_id", user_id);
            req->bindValue(":year", date_y_m_d.at(0));
            req->bindValue(":month", date_y_m_d.at(1));
            req->bindValue(":day", date_y_m_d.at(2));
            req->bindValue(":state", 1);
            req->exec();
            rec = req->record();


            int noCol = date.dayOfWeek() - 1;
            QString data(date.toString("d MMM")+" :\n\n");
            while(req->next()){
                // Si c'est un rendez-vous periodique hebdomadaire.
                if(req->value(rec.indexOf("meeting_periodic")).toInt() == 1)
                {
                    QDateTime meeting_begin = QDateTime::fromString(req->value(rec.indexOf("meeting_begin")).toString(), "yyyy-MM-dd hh:mm");
                    if(meeting_begin.date().dayOfWeek() != date.dayOfWeek() || meeting_begin.date().daysTo(date) < 0)
                        continue;
                }
                // Si c'est un rendez-vous periodique mensuel.
                else if(req->value(rec.indexOf("meeting_periodic")).toInt() == 2)
                {
                    QDateTime meeting_begin = QDateTime::fromString(req->value(rec.indexOf("meeting_begin")).toString(), "yyyy-MM-dd hh:mm");
                    if(meeting_begin.date().daysTo(date) % 28 != 0 || meeting_begin.date().daysTo(date) < 0)
                        continue;
                }


                QString datetime_b = req->value(rec.indexOf("meeting_begin")).toString();

                QStringList time_b = datetime_b.split(" ").at(1).split(":"); // ["hh", "mm"]                

                data += time_b.at(0) + ":" + time_b.at(1) + " " + req->value(rec.indexOf("meeting_label")).toString() + "\n";
            }
            qDebug() << data;
            QStandardItem *item = new QStandardItem();
            item->setText(data);

            model->setItem(((i - 1) / 7), noCol, item);

            qDebug() << "jour : " << date.toString("dd-MM-yyyy");
            date = date.addDays(1);
            i++;

        }
    }
    view->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    view->setModel(model);
}

void TabItemPlanning::displayInfo(){
    QModelIndex index = view->selectionModel()->currentIndex();
    if(index.isValid()){
        if(cb_view->currentText() != "Mois")
        {
            Meeting m = model->item(index.row(), index.column())->data().value<Meeting>();
            MeetingDetails *meeting_details = new MeetingDetails(user_id, m.id());
            connect(meeting_details, SIGNAL(notifyRefreshList()), this, SLOT(refreshList()));
            meeting_details->exec();
        }
    }
}

void TabItemPlanning::createMenu(QPoint pos){
    /*
    QMenu *menu = new QMenu(this);
    QAction *action_add = new QAction("&Nouveau", this);
    menu->addAction(action_add);

    QModelIndex index = view->selectionModel()->currentIndex();
    index = index.sibling(index.row(), 0); // ici je force le n° de colonne à 0, pour etre sur le premier champ
    if (index.row() != -1){

        QAction *action_del = new QAction("&Supprimer", this);
        QAction *action_edit = new QAction("&Modifier", this);
        menu->addAction(action_del);
        menu->addAction(action_edit);

        connect(action_del, SIGNAL(triggered()), this, SLOT(deleteItem()));
        connect(action_edit, SIGNAL(triggered()), this, SLOT(editItem()));
    }
    connect(action_add, SIGNAL(triggered()), this, SLOT(addItem()));
    menu->exec(view->mapToGlobal(pos));
    */
}

void TabItemPlanning::deleteItem(){
    /*
    QModelIndex index = view->selectionModel()->currentIndex();
    index = index.sibling(index.row(), 0); // ici je force le n° de colonne à 0, pour etre sur le premier champ
    if (index.row() != -1){
        int id_room = proxyModel->data(index).toInt();
        QSqlQuery *req = new QSqlQuery();
        QSqlQuery *req2 = new QSqlQuery();
        req->prepare("DELETE FROM haveequipment WHERE id_room = :id_room");
        req->bindValue("id_room", id_room);

        req2->prepare("DELETE FROM room WHERE id = :id_room");
        req2->bindValue("id_room", id_room);

        int rep = QMessageBox::question(this, "Confirmation", "Etes-vous sûr de vouloir supprimer cette salle ?", QMessageBox::Yes | QMessageBox::No);
        if(rep == QMessageBox::Yes){

            if (req->exec() && req2->exec()){
                refreshList();
                QMessageBox::information(this, "Requête exécutée avec succès !", "La salle a été supprimé de la base de données !");
            }
            else
                QMessageBox::warning(this, "Erreur !", "La requête n'a pas pu être exécutée ! \nDes réunions peut-être rattachées à cette salle ...");
        }
    }*/
}

void TabItemPlanning::editItem(){/*
    QModelIndex index = view->selectionModel()->currentIndex();
    index = index.sibling(index.row(), 0); // ici je force le n° de colonne à 0, pour etre sur le premier champ
    if (index.row() != -1){
        int id_room = proxyModel->data(index).toInt();

        TabItemRoomActions *roomEdit = new TabItemRoomActions(EDIT, this, id_room);
        connect(roomEdit, SIGNAL(notifyRefreshList()), this, SLOT(refreshList()));
        roomEdit->exec();
    }*/
}

void TabItemPlanning::addItem(){
    /*
    TabItemRoomActions *roomAdd = new TabItemRoomActions(ADD, this);
    connect(roomAdd, SIGNAL(notifyRefreshList()), this, SLOT(refreshList()));
    roomAdd->exec();
    */
}

/*
void TabItemPlanning::resizeEvent(QResizeEvent * event)
{
        int largeur = event->size().width()/model->columnCount();

        for(int i = 0; i < model->columnCount(); i++)
                view->setColumnWidth(i, largeur);
}*/

void TabItemPlanning::addMeeting(){
    MeetingActions *meeting = new MeetingActions(ADD, this, user_id);
    connect(meeting, SIGNAL(notifyRefreshList()), this, SLOT(refreshList()));
    meeting->exec();
}
