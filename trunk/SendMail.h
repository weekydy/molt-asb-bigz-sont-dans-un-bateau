#ifndef SEND_MAIL_H
#define SEND_MAIL_H

#include <QtGui>
#include <QtCore>
#include <QtSql>

#include "ListWidget.h"
#include "Commons.h"

class SendMail : public QDialog
{
Q_OBJECT
public:
    SendMail(int user_id_from, int user_id_to, QString _subject = "", QWidget *parent = 0);

private:
    int action;
    int user_id_from;
    int user_id_to;
    QString subject;

    QLabel *lbl_to;
    QLineEdit *le_subject;
    QTextEdit *te_message;

    QPushButton *btn_action;
    QPushButton *btn_cancel;

    ListWidget *lw_groups;
    ListWidget *lw_users;
    ListWidget *lw_targets;

    void moveCurrentItem(ListWidget *source, ListWidget *target);

private slots:
    void makeAction();

signals:
    void notifyRefreshList();
};

#endif // TabItemRoomActions_H
