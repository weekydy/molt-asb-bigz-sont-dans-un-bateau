#ifndef TABITEMGROUPACTIONS_H
#define TABITEMGROUPACTIONS_H

#include <QtGui>
#include <QtCore>
#include <QtSql>

#include "Commons.h"

class TabItemGroupActions : public QDialog
{
Q_OBJECT
public:
    TabItemGroupActions(QWidget *parent = 0);

private:
	
    QLineEdit *le_group_name;

    QPushButton *btn_action;
    QPushButton *btn_cancel;

private slots:
    void makeAction();

signals:
    void notifyRefreshList();
};

#endif // TabItemRoomActions_H
