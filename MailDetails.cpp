#include "MailDetails.h"

MailDetails::MailDetails(int user_id_from, int user_id_to, QString date, QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Détails du mail");

    QSqlQuery *req = new QSqlQuery();
    req->prepare("SELECT ufrom.user_surname AS user_from_surname, ufrom.user_name AS user_from_name, uto.user_surname AS user_to_surname, uto.user_name AS user_to_name, m.msg_date, m.msg_subject, m.msg_text FROM message m INNER JOIN user ufrom ON m.user_id_from = ufrom.user_id INNER JOIN user uto ON m.user_id_to = uto.user_id WHERE m.user_id_from = :user_id_from AND m.user_id_to = :user_id_to AND m.msg_date = :msg_date");
    req->bindValue(":user_id_from", user_id_from);
    req->bindValue(":user_id_to", user_id_to);
    req->bindValue(":msg_date", date);
    req->exec();
    QSqlRecord rec = req->record();
    req->next();

    lb_from = new QLabel(req->value(rec.indexOf("user_from_surname")).toString() + " " + req->value(rec.indexOf("user_from_name")).toString());
    lb_to = new QLabel(req->value(rec.indexOf("user_to_surname")).toString() + " " + req->value(rec.indexOf("user_to_name")).toString());
    lb_date = new QLabel(QDateTime::fromString(req->value(rec.indexOf("msg_date")).toString(), "yyyy-MM-dd hh:mm").toString("dd/MM/yyyy à hh:mm"));
    lb_subject = new QLabel(req->value(rec.indexOf("msg_subject")).toString());
    te_text = new QTextEdit(req->value(rec.indexOf("msg_text")).toString());
    te_text->setEnabled(false);


    QFormLayout *fl_data = new QFormLayout;
    fl_data->addRow("De:", lb_from);
    fl_data->addRow("A:", lb_to);
    fl_data->addRow("Le:", lb_date);
    fl_data->addRow("Objet:", lb_subject);
    fl_data->addRow("Message:", te_text);

    btn_cancel = new QPushButton("&Fermer");

    QHBoxLayout *layout_buttons = new QHBoxLayout;
    layout_buttons->setAlignment(Qt::AlignRight);
    layout_buttons->addWidget(btn_cancel);

    QVBoxLayout *layout_main = new QVBoxLayout;
    layout_main->addLayout(fl_data);
    layout_main->addLayout(layout_buttons);

    setLayout(layout_main);

    connect(btn_cancel, SIGNAL(clicked()), this, SLOT(accept()));
}
