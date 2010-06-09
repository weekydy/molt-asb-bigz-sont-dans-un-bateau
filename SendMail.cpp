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


        QFormLayout *fl_data = new QFormLayout;
        fl_data->addRow("A:", lbl_to);
        fl_data->addRow("Message:", te_message);


        QVBoxLayout *layout_main = new QVBoxLayout;
        layout_main->addLayout(fl_data);
        layout_main->addLayout(layout_buttons);

        setLayout(layout_main);

        connect(btn_cancel, SIGNAL(clicked()), this, SLOT(reject()));
        connect(btn_action, SIGNAL(clicked()), this, SLOT(makeAction()));
    }
}

void SendMail::makeAction(){
    if(user_id_to != -1){

        QString missingFields("");
        if(te_message->toPlainText() == "") missingFields += "Message ; ";

        if (missingFields == ""){ // Si tout a été saisi

            QSqlQuery *req = new QSqlQuery();

            req->prepare("INSERT INTO message VALUES(:user_id_from, :user_id_to, :date, :message)");
            qDebug() << user_id_from << " " << user_id_to << endl;
            req->bindValue(":user_id_from", user_id_from);
            req->bindValue(":user_id_to", user_id_to);
            req->bindValue(":date", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:ss"));
            req->bindValue(":message", te_message->toPlainText());

            if(req->exec()){
                //emit notifyRefreshList();
                QMessageBox::information(this, "Requête exécutée avec succès !", "L'équipement a été ajouté dans la base de données !");
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
}
