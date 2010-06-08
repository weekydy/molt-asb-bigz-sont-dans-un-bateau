#ifndef SMTP_H
#define SMTP_H

#include <QtCore>
#include <QtGui>
#include <QTcpSocket>

class Smtp : public QObject
{

Q_OBJECT

public:
    Smtp(const QString &from, const QString &to, const QString &subject, const QString &body );
    ~Smtp();

signals:
    void status( const QString &);

private slots:
    void stateChanged(QTcpSocket::SocketState socketState);
    void errorReceived(QTcpSocket::SocketError socketError);
    void disconnected();
    void connected();
    void readyRead();

private:

    QString message;
    QTextStream *t;
    QTcpSocket *socket;
    QString from;
    QString rcpt;
    QString response;
    enum states{Rcpt,Mail,Data,Init,Body,Quit,Close};

    int state;

};

#endif
