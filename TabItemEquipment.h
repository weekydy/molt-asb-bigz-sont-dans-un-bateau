#ifndef TABITEMEQUIPMENT_H
#define TABITEMEQUIPMENT_H

#include <QtCore>
#include <QtGui>
#include <QtSql>

#include "Commons.h"
#include "TabItemEquipmentActions.h"

class TabItemEquipment : public QWidget
{
Q_OBJECT
public:
    TabItemEquipment(QWidget *parent = 0);

private:
    //QStandardItemModel *model;
    QSqlQueryModel *model;
    QTableView *view;
    QSortFilterProxyModel *proxyModel;

    QPushButton *btn_add;
    QPushButton *btn_del;

private slots:
    void createMenu(QPoint pos);
    void deleteItem();
    void addItem();
    void refreshList();
    void refreshButtonState(QModelIndex index);

};

#endif // TABITEMUSER_H
