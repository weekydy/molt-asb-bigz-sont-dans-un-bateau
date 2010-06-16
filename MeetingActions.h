#ifndef MEETINGACTIONS_H
#define MEETINGACTIONS_H

#include <QtGui>
#include <QtCore>
#include <QtSql>

#include "Commons.h"
#include "CheckBoxList.h"
#include "SolutionsEngine.h"
#include "ListWidget.h"

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

    QTimeEdit *qte_duration;

    //CheckBoxList *list_users;
    ListWidget *lw_groups;
    ListWidget *lw_users;
    ListWidget *lw_targets;

    QComboBox *cb_room;
    QComboBox *list_equipments;
    QComboBox *qcb_recurring;

    QFormLayout *fl_data;
    QFormLayout *fl_hour;
    QFormLayout *fl_recurring;

    QPushButton *btn_findHour;
    QPushButton *btn_findRoom;
    QPushButton *btn_action;
    QPushButton *btn_cancel;

    QGroupBox *qgb_frm;
    QGroupBox *qgb_room;
    QGroupBox *qgb_hour;
    QGroupBox *qgb_option;

    QCheckBox *qcb_guest;
    QCheckBox *qcb_equipment;
    QCheckBox *qcb_extend;
    QCheckBox *qcb_available;
    QCheckBox *qcb_compulsory;

    QComboBox *cb_colors;

    SolutionsEngine* engine;

    void moveCurrentItem(ListWidget *source, ListWidget *target);

private slots:
    void makeAction();
    void changeComboBoxEquipment(int);
    void findHours();
    void findRoom();

signals:
    void notifyRefreshList();
};

#endif // MeetingActions_H
