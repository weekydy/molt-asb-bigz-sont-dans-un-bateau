#ifndef TABITEMEQUIPMENTACTIONS_H
#define TABITEMEQUIPMENTACTIONS_H

#include <QtGui>
#include <QtCore>
#include <QtSql>

#include "Commons.h"

class TabItemEquipmentActions : public QDialog
{
Q_OBJECT
public:
    TabItemEquipmentActions(QWidget *parent = 0);

private:
	
    QLineEdit *le_equip_name;

    QPushButton *btn_action;
    QPushButton *btn_cancel;

private slots:
    void makeAction();

signals:
    void notifyRefreshList();
};

#endif // TabItemRoomActions_H
