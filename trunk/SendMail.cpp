#include "SendMail.h"

SendMail::SendMail(int _user_id_from, int _user_id_to, QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Envoyer un message");

    user_id_from = _user_id_from;
    user_id_to = _user_id_to;

    btn_cancel = new QPushButton("&Annuler");
    btn_action = new QPushButton("Envoyer");

    QHBoxLayout *layout_buttons = new QHBoxLayout;
    layout_buttons->setAlignment(Qt::AlignRight);
    layout_buttons->addWidget(btn_action);
    layout_buttons->addWidget(btn_cancel);

    if(user_id_to != -1) // réponse à une personne
    {
        QSqlQuery *req = new QSqlQuery;
        req->prepare("SELECT user_name, user_surname FROM user WHERE user_id = :user_id_to");
        req->bindValue(":user_id_to", user_id_to);
        req->exec();
        req->first();
        lbl_to = new QLabel(req->value(0).toString() + " " + req->value(1).toString());
        te_message = new QTextEdit();
        le_subject = new QLineEdit();


        QFormLayout *fl_data = new QFormLayout;
        fl_data->addRow("A:", lbl_to);
        fl_data->addRow("Objet:", le_subject);
        fl_data->addRow("Message:", te_message);


        QVBoxLayout *layout_main = new QVBoxLayout;
        layout_main->addLayout(fl_data);
        layout_main->addLayout(layout_buttons);

        setLayout(layout_main);


    }
    else{
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

        te_message = new QTextEdit();
        le_subject = new QLineEdit();

        QVBoxLayout *layout_right = new QVBoxLayout();
        layout_right->addWidget(lb_user_to);
        layout_right->addWidget(lw_targets);


        QVBoxLayout *layout_users_to = new QVBoxLayout();
        layout_users_to->addWidget(lb_group);
        layout_users_to->addWidget(lw_groups);
        layout_users_to->addWidget(lb_user);
        layout_users_to->addWidget(lw_users);

        QHBoxLayout *layout_widget_group = new QHBoxLayout();
        layout_widget_group->addLayout(layout_users_to);
        layout_widget_group->addLayout(layout_right);

        QWidget *users_to = new QWidget();
        users_to->setLayout(layout_widget_group);

        QFormLayout *fl_data = new QFormLayout;
        fl_data->addRow("A:", users_to);
        fl_data->addRow("Objet:", le_subject);
        fl_data->addRow("Message:", te_message);


        QVBoxLayout *layout_main = new QVBoxLayout;
        layout_main->addLayout(fl_data);
        layout_main->addLayout(layout_buttons);


        setLayout(layout_main);
    }

    connect(btn_cancel, SIGNAL(clicked()), this, SLOT(reject()));
    connect(btn_action, SIGNAL(clicked()), this, SLOT(makeAction()));
}

void SendMail::makeAction(){
    if(user_id_to != -1){

        QString missingFields("");
        if(te_message->toPlainText() == "") missingFields += "Message ; ";
        if(le_subject->text() == "") missingFields += "Sujet ; ";

        if (missingFields == ""){ // Si tout a été saisi

            QSqlQuery *req = new QSqlQuery();

            req->prepare("INSERT INTO message VALUES(:user_id_from, :user_id_to, :date, :from, :subject, :message)");
            req->bindValue(":user_id_from", user_id_from);
            req->bindValue(":user_id_to", user_id_to);
            req->bindValue(":date", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:ss"));
            req->bindValue(":from", 1);
            req->bindValue(":subject", le_subject->text());
            req->bindValue(":message", te_message->toPlainText());

            if(req->exec()){
                req->bindValue(":from", 0);
                req->exec();

                emit notifyRefreshList();
                QMessageBox::information(this, "Requête exécutée avec succès !", "Le message a été envoyé !");
                accept();
            }
            else{
                QMessageBox::warning(this, "Erreur !", "La requête n'a pas pu être exécutée !");
            }
        }
        else{
            QMessageBox::warning(this, "Action Impossible", "Veuillez remplir les champs vides :\n"+missingFields);
        }
    }
    else
    {
        QString missingFields("");
        if(te_message->toPlainText() == "") missingFields += "Message ; ";
        if(le_subject->text() == "") missingFields += "Sujet ; ";
        if(lw_targets->count() == 0) missingFields += "Destinataire(s) ;";

        if (missingFields == ""){ // Si tout a été saisi
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

            if(list_users_id.contains(user_id_from))
                list_users_id.remove(user_id_from);

            QString message(te_message->toPlainText());
            QString subject = "";
            if(list_users_id.size() > 1)
                subject += "[Message groupé] ";
            subject += le_subject->text();

            foreach (int user_id_to, list_users_id)
            {
                req->prepare("INSERT INTO message VALUES(:user_id_from, :user_id_to, :msg_date, :msg_from, :msg_subject, :msg_text)");
                req->bindValue(":user_id_from", user_id_from);
                req->bindValue(":user_id_to", user_id_to);
                req->bindValue(":msg_date", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm"));
                req->bindValue(":msg_from", 1);
                req->bindValue(":msg_subject", subject);
                req->bindValue(":msg_text", message);

                req->exec();
                req->bindValue(":msg_from", 0);
                req->exec();
            }
            emit notifyRefreshList();
            QMessageBox::information(this, "Requête exécutée avec succès !", "Le message a été envoyé !");            
            accept();
        }
        else{
            QMessageBox::warning(this, "Action Impossible", "Veuillez remplir les champs vides :\n"+missingFields);
        }
    }
}

void SendMail::moveCurrentItem(ListWidget *source, ListWidget *target)
{
    if (source->currentItem()) {
        QListWidgetItem *newItem = source->currentItem()->clone();
        target->addItem(newItem);
        target->setCurrentItem(newItem);
        delete source->currentItem();
    }
}
