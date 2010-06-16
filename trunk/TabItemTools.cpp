#include "TabItemTools.h"

TabItemTools::TabItemTools(QWidget *parent) : QWidget(parent)
{
    model = new QSqlQueryModel();

    te_request = new QTextEdit();
    te_request->setFixedHeight(150);

    btn_execute = new QPushButton("Executer la requête");
    btn_execute->setFixedWidth(150);

    view = new QTableView();
    view->setDragEnabled(true);

    view->setSortingEnabled (true);
    view->setEditTriggers(QAbstractItemView::DoubleClicked);

    proxyModel = new QSortFilterProxyModel();

    QVBoxLayout *layout_main = new QVBoxLayout();
    layout_main->addWidget(te_request);
    layout_main->addWidget(btn_execute);
    layout_main->addWidget(view);

    setLayout(layout_main);

    connect(btn_execute, SIGNAL(clicked()), this, SLOT(refreshList()));
}

void TabItemTools::refreshList(){
    if(te_request->toPlainText() != "")
    {
        QSqlQuery *req = new QSqlQuery();
        req->prepare(te_request->toPlainText());
        if(req->exec())
        {
            model->setQuery(*req);
            proxyModel->setSourceModel(model);
            view->setModel(proxyModel);

            QMessageBox::information(this, "Requête exécutée avec succès !", "Requête bien executée.\n" + req->lastInsertId().toString());
        }
        else
        {
            QMessageBox::warning(this, "Erreur !", "La requête n'a pas pu être exécutée !");
        }
    }
}
