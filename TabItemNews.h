#ifndef TABITEMROOM_H
#define TABITEMROOM_H

#include <QtCore>
#include <QtGui>
#include <QtSql>

#include "TabItemRoomActions.h"
#include "Commons.h"

class TabItemRoom : public QWidget
{
Q_OBJECT
public:
    TabItemRoom(QWidget *parent = 0);

private:
    QSqlQueryModel *model;
    QTableView *view;
    QSortFilterProxyModel *proxyModel;

private slots:
    void createMenu(QPoint pos);
    void deleteItem();
    void editItem();
    void addItem();
    void refreshList();

};

#endif // TABITEMROOM_H
