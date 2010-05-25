#include "SubscribeWidget.h"

SubscribeWidget::SubscribeWidget(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Ajouter un utilisateur");


    le_surname = new QLineEdit();
    le_surname->setMaxLength(20);
    le_name = new QLineEdit();
    le_name->setMaxLength(20);
    le_nickname = new QLineEdit();
    le_nickname->setMaxLength(20);
    le_password = new QLineEdit();
    le_password->setMaxLength(20);
    le_password->setEchoMode(QLineEdit::Password);
    le_password_bis = new QLineEdit();
    le_password_bis->setMaxLength(20);
    le_password_bis->setEchoMode(QLineEdit::Password);

    QFormLayout *fl_data = new QFormLayout;
    fl_data->addRow("Prénom:", le_name);
    fl_data->addRow("Nom:", le_surname);
    fl_data->addRow("Identifiant:", le_nickname);
    fl_data->addRow("Mot de passe:", le_password);
    fl_data->addRow("Mot de passe (confirmation):", le_password_bis);

    btn_cancel = new QPushButton("&Annuler");
    btn_save = new QPushButton("&Créer mon compte");


    QHBoxLayout *layout_buttons = new QHBoxLayout;
    layout_buttons->setAlignment(Qt::AlignRight);
    layout_buttons->addWidget(btn_save);
    layout_buttons->addWidget(btn_cancel);

    QVBoxLayout *layout_main = new QVBoxLayout;
    layout_main->addLayout(fl_data);
    layout_main->addLayout(layout_buttons);

    setLayout(layout_main);

    connect(btn_cancel, SIGNAL(clicked()), this, SLOT(reject()));
    connect(btn_save, SIGNAL(clicked()), this, SLOT(subscribe()));

}

void SubscribeWidget::subscribe(){
    QString missingFields("");
    if(le_surname->text() == "") missingFields += "Prénom ; ";
    if(le_name->text() == "") missingFields += "Nom ; ";
    if(le_nickname->text() == "") missingFields += "Identifiant ; ";
    if(le_password->text() == "") missingFields += "Mot de passe ; ";
    if(le_password_bis->text() == "") missingFields += "Mot de passe (confirmation) ; ";

    if (missingFields == ""){ // Si tout a été saisi
        if(le_password->text() == le_password_bis->text()){

            QSqlQuery *req = new QSqlQuery;
            req->prepare("SELECT user_name, user_surname FROM user WHERE user_name = :name AND user_surname = :surname");
            req->bindValue(":surname", le_surname->text());
            req->bindValue(":name", le_name->text());
            req->exec();

            // si il n'y a pas d'entrée similaire
            if(!req->next()){
                req->prepare("INSERT INTO user(user_name, user_surname, user_nickname, user_password, user_access) VALUES (:name, :surname, :nickname, :password, :access)");
                req->bindValue(":name", le_name->text());
                req->bindValue(":surname", le_surname->text());
                req->bindValue(":nickname", le_nickname->text());
                req->bindValue(":password", le_password->text());
                req->bindValue(":access", 1);

                if(req->exec()){
                    QMessageBox::information(this, "Requête exécutée avec succès !", "L'utilisateur a été ajouté dans la base de données !");
                    accept();
                }
                else{
                    QMessageBox::warning(this, "Erreur !", "La requête n'a pas pu être exécutée !");
                }
            }
            else{
                QMessageBox::warning(this, "Erreur !", "La personne existe déjà dans la base de données !");
            }
        }
        else{
            QMessageBox::warning(this, "Action Impossible", "Les mots de passe ne sont pas identiques");
        }
    }
    else{
        QMessageBox::warning(this, "Action Impossible", "Veuillez remplir les champs vides :\n"+missingFields);
    }
}
