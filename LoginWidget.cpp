#include "LoginWidget.h"

LoginWidget::LoginWidget(QWidget *parent) : QWidget(parent)
{
    setFixedSize(220, 160);

    le_id = new QLineEdit();
    le_id->setMaxLength(20);
    le_id->setFixedWidth(100);
    le_id->setText("agavigne");

    le_pass = new QLineEdit();
    le_pass->setEchoMode(QLineEdit::Password);
    le_pass->setMaxLength(20);
    le_pass->setFixedWidth(100);
    le_pass->setText("asb");

    button_connect = new QPushButton("Valider");
    button_connect->setFixedWidth(50);

    button_subscribe = new QPushButton("Nouveau");
    button_subscribe->setFixedSize(50, 15);

    label_msg = new QLabel();

    QFormLayout *fl_log = new QFormLayout;
    fl_log->addRow("Identifiant:", le_id);
    fl_log->addRow("Mot de passe:", le_pass);
    fl_log->addWidget(label_msg);
    fl_log->addWidget(button_connect);

    QGroupBox *gb_log = new QGroupBox("Connexion");
    gb_log->setLayout(fl_log);

    QVBoxLayout *layout_main = new QVBoxLayout();
    layout_main->setSizeConstraint(QLayout::SetMinimumSize);
    layout_main->addWidget(gb_log);
    layout_main->addWidget(button_subscribe, Qt::AlignRight);

    setLayout(layout_main);

    connect(button_connect, SIGNAL(clicked()), this, SLOT(connection()));
    connect(button_subscribe, SIGNAL(clicked()), this, SLOT(subscribe()));
    connect(this, SIGNAL(notifyConnected(int,int)), parent, SLOT(updateConnected(int, int)));
}

void LoginWidget::connection(){
    QSqlQuery req;
    req.prepare("SELECT * FROM user WHERE nickname = :nickname AND password = :pass");
    req.bindValue(":nickname", le_id->text());
    req.bindValue(":pass", le_pass->text());
    req.exec();
    QSqlRecord rec = req.record();

    if(req.first()){
        label_msg->setText("Informations valides");
        int id_user = req.value(rec.indexOf("id")).toInt();
        int access = req.value(rec.indexOf("access")).toInt();
        emit notifyConnected(id_user, access);
    }
    else{
        label_msg->setText("Informations invalides");
    }
}

void LoginWidget::subscribe(){
    SubscribeWidget *sw = new SubscribeWidget(this);
    sw->exec();
}

/*void LoginWidget::keyPressEvent(QKeyEvent *keyEvent)
{
    if(keyEvent->key() == Qt::Key_Enter)
        connection();
}*/
