#include <QtCore>
#include <QtGui>

#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setStyle(new QPlastiqueStyle);
    MainWindow w;
    w.show();

    return app.exec();
}
