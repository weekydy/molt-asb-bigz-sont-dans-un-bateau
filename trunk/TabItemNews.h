#ifndef TABITEMNEWS_H
#define TABITEMNEWS_H

#include <QtCore>
#include <QtGui>
#include <QtSql>

#include "Commons.h"
#include "SendMail.h"

class TabItemNews : public QWidget
{
Q_OBJECT
public:
    TabItemNews(int user_id, QWidget *parent = 0);

private:
    int user_id;

    QLabel *lb_message_event;
    QTableWidget *table;

    QLabel *lb_message_mail;

    QTabWidget *tab_mail;

    QSqlQueryModel *model_in;
    QTableView *view_in;
    QSortFilterProxyModel *proxyModel_in;

    QSqlQueryModel *model_out;
    QTableView *view_out;
    QSortFilterProxyModel *proxyModel_out;

    QPushButton *btn_add;
    QPushButton *btn_add2;
    QPushButton *btn_reply;

private slots:
    void refreshListEvent();
    void refreshListMail();
    void acceptMeeting(int id_meeting);
    void declineMeeting(int id_meeting);
    void refreshButtonState(QModelIndex index);
    void replyMail();
    void sendMail();
};

#endif // TABITEMROOM_H
