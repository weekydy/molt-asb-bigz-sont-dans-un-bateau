#ifndef TABITEMGROUP_H
#define TABITEMGROUP_H

#include <QtCore>
#include <QtGui>
#include <QtSql>

#include "Commons.h"
#include "TabItemGroupActions.h"

class TabItemGroup : public QWidget
{
Q_OBJECT
public:
    TabItemGroup(QWidget *parent = 0);

private:
    QSqlQueryModel *model;
    QTableView *view;
    QSortFilterProxyModel *proxyModel;

    QPushButton *btn_add;
    QPushButton *btn_edit;
    QPushButton *btn_del;

private slots:
    void createMenu(QPoint pos);
    void deleteItem();
    void addItem();
    void refreshList();
    void refreshButtonState(QModelIndex index);
};

#endif // TABITEMUSER_H
