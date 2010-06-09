#ifndef SEND_MAIL_H
#define SEND_MAIL_H

#include <QtGui>
#include <QtCore>
#include <QtSql>

#include "Commons.h"

class SendMail : public QDialog
{
Q_OBJECT
public:
    SendMail(int user_id_from, int user_id_to, QWidget *parent = 0);

private:
    int action;
    int user_id_from;
    int user_id_to;

    QLabel *lbl_to;
    QTextEdit *te_message;

    QPushButton *btn_action;
    QPushButton *btn_cancel;

private slots:
    void makeAction();

signals:
    void notifyRefreshList();
};

#endif // TabItemRoomActions_H
