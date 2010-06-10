#ifndef HEADER_FENPRINCIPALE
#define HEADER_FENPRINCIPALE

#include <QtCore>
#include <QtGui>
#include <QtSql>

#include "Commons.h"
#include "dbAccess.h"
#include "LoginWidget.h"
#include "TabItemRoom.h"
#include "TabItemUser.h"
#include "TabItemEquipment.h"
#include "TabItemPlanning.h"
#include "TabItemNews.h"
#include "TabItemGroup.h"
#include "TabItemProfile.h"


using namespace std;

class MainWindow : public QMainWindow
{
Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

private:
    void createActions();
    void createMenus();
    void createStatusBar();
    void connectionDB();
    void createWidgets();
    void createSignalsSlots();
    void closeEvent(QCloseEvent* event);

private:
    int access;
    int user_id;

    QAction *action_quit;
    QAction *action_about;

    QMenu *menu_file;
    QMenu *menu_help;
    QStatusBar *myStatusBar;

public slots:
    void about();
    void updateConnected(int user_id, int access);
    void disconnected();
    void deleteAccount();

};


#endif