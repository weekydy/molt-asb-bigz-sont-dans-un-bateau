#ifndef TABITEMPLANNING_H
#define TABITEMPLANNING_H

#include <QtCore>
#include <QtGui>
#include <QtSql>

#include "Commons.h"
#include "MeetingActions.h"

class TabItemPlanning : public QWidget
{
Q_OBJECT
public:
    TabItemPlanning(int _user_id, QWidget *parent = 0);

private:
    int user_id;
    QStandardItemModel *model;
    QTableView *view;
    QCalendarWidget *calendar;
    QComboBox *cb_view;
    QPushButton *btn_add;
    QStringList hours;
    QList<QColor> *colours;

private slots:
    void createMenu(QPoint pos);
    void deleteItem();
    void editItem();
    void addItem();
    void refreshList();
    void displayInfo();
    void addMeeting();
    //void resizeEvent(QResizeEvent * event);

};

#endif // TABITEMROOM_H
