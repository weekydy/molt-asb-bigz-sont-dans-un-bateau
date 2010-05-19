#include "TabItemPlanning.h"

TabItemPlanning::TabItemPlanning(QWidget *parent) : QWidget(parent)
{
    model = new QTableWidget();

    refreshList();

    QVBoxLayout *layout_main = new QVBoxLayout();
    layout_main->addWidget(table);

    setLayout(layout_main);

    connect(table, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(createMenu(QPoint)));

}

void TabItemPlanning::refreshList(){

}

void TabItemPlanning::createMenu(QPoint pos){
    QMenu *menu = new QMenu(this);
    QAction *action_add = new QAction("&Nouveau", this);
    menu->addAction(action_add);

    QModelIndex index = view->selectionModel()->currentIndex();
    index = index.sibling(index.row(), 0); // ici je force le n° de colonne à 0, pour etre sur le premier champ
    if (index.row() != -1){

        QAction *action_del = new QAction("&Supprimer", this);
        QAction *action_edit = new QAction("&Modifier", this);
        menu->addAction(action_del);
        menu->addAction(action_edit);

        connect(action_del, SIGNAL(triggered()), this, SLOT(deleteItem()));
        connect(action_edit, SIGNAL(triggered()), this, SLOT(editItem()));
    }
    connect(action_add, SIGNAL(triggered()), this, SLOT(addItem()));
    menu->exec(view->mapToGlobal(pos));
}

void TabItemPlanning::deleteItem(){
    QModelIndex index = view->selectionModel()->currentIndex();
    index = index.sibling(index.row(), 0); // ici je force le n° de colonne à 0, pour etre sur le premier champ
    if (index.row() != -1){
        int id_room = proxyModel->data(index).toInt();
        QSqlQuery *req = new QSqlQuery();
        QSqlQuery *req2 = new QSqlQuery();
        req->prepare("DELETE FROM haveequipment WHERE id_room = :id_room");
        req->bindValue("id_room", id_room);

        req2->prepare("DELETE FROM room WHERE id = :id_room");
        req2->bindValue("id_room", id_room);

        int rep = QMessageBox::question(this, "Confirmation", "Etes-vous sûr de vouloir supprimer cette salle ?", QMessageBox::Yes | QMessageBox::No);
        if(rep == QMessageBox::Yes){

            if (req->exec() && req2->exec()){
                refreshList();
                QMessageBox::information(this, "Requête exécutée avec succès !", "La salle a été supprimé de la base de données !");
            }
            else
                QMessageBox::warning(this, "Erreur !", "La requête n'a pas pu être exécutée ! \nDes réunions peut-être rattachées à cette salle ...");
        }
    }
}

void TabItemPlanning::editItem(){/*
    QModelIndex index = view->selectionModel()->currentIndex();
    index = index.sibling(index.row(), 0); // ici je force le n° de colonne à 0, pour etre sur le premier champ
    if (index.row() != -1){
        int id_room = proxyModel->data(index).toInt();

        TabItemRoomActions *roomEdit = new TabItemRoomActions(EDIT, this, id_room);
        connect(roomEdit, SIGNAL(notifyRefreshList()), this, SLOT(refreshList()));
        roomEdit->exec();
    }*/
}

void TabItemPlanning::addItem(){
    /*
    TabItemRoomActions *roomAdd = new TabItemRoomActions(ADD, this);
    connect(roomAdd, SIGNAL(notifyRefreshList()), this, SLOT(refreshList()));
    roomAdd->exec();
    */
}
