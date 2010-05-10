#ifndef DEF_DBACCESS
#define DEF_DBACCESS

#include <QSqlDatabase>
#include <QDebug>

#include "dbAccess.h"

using namespace std;

class DbAccess
{
private:
    DbAccess();
    ~DbAccess();

public:
    static DbAccess *getInstance();
    static void kill();
    bool openDB();

private:
    QSqlDatabase db;
    static DbAccess *instance;
};

#endif
