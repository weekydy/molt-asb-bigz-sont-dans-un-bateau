#include "TabItemProfile.h"

TabItemProfile::TabItemProfile(int _user_id, QWidget *parent) : QWidget(parent)
{
    user_id = _user_id;

    QSqlQuery *req = new QSqlQuery();
    req->prepare("SELECT * FROM user WHERE user_id = :user_id");
    req->bindValue(":user_id", user_id);
    req->exec();
    QSqlRecord rec = req->record();
    req->first();

    le_surname = new QLineEdit(req->value(rec.indexOf("user_surname")).toString());
    le_name = new QLineEdit(req->value(rec.indexOf("user_name")).toString());
    le_nickname = new QLineEdit(req->value(rec.indexOf("user_nickname")).toString());

    le_pass = new QLineEdit();
    le_new_pass = new QLineEdit();
    le_new_pass2 = new QLineEdit();
    le_pass->setEchoMode(QLineEdit::Password);
    le_new_pass->setEchoMode(QLineEdit::Password);
    le_new_pass2->setEchoMode(QLineEdit::Password);

    btn_edit_info = new QPushButton("Enregistrer les modifications");
    btn_edit_info->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    btn_edit_password = new QPushButton("Changer mon mot de passe");
    btn_edit_password->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    QFormLayout *fl_info = new QFormLayout();
    fl_info->addRow("Nom:", le_surname);
    fl_info->addRow("Prénom:", le_name);
    fl_info->addRow("Identifiant:", le_nickname);
    fl_info->addWidget(btn_edit_info);

    QGroupBox *gb_info = new QGroupBox("Informations");
    gb_info->setLayout(fl_info);

    QFormLayout *fl_pass = new QFormLayout();
    fl_pass->addRow("Mot de passe:", le_pass);
    fl_pass->addRow("Nouveau mot de passe:", le_new_pass);
    fl_pass->addRow("Nouveau mot de passe (confirmation):", le_new_pass2);
    fl_pass->addWidget(btn_edit_password);

    QGroupBox *gb_pass = new QGroupBox("Changer son mot de passe");
    gb_pass->setLayout(fl_pass);

    QHBoxLayout *layout_top = new QHBoxLayout();
    layout_top->addWidget(gb_info);
    layout_top->addWidget(gb_pass);

    lw_left = new ListWidget(":group.png", this);
    lw_right = new ListWidget(":group_not.png", this);

    req->prepare("SELECT * FROM grp WHERE grp_id NOT IN (SELECT g.grp_id FROM belongtogroup bg INNER JOIN grp g ON g.grp_id = bg.grp_id WHERE bg.user_id = :user_id)");
    req->bindValue(":user_id", user_id);
    req->exec();
    rec = req->record();
    while(req->next()){
        QListWidgetItem *item = new QListWidgetItem();
        item->setData(Qt::UserRole, req->value(rec.indexOf("grp_id")).toInt());
        item->setText(req->value(rec.indexOf("grp_name")).toString());
        lw_left->addItem(item);
    }

    req->prepare("SELECT * FROM belongtogroup bg INNER JOIN grp g ON g.grp_id = bg.grp_id WHERE bg.user_id = :user_id");
    req->bindValue(":user_id", user_id);
    req->exec();
    rec = req->record();
    while(req->next()){
        QListWidgetItem *item = new QListWidgetItem();
        item->setData(Qt::UserRole, req->value(rec.indexOf("grp_id")).toInt());
        item->setText(req->value(rec.indexOf("grp_name")).toString());
        lw_right->addItem(item);
    }

    btn_to_left = new QPushButton(this);
    btn_to_right = new QPushButton(this);
    btn_to_left->setIcon(QIcon(":to_left.png"));
    btn_to_right->setIcon(QIcon(":to_right.png"));
    btn_to_left->setFixedSize(30, 30);
    btn_to_right->setFixedSize(30, 30);
    btn_to_left->setDisabled(true);
    btn_to_right->setDisabled(true);

    QVBoxLayout *layout_buttons_drag_drop = new QVBoxLayout();
    layout_buttons_drag_drop->addStretch();
    layout_buttons_drag_drop->addWidget(btn_to_left);
    layout_buttons_drag_drop->addWidget(btn_to_right);
    layout_buttons_drag_drop->addStretch();

    QHBoxLayout *layout_label = new QHBoxLayout();
    QLabel *lb_left = new QLabel("Groupes disponibles :");
    lb_left->setAlignment(Qt::AlignCenter);
    QLabel *lb_right = new QLabel("Mes groupes :");
    lb_right->setAlignment(Qt::AlignCenter);
    layout_label->addWidget(lb_left);
    layout_label->addWidget(lb_right);

    QHBoxLayout *layout_widget_group = new QHBoxLayout();
    layout_widget_group->addWidget(lw_left);
    layout_widget_group->addLayout(layout_buttons_drag_drop);
    layout_widget_group->addWidget(lw_right);

    QVBoxLayout *layout_group = new QVBoxLayout();
    layout_group->addLayout(layout_label);
    layout_group->addLayout(layout_widget_group);


    QGroupBox *gb_group = new QGroupBox("Groupe(s)");
    gb_group->setLayout(layout_group);

    QVBoxLayout *layout_main = new QVBoxLayout();
    layout_main->addLayout(layout_top);
    layout_main->addWidget(gb_group);

    connect(btn_edit_info, SIGNAL(clicked()), this, SLOT(editInfo()));
    connect(btn_edit_password, SIGNAL(clicked()), this, SLOT(editPassword()));
    connect(btn_to_right, SIGNAL(clicked()), this, SLOT(on_rightButton_clicked()));
    connect(btn_to_left, SIGNAL(clicked()), this, SLOT(on_leftButton_clicked()));
    connect(lw_left, SIGNAL(clicked(QModelIndex)), this, SLOT(refreshButtonStateLeft()));
    connect(lw_right, SIGNAL(clicked(QModelIndex)), this, SLOT(refreshButtonStateRight()));
    connect(lw_right, SIGNAL(notifyNewItem()), this, SLOT(updateGroups()));
    connect(lw_left, SIGNAL(notifyNewItem()), this, SLOT(updateGroups()));

    setLayout(layout_main);
}

void TabItemProfile::on_leftButton_clicked()
{
    moveCurrentItem(lw_right, lw_left);
}

void TabItemProfile::on_rightButton_clicked()
{
    moveCurrentItem(lw_left, lw_right);
}

void TabItemProfile::moveCurrentItem(ListWidget *source, ListWidget *target)
{
    if (source->currentItem()) {
        QListWidgetItem *newItem = source->currentItem()->clone();
        target->addItem(newItem);
        target->setCurrentItem(newItem);
        delete source->currentItem();
        updateGroups();
    }
}

void TabItemProfile::refreshButtonStateLeft(){
    btn_to_left->setDisabled(true);
    btn_to_right->setDisabled(false);
}

void TabItemProfile::refreshButtonStateRight(){
    btn_to_right->setDisabled(true);
    btn_to_left->setDisabled(false);
}

void TabItemProfile::updateGroups(){

    QSqlQuery *req = new QSqlQuery();
    req->prepare("DELETE FROM belongtogroup WHERE user_id = :user_id");
    req->bindValue(":user_id", user_id);
    req->exec();

    qDebug() << "nb row " << lw_right->count();

    for(int i = 0; i < lw_right->count(); i++){

        req->prepare("INSERT INTO belongtogroup VALUES(:grp_id, :user_id)");
        req->bindValue(":grp_id", lw_right->item(i)->data(Qt::UserRole));
        req->bindValue(":user_id", user_id);
        req->exec();
    }
}

void TabItemProfile::editInfo()
{
    QString missingFields("");
    if(le_surname->text() == "") missingFields += "Prénom ; ";
    if(le_name->text() == "") missingFields += "Nom ; ";
    if(le_nickname->text() == "") missingFields += "Identifiant ; ";

    if (missingFields == ""){ // Si tout a été saisi

        QSqlQuery *req = new QSqlQuery;
        req->prepare("SELECT user_name, user_surname FROM user WHERE (user_name = :name AND user_surname = :surname) OR user_nickname = :nickname AND user_id <> :user_id");
        req->bindValue(":user_id", user_id);
        req->bindValue(":surname", le_surname->text());
        req->bindValue(":name", le_name->text());
        req->bindValue(":nickname", le_nickname->text());
        req->exec();

        // si il n'y a pas d'entrée similaire
        if(!req->next()){
            req->prepare("UPDATE user SET user_surname = :surname, user_name = :name, user_nickname = :nickname WHERE user_id = :user_id");
            req->bindValue(":user_id", user_id);
            req->bindValue(":name", le_name->text());
            req->bindValue(":surname", le_surname->text());
            req->bindValue(":nickname", le_nickname->text());

            if(req->exec()){
                QMessageBox::information(this, "Requête exécutée avec succès !", "Les modifications ont été prise en compte !");
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
        QMessageBox::warning(this, "Action Impossible", "Veuillez remplir les champs vides :\n"+missingFields);
    }
}

void TabItemProfile::editPassword()
{
    QString missingFields("");
    if(le_pass->text() == "") missingFields += "Mot de passe ; ";
    if(le_new_pass->text() == "") missingFields += "Nouveau mot de passe ; ";
    if(le_new_pass2->text() == "") missingFields += "Nouveau mot de passe (confirmation) ; ";

    if (missingFields == ""){ // Si tout a été saisi
        if(le_new_pass->text() == le_new_pass2->text()){
            QSqlQuery *req = new QSqlQuery;
            req->prepare("SELECT * FROM user WHERE user_password = :password AND user_id = :user_id");
            req->bindValue(":user_id", user_id);
            req->bindValue(":password", le_pass->text());
            req->exec();
            if(req->next()){
                req->prepare("UPDATE user SET user_password = :password WHERE user_id = :user_id");
                req->bindValue(":password", le_new_pass->text());
                req->bindValue(":user_id", user_id);

                if(req->exec()){
                    QMessageBox::information(this, "Requête exécutée avec succès !", "Mot de passe modifié !");
                }
                else{
                    QMessageBox::warning(this, "Erreur !", "La requête n'a pas pu être exécutée !");
                }
            }
            else{
                QMessageBox::warning(this, "Erreur !", "Mot de passe actuel incorrect !");
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
