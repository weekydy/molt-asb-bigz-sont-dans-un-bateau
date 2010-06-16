#include <QtCore>
#include <QtGui>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>



#include "Smtp.h"
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    //app.setStyle(new QPlastiqueStyle);

    // Lange francais
    QString locale = QLocale::system().name();
    QTranslator translator;
    translator.load(QString("qt_fr"), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&translator);

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


    //Smtp *newMail = new Smtp("adrien.gavignet@gmail.com", "bastien.cramillet@gmail.com", "Test","Moi, un plow du reseau ?");
    //delete newMail;

    MainWindow w;
    w.show();

    return app.exec();
}
