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

    btn_edit_info = new QPushButton("Enregistrer profil");
    btn_edit_password = new QPushButton("Enregistrer mot de passe");

    QHBoxLayout *layout_buttons = new QHBoxLayout;
    layout_buttons->setAlignment(Qt::AlignRight);
    layout_buttons->addWidget(btn_edit_info);
    layout_buttons->addWidget(btn_edit_password);

    QFormLayout *fl_data = new QFormLayout();
    fl_data->addRow("Nom:", le_surname);
    fl_data->addRow("Prénom:", le_name);
    fl_data->addRow("Identifiant:", le_nickname);
    fl_data->addRow("Mot de passe:", le_pass);
    fl_data->addRow("Nouveau mot de passe:", le_new_pass);
    fl_data->addRow("Nouveau mot de passe (confirmation):", le_new_pass2);

    QGroupBox *gb_info = new QGroupBox("Informations");
    gb_info->setLayout(fl_data);


    lw_left = new ListWidget(this);
    lw_right = new ListWidget(this);

    /*QSqlQuery *req = new QSqlQuery();
    req->prepare("SELECT * FROM user u INNER JOIN belongtogroup bg ON u.user_id = bg.user_id INNER JOIN grp g ON g.grp_id = bg.grp_id");
    req->bindValue(":user_id", user_id);
    req->exec();
    QSqlRecord rec = req->record();
    req->first();*/


    btn_to_left = new QPushButton(this);
    btn_to_right = new QPushButton(this);    
    btn_to_left->setIcon(QIcon(":to_left.png"));
    btn_to_right->setIcon(QIcon(":to_right.png"));
    btn_to_left->setFixedSize(30, 30);
    btn_to_right->setFixedSize(30, 30);

    QVBoxLayout *layout_buttons_drag_drop = new QVBoxLayout();
    layout_buttons_drag_drop->addStretch();
    layout_buttons_drag_drop->addWidget(btn_to_left);
    layout_buttons_drag_drop->addWidget(btn_to_right);
    layout_buttons_drag_drop->addStretch();

    QHBoxLayout *layout_group = new QHBoxLayout();
    layout_group->addWidget(lw_left);
    layout_group->addLayout(layout_buttons_drag_drop);
    layout_group->addWidget(lw_right);

    QGroupBox *gb_group = new QGroupBox("Groupe(s)");
    gb_group->setLayout(layout_group);

    QVBoxLayout *layout_main = new QVBoxLayout();
    layout_main->addWidget(gb_info);
    layout_main->addWidget(gb_group);
    layout_main->addLayout(layout_buttons);

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
    }
}
