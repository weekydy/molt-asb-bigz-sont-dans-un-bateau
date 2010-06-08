#include <QtCore>
#include <QtGui>

#include "Smtp.h"
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    //app.setStyle(new QPlastiqueStyle);

    // Style Sheet
    QString error = "";
    QFile* style = new QFile(":style.css");
    if(!style->open(QIODevice::Text | QIODevice::ReadOnly)){
        error = "A file is missing, please reinstall the application...";
        qDebug() << error;
    }
    else{
        app.setStyleSheet(style->readAll());
        style->close();
    }


    Smtp *newMail = new Smtp("from@address.com","to@address.com"," Your Subject","My body text");
    delete newMail;

    MainWindow w;
    w.show();

    return app.exec();
}
