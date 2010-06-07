#ifndef TABITEMUSER_H
#define TABITEMUSER_H

#include <QtCore>
#include <QtGui>
#include <QtSql>

#include "Commons.h"

class TabItemUser : public QWidget
{
Q_OBJECT
public:
    TabItemUser(int _user_id, QWidget *parent = 0);

private:
    int user_id;
    QTableWidget *table;

private slots:
    void createMenu(QPoint pos);
    void deleteItem();
    void refreshList();
    void setAdmin(int);

};

#endif // TABITEMUSER_H
