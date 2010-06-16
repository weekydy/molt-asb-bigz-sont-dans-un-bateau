#ifndef TABITEMTOOLS_H
#define TABITEMTOOLS_H

#include <QtCore>
#include <QtGui>
#include <QtSql>

#include "Commons.h"
#include "TabItemTools.h"

class TabItemTools : public QWidget
{
Q_OBJECT
public:
    TabItemTools(QWidget *parent = 0);

private:
    QTextEdit *te_request;
    QPushButton *btn_execute;

    QSqlQueryModel *model;
    QTableView *view;
    QSortFilterProxyModel *proxyModel;

private slots:
    void refreshList();

};

#endif // TABITEMUSER_H
