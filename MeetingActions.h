#ifndef MEETINGACTIONS_H
#define MEETINGACTIONS_H

#include <QtGui>
#include <QtCore>
#include <QtSql>

#include "Commons.h"
#include "CheckBoxList.h"
#include "SolutionsEngine.h"

class MeetingActions : public QDialog
{
Q_OBJECT
public:
    MeetingActions(int action, QWidget *parent = 0, int id = -1);

private:
    int action;
    int id;
    
    QLineEdit *le_label;
    QDateTimeEdit *dt_begin;
    QDateTimeEdit *dt_end;

    CheckBoxList *list_users;

    QComboBox *cb_room;
    QComboBox *list_equipments;

    QFormLayout *fl_data;

    QPushButton *btn_find;
    QPushButton *btn_check;
    QPushButton *btn_action;
    QPushButton *btn_cancel;

    SolutionsEngine* engine;

private slots:
    void makeAction();
    void changeComboBoxEquipment(int);
    void findHours();
    void findRoom();

signals:
    void notifyRefreshList();
};

#endif // MeetingActions_H
