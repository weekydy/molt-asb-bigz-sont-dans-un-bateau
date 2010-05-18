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
    TabItemUser(QWidget *parent = 0);

private:
    QStandardItemModel *model;
    QTableView *view;
    QSortFilterProxyModel *proxyModel;

private slots:
    void createMenu(QPoint pos);
    void deleteItem();
    void refreshList();

};

#endif // TABITEMUSER_H
