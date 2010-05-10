#include "DbAccess.h"

DbAccess::DbAccess(){
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("../db.sqlite");
}
DbAccess::~DbAccess(){}

DbAccess* DbAccess::getInstance (){
    if (instance == NULL){
        instance =  new DbAccess();
    }
    return instance;
}

void DbAccess::kill(){
    if (instance != NULL){
        delete instance;
        instance = NULL;
    }
}

bool DbAccess::openDB(){
    return db.open();
}

DbAccess* DbAccess::instance = NULL;
