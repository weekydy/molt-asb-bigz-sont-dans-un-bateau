#ifndef HEADER_TABWIDGET
#define HEADER_TABWIDGET

#include <QtCore>
#include <QtGui>

class TabWidget : public QTabWidget
{
public:
    TabWidget(QWidget *parent = 0);

private:
    void createTabs();

};


#endif
