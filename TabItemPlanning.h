#ifndef TABITEMPLANNING_H
#define TABITEMPLANNING_H

#include <QtCore>
#include <QtGui>
#include <QtSql>

#include "Commons.h"

class TabItemPlanning : public QWidget
{
Q_OBJECT
public:
    TabItemPlanning(QWidget *parent = 0);

private:
    QTableWidget *table;

private slots:
    void createMenu(QPoint pos);
    void deleteItem();
    void editItem();
    void addItem();
    void refreshList();

};

#endif // TABITEMROOM_H
