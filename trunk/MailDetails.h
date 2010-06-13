#ifndef MAILDETAILS_H
#define MAILDETAILS_H

#include <QtGui>
#include <QtCore>
#include <QtSql>

#include "Commons.h"

class MailDetails : public QDialog
{
Q_OBJECT
public:
    MailDetails(int user_id_from, int user_id_to, QString date, QWidget *parent = 0);

private:

    QLabel *lb_from;
    QLabel *lb_to;
    QLabel *lb_date;
    QLabel *lb_subject;

    QTextEdit *te_text;

    QComboBox *cb_users;

    QPushButton *btn_cancel;

private slots:

signals:

};

#endif
