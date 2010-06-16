#include "MeetingDetails.h"

MeetingDetails::MeetingDetails(int _user_id, int _meeting_id, QWidget *parent) : QDialog(parent)
{
    user_id = _user_id;
    meeting_id = _meeting_id;

    setWindowTitle("Détails de la réunion");

    QSqlQuery *req = new QSqlQuery();
    req->prepare("SELECT * FROM meeting m WHERE m.meeting_id = :meeting_id");
    req->bindValue(":meeting_id", meeting_id);
    req->exec();
    QSqlRecord rec = req->record();
    req->next();

    lb_label = new QLabel(req->value(rec.indexOf("meeting_label")).toString());

    QDateTime datetime_begin = QDateTime::fromString(req->value(rec.indexOf("meeting_begin")).toString(), "yyyy-MM-dd hh:mm");
    QDateTime datetime_end = QDateTime::fromString(req->value(rec.indexOf("meeting_end")).toString(), "yyyy-MM-dd hh:mm");

    lb_begin = new QLabel(datetime_begin.toString("dd/MM/yyyy hh:mm"));
    lb_end = new QLabel(datetime_end.toString("dd/MM/yyyy hh:mm"));


    QTime duration(datetime_end.time().hour() - datetime_begin.time().hour(), datetime_end.time().minute() - datetime_begin.time().minute(), 0, 0);
    te_duration = new QTimeEdit();
    te_duration->setDisplayFormat("hh:mm");
    te_duration->setTime(duration);
    te_duration->setMaximumTime(duration);
    te_duration->setDisabled(true);

    btn_editDuration = new QPushButton("MAJ");
    btn_editDuration->setDisabled(true);
    btn_editDuration->setToolTip("Seul les organisateurs de la réunion peuvent modifier la durée de la réunion.");

    QHBoxLayout *hbl_duration = new QHBoxLayout();
    hbl_duration->addWidget(te_duration);
    hbl_duration->addWidget(btn_editDuration);

    QWidget *widget_duration = new QWidget();
    widget_duration->setLayout(hbl_duration);

    btn_unavailable = new QPushButton("Ne sera pas présent");
    btn_action = new QPushButton("Ok");
    btn_cancel = new QPushButton("&Annuler");
    btn_del = new QPushButton("Supprimer cette réunion");
    btn_del->setDisabled(true);
    btn_del->setToolTip("Seul les organisteurs peuvent supprimer la réunion.");

    if(req->value(rec.indexOf("meeting_compulsary")).toInt() == 1)
    {
        btn_unavailable->setDisabled(true);
        btn_unavailable->setToolTip("Cette réunion est obligatoire.");
    }

    cb_users = new QComboBox();

    req->prepare("SELECT * FROM meeting m INNER JOIN havemeeting hm ON m.meeting_id = hm.meeting_id INNER JOIN user u ON u.user_id = hm.user_id WHERE m.meeting_id = :meeting_id ORDER BY hm.hm_state DESC, user_surname, user_name");
    req->bindValue(":meeting_id", meeting_id);
    req->exec();
    rec = req->record();
    while(req->next()){
        QIcon icon;
        if(req->value(rec.indexOf("hm_state")).toInt() == -1)
        {
            cb_users->addItem(QIcon(":not_attend.png"), req->value(rec.indexOf("user_surname")).toString() + " " + req->value(rec.indexOf("user_name")).toString(), req->value(rec.indexOf("user_id")).toInt());
        }
        else if(req->value(rec.indexOf("hm_state")).toInt() == 1)
        {
            cb_users->addItem(QIcon(":attend.png"), req->value(rec.indexOf("user_surname")).toString() + " " + req->value(rec.indexOf("user_name")).toString(), req->value(rec.indexOf("user_id")).toInt());
        }
        else
        {
            cb_users->addItem(QIcon(":may_attend.png"), req->value(rec.indexOf("user_surname")).toString() + " " + req->value(rec.indexOf("user_name")).toString(), req->value(rec.indexOf("user_id")).toInt());
        }
    }

    cb_organizer = new QComboBox();
    req->prepare("SELECT * FROM organizemeeting om INNER JOIN user u ON om.user_id = u.user_id WHERE om.meeting_id = :meeting_id ORDER BY user_surname, user_name");
    req->bindValue(":meeting_id", meeting_id);
    req->exec();
    rec = req->record();
    while(req->next())
    {
        cb_organizer->addItem(req->value(rec.indexOf("user_surname")).toString() + " " + req->value(rec.indexOf("user_name")).toString());
    }

    cb_equipments = new QComboBox();
    req->prepare("SELECT * FROM meeting m INNER JOIN room r ON r.room_id = m.room_id INNER JOIN haveequipment he ON he.room_id = r.room_id INNER JOIN equipment e ON he.equip_id = e.equip_id WHERE m.meeting_id = :meeting_id ORDER BY equip_name");
    req->bindValue(":meeting_id", meeting_id);
    req->exec();
    rec = req->record();
    while(req->next()){
        cb_equipments->addItem(req->value(rec.indexOf("equip_name")).toString(), req->value(rec.indexOf("equip_id")).toInt());
    }

    lb_room = new QLabel();
    req->first();
    lb_room->setText(req->value(rec.indexOf("room_name")).toString());

    QFormLayout *fl_data = new QFormLayout;
    fl_data->addRow("Libellé:", lb_label);
    fl_data->addRow("Début:", lb_begin);
    fl_data->addRow("Fin:", lb_end);
    fl_data->addRow("Durée:", widget_duration);
    fl_data->addRow("Organisateur(s):", cb_organizer);
    fl_data->addRow("Invité(s):", cb_users);
    fl_data->addRow("Salle:", lb_room);
    fl_data->addRow("Equipement(s):", cb_equipments);



    req->prepare("SELECT * FROM organizemeeting om INNER JOIN meeting m ON om.meeting_id = m.meeting_id WHERE user_id = :user_id AND m.meeting_id = :meeting_id");
    req->bindValue(":user_id", user_id);
    req->bindValue(":meeting_id", meeting_id);
    req->exec();
    rec = req->record();
    if(req->first())
    {
        btn_unavailable->setDisabled(true);
        btn_unavailable->setToolTip("En tant qu'organisateur, vous êtes contraint d'assister à cette réunion.");

        te_duration->setDisabled(false);
        btn_editDuration->setDisabled(false);
        btn_editDuration->setToolTip("Cette action sera irréversible.");

        connect(btn_editDuration, SIGNAL(clicked()), this, SLOT(editDuration()));

        QDateTime meeting_begin = QDateTime::fromString(req->value(rec.indexOf("meeting_begin")).toString(), "yyyy-MM-dd hh:mm");

        if(QDateTime::currentDateTime().daysTo(meeting_begin) <= 7 && req->value(rec.indexOf("meeting_periodic")).toInt() == 0)
        {
            btn_del->setToolTip("Impossible de supprimer le rendez-vous moins d'une semaine avant.");
        }
        else
        {
            btn_del->setDisabled(false);
            btn_del->setToolTip("Ceci est une action irréversible.");
        }

        connect(btn_del, SIGNAL(clicked()), this, SLOT(deleteMeeting()));
    }


    QHBoxLayout *layout_buttons = new QHBoxLayout;
    layout_buttons->setAlignment(Qt::AlignRight);
    layout_buttons->addWidget(btn_unavailable);
    layout_buttons->addWidget(btn_del);
    layout_buttons->addWidget(btn_action);
    layout_buttons->addWidget(btn_cancel);

    QVBoxLayout *layout_main = new QVBoxLayout;
    layout_main->addLayout(fl_data);
    layout_main->addLayout(layout_buttons);

    setLayout(layout_main);

    connect(btn_cancel, SIGNAL(clicked()), this, SLOT(reject()));
    connect(btn_action, SIGNAL(clicked()), this, SLOT(accept()));
    connect(btn_unavailable, SIGNAL(clicked()), this, SLOT(canceledMeeting()));

}

void MeetingDetails::canceledMeeting(){
    int rep = QMessageBox::question(this, "Confirmation", "Etes-vous sûr de ne pas assister à cette réunion ?", QMessageBox::Yes | QMessageBox::No);
    if(rep == QMessageBox::Yes){
        QSqlQuery *req = new QSqlQuery();
        req->prepare("UPDATE havemeeting SET hm_state = :state WHERE user_id = :user_id AND meeting_id = :meeting_id");
        req->bindValue(":state", -1);
        req->bindValue(":user_id", user_id);
        req->bindValue(":meeting_id", meeting_id);
        if(req->exec())
        {
            emit notifyRefreshList();
            QMessageBox::information(this, "Requête exécutée avec succès !", "Vous ne serez pas à la réunion.");
            accept();
        }
    }
}

void MeetingDetails::deleteMeeting(){
    QSqlQuery *req = new QSqlQuery();
    req->prepare("SELECT meeting_periodic FROM meeting WHERE meeting_id = :meeting_id AND meeting_periodic > 0");
    req->bindValue(":meeting_id", meeting_id);
    req->exec();
    int rep;
    if(req->first())
        rep = QMessageBox::question(this, "Confirmation", "Ceci est une réunion périodique, êtes-vous certain de supprimer toutes ces réunions ?", QMessageBox::Yes | QMessageBox::No);
    else
        rep = QMessageBox::question(this, "Confirmation", "Etes-vous sûr de supprimer cette réunion ?", QMessageBox::Yes | QMessageBox::No);

    if(rep == QMessageBox::Yes){
        bool ok = true;
        req->prepare("DELETE FROM havemeeting WHERE meeting_id = :meeting_id");
        req->bindValue(":meeting_id", meeting_id);
        if(!req->exec()) ok = false;

        req->prepare("DELETE FROM organizemeeting WHERE meeting_id = :meeting_id");
        req->bindValue(":meeting_id", meeting_id);
        if(!req->exec()) ok = false;

        req->prepare("DELETE FROM meeting WHERE meeting_id = :meeting_id");
        req->bindValue(":meeting_id", meeting_id);
        if(!req->exec()) ok = false;

        if(ok)
        {
            emit notifyRefreshList();
            QMessageBox::information(this, "Requête exécutée avec succès !", "Réunion annulée.");
            accept();
        }
        else{
            QMessageBox::warning(this, "Erreur !", "La requête n'a pas pu être exécutée !");
        }
    }
}

void MeetingDetails::editDuration()
{
    int nb_quarter = te_duration->time().hour() * 4;
    nb_quarter += te_duration->time().minute() / 15;

    if(nb_quarter != 0)
    {

        QSqlQuery *req = new QSqlQuery();
        req->prepare("SELECT meeting_begin FROM meeting WHERE meeting_id = :meeting_id");
        req->bindValue(":meeting_id", meeting_id);
        req->exec();
        req->first();

        QDateTime meeting_end = QDateTime::fromString(req->value(0).toString(), "yyyy-MM-dd hh:mm");
        meeting_end.setTime(QTime(meeting_end.time().hour() + nb_quarter / 4, meeting_end.time().minute() + (nb_quarter % 4) * 15, 0, 0));

        te_duration->setTime(QTime(nb_quarter / 4, (nb_quarter % 4) * 15));

        req->prepare("UPDATE meeting SET meeting_end = :meeting_end WHERE meeting_id = :meeting_id");
        req->bindValue(":meeting_id", meeting_id);
        req->bindValue(":meeting_end", meeting_end.toString("yyyy-MM-dd hh:mm"));

        if(req->exec())
        {
            lb_end->setText(meeting_end.toString("dd/MM/yyyy hh:mm"));
            emit notifyRefreshList();
            QMessageBox::information(this, "Requête exécutée avec succès !", "La réunion se terminera à " + lb_end->text());
        }
        else
        {
            QMessageBox::warning(this, "Erreur !", "La requête n'a pas pu être exécutée !");
        }
    }
    else
    {
        QMessageBox::warning(this, "Erreur !", "Durée nulle impossible.");
    }
}
