#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QtCore>
#include <QtGui>
#include <QSqlQuery>
#include <QSqlRecord>

#include "SubscribeWidget.h"

class LoginWidget : public QWidget
{
Q_OBJECT

public:
    LoginWidget(QWidget *parent = 0);

private:
    QLineEdit *le_id;
    QLineEdit *le_pass;
    QPushButton *button_connect;
    QPushButton *button_subscribe;
    QLabel *label_msg;
    QCheckBox *cb_remember;

    QSettings *settings;
    void keyPressEvent(QKeyEvent *keyEvent);

private slots:
    void connection();
    void subscribe();

signals:
    void notifyConnected(int user_id, int access);
};

#endif // LOGINWIDGET_H
