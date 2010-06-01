#ifndef MEETINGDETAILS_H
#define MEETINGDETAILS_H

#include <QtGui>
#include <QtCore>
#include <QtSql>

#include "Commons.h"
#include "CheckBoxList.h"

class MeetingDetails : public QDialog
{
Q_OBJECT
public:
    MeetingDetails(int id, QWidget *parent = 0);

private:
    int id;
    
    QLabel *lb_label;
    QLabel *lb_begin;
    QLabel *lb_end;

    QComboBox *cb_users;

    QLabel *lb_room;
    QComboBox *cb_equipments;

    QPushButton *btn_unavailable;
    QPushButton *btn_action;
    QPushButton *btn_cancel;

private slots:
    void canceledMeeting();

signals:
    void notifyRefreshList();

};

#endif // MeetingActions_H
