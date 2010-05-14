#ifndef TABITEMROOMACTIONS_H
#define TABITEMROOMACTIONS_H

#include <QtGui>
#include <QtCore>
#include <QtSql>

#include "Commons.h"
#include "CheckBoxList.h"

class TabItemRoomActions : public QDialog
{
Q_OBJECT
public:
    TabItemRoomActions(int action, QWidget *parent = 0, int id = -1);

private:
    int action;
    int id;
	
    QLineEdit *le_room_name;
    QSpinBox *le_room_capacity;
    CheckBoxList *list;

    QPushButton *btn_action;
    QPushButton *btn_cancel;

private slots:
    void makeAction();

signals:
    void notifyRefreshList();
};

#endif // TabItemRoomActions_H
