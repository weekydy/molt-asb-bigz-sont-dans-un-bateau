#include "TabItemGroup.h"

TabItemGroup::TabItemGroup(QWidget *parent) : QWidget(parent)
{
    model = new QSqlQueryModel();

    btn_add = new QPushButton("Ajouter");
    btn_del = new QPushButton("Supprimer");
    btn_del->setDisabled(true);

    QHBoxLayout *layout_btn = new QHBoxLayout();
    layout_btn->addWidget(btn_add);
    layout_btn->addWidget(btn_del);
    layout_btn->addStretch();

    view = new QTableView();
    view->setEditTriggers(QAbstractItemView::CurrentChanged);
    view->setDragEnabled(true);
    view->verticalHeader()->hide();
    view->setSelectionBehavior(QAbstractItemView::SelectRows);
    view->setSelectionMode(QAbstractItemView::SingleSelection);
    view->setAlternatingRowColors(true);
    view->setSortingEnabled (true);
    view->setContextMenuPolicy(Qt::CustomContextMenu);

    proxyModel = new QSortFilterProxyModel();

    refreshList();

    QVBoxLayout *layout_main = new QVBoxLayout();
    layout_main->addLayout(layout_btn);
    layout_main->addWidget(view);

    setLayout(layout_main);

    connect(view, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(createMenu(QPoint)));
    connect(view, SIGNAL(clicked(QModelIndex)), this, SLOT(refreshButtonState(QModelIndex)));
    connect(btn_add, SIGNAL(clicked()), this, SLOT(addItem()));
    connect(btn_del, SIGNAL(clicked()), this, SLOT(deleteItem()));

}

void TabItemGroup::refreshList(){

    model->setQuery("SELECT * FROM grp ORDER BY grp_name");
    model->setHeaderData(1, Qt::Horizontal, "Nom");

    proxyModel->setSourceModel(model);

    view->setModel(proxyModel);
    view->hideColumn(0); // id

}

void TabItemGroup::createMenu(QPoint pos){
    QMenu *menu = new QMenu(this);
    QAction *action_add = new QAction("&Nouveau", this);
    menu->addAction(action_add);

    QModelIndex index = view->selectionModel()->currentIndex();
    index = index.sibling(index.row(), 0); // ici je force le n° de colonne à 0, pour etre sur le premier champ
    if (index.row() != -1){

        QAction *action_del = new QAction("&Supprimer", this);
        menu->addAction(action_del);

        connect(action_del, SIGNAL(triggered()), this, SLOT(deleteItem()));
    }
    connect(action_add, SIGNAL(triggered()), this, SLOT(addItem()));
    menu->exec(view->mapToGlobal(pos));
}

void TabItemGroup::deleteItem(){
    QModelIndex index = view->selectionModel()->currentIndex();
    index = index.sibling(index.row(), 0); // ici je force le n° de colonne à 0, pour etre sur le premier champ
    if (index.row() != -1){
        int id_grp = proxyModel->data(index).toInt();
        QSqlQuery *req = new QSqlQuery();
        req->prepare("DELETE FROM belongtogroup WHERE grp_id = :id_grp");
        req->bindValue(":id_grp", id_grp);

        QSqlQuery *req2 = new QSqlQuery();
        req2->prepare("DELETE FROM grp WHERE grp_id = :id_grp");
        req2->bindValue("id_grp", id_grp);

        int rep = QMessageBox::question(this, "Confirmation", "Etes-vous sûr de vouloir supprimer ce groupe ?", QMessageBox::Yes | QMessageBox::No);
        if(rep == QMessageBox::Yes){
            if (req->exec() && req2->exec()){
                refreshList();
                QMessageBox::information(this, "Requête exécutée avec succès !", "L'équipement a été supprimé de la base de données !");
            }
            else
                QMessageBox::warning(this, "Erreur !", "La requête n'a pas pu être exécutée !");
        }
    }
}

void TabItemGroup::refreshButtonState(QModelIndex index){
    if(index.isValid())
    {
        btn_del->setDisabled(false);
    }
    else
    {
        btn_del->setDisabled(true);
    }
}

void TabItemGroup::addItem(){
    TabItemGroupActions *groupAdd = new TabItemGroupActions(this);
    connect(groupAdd, SIGNAL(notifyRefreshList()), this, SLOT(refreshList()));
    groupAdd->exec();
}
