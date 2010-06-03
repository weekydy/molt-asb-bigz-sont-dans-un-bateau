#ifndef TABITEMNEWS_H
#define TABITEMNEWS_H

#include <QtCore>
#include <QtGui>
#include <QtSql>

#include "Commons.h"

class TabItemNews : public QWidget
{
Q_OBJECT
public:
    TabItemNews(int user_id, QWidget *parent = 0);

private:
    int id;

    QLabel *lb_message;
    QTableWidget *table;

private slots:
    void refreshList();
    void acceptMeeting();

};

#endif // TABITEMROOM_H
