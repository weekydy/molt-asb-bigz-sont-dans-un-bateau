#ifndef TABITEMPROFILE_H
#define TABITEMPROFILE_H

#include <QtCore>
#include <QtGui>
#include <QtSql>

#include "Commons.h"
#include "ListWidget.h"

class TabItemProfile : public QWidget
{
Q_OBJECT
public:
    TabItemProfile(int _user_id, QWidget *parent = 0);

private:
    int user_id;

    QLineEdit *le_surname;
    QLineEdit *le_name;
    QLineEdit *le_nickname;
    QLineEdit *le_pass;
    QLineEdit *le_new_pass;
    QLineEdit *le_new_pass2;

    QPushButton *btn_edit_info;
    QPushButton *btn_edit_password;

    ListWidget *lw_left;
    ListWidget *lw_right;

    QPushButton *btn_to_left;
    QPushButton *btn_to_right;

    void moveCurrentItem(ListWidget *source, ListWidget *target);


private slots:
    void on_leftButton_clicked();
    void on_rightButton_clicked();
    void refreshButtonStateLeft();
    void refreshButtonStateRight();
    void updateGroups();
    void editInfo();
    void editPassword();

};

#endif // TABITEMUSER_H
