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
    QDateTimeEdit *dt_begin2;
    QDateTimeEdit *dt_end;

    CheckBoxList *list_users;

    QComboBox *cb_room;
    QComboBox *list_equipments;

    QFormLayout *fl_data;

    QPushButton *btn_findHour;
    QPushButton *btn_findRoom;
    QPushButton *btn_action;
    QPushButton *btn_cancel;

    QGroupBox *qgb_room;
    QGroupBox *qgb_hour;
    QGroupBox *qgb_option;

    QCheckBox *qcb_guest;
    QCheckBox *qcb_equipment;
    QCheckBox *qcb_extend;
    QCheckBox *qcb_available;
    QCheckBox *qcb_compulsory;
    QCheckBox *qcb_recurring;

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
