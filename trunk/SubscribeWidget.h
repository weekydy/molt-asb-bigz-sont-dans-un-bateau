#ifndef SUBSCRIBEWIDGET_H
#define SUBSCRIBEWIDGET_H

#include <QtGui>
#include <QtCore>
#include <QtSql>

class SubscribeWidget : public QDialog
{
Q_OBJECT
public:
    SubscribeWidget(QWidget *parent = 0);

private:
    QLineEdit *le_surname;
    QLineEdit *le_name;
    QLineEdit *le_nickname;
    QLineEdit *le_password;
    QLineEdit *le_password_bis;

    QPushButton *btn_save;
    QPushButton *btn_cancel;

private slots:
    void subscribe();


};

#endif // SUBSCRIBEWIDGET_H
