#include "TabItemRoom.h"

TabItemRoom::TabItemRoom(QWidget *parent) : QWidget(parent)
{
    model = new QSqlQueryModel();

    btn_add = new QPushButton("Ajouter");
    btn_edit = new QPushButton("Modifier");
    btn_edit->setDisabled(true);
    btn_del = new QPushButton("Supprimer");
    btn_del->setDisabled(true);

    QHBoxLayout *layout_btn = new QHBoxLayout();
    layout_btn->addWidget(btn_add);
    layout_btn->addWidget(btn_edit);
    layout_btn->addWidget(btn_del);
    layout_btn->addStretch();

    view = new QTableView();
    view->setDragEnabled(true);
    view->verticalHeader()->hide();
    view->setSelectionBehavior(QAbstractItemView::SelectRows);
    view->setSelectionMode(QAbstractItemView::SingleSelection);
    //view->setAlternatingRowColors(true);
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
    connect(btn_edit, SIGNAL(clicked()), this, SLOT(editItem()));
    connect(btn_del, SIGNAL(clicked()), this, SLOT(deleteItem()));
}

void TabItemRoom::refreshList()
{
    model->setQuery("SELECT * FROM room ORDER BY room_name");
    model->setHeaderData(1, Qt::Horizontal, "Nom");
    model->setHeaderData(2, Qt::Horizontal, "Capacit�");

    proxyModel->setSourceModel(model);

    view->setModel(proxyModel);
    view->hideColumn(0);
}

void TabItemRoom::createMenu(QPoint pos){
    QMenu *menu = new QMenu(this);
    QAction *action_add = new QAction("&Nouveau", this);
    menu->addAction(action_add);

    QModelIndex index = view->selectionModel()->currentIndex();
    index = index.sibling(index.row(), 0); // ici je force le n� de colonne � 0, pour etre sur le premier champ
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

void TabItemRoom::refreshButtonState(QModelIndex index){
    if(index.isValid())
    {
        btn_edit->setDisabled(false);
        btn_del->setDisabled(false);
    }
    else
    {
        btn_edit->setDisabled(true);
        btn_del->setDisabled(true);
    }
}

void TabItemRoom::deleteItem(){
    QModelIndex index = view->selectionModel()->currentIndex();
    index = index.sibling(index.row(), 0); // ici je force le n� de colonne � 0, pour etre sur le premier champ
    if (index.row() != -1){
        int id_room = proxyModel->data(index).toInt();
        QSqlQuery *req = new QSqlQuery();
        QSqlQuery *req2 = new QSqlQuery();
        req->prepare("DELETE FROM haveequipment WHERE room_id = :id_room");
        req->bindValue("id_room", id_room);

        req2->prepare("DELETE FROM room WHERE room_id = :id_room");
        req2->bindValue("id_room", id_room);

        int rep = QMessageBox::question(this, "Confirmation", "Etes-vous s�r de vouloir supprimer cette salle ?", QMessageBox::Yes | QMessageBox::No);
        if(rep == QMessageBox::Yes){

            if (req->exec() && req2->exec()){
                refreshList();
                QMessageBox::information(this, "Requ�te ex�cut�e avec succ�s !", "La salle a �t� supprim� de la base de donn�es !");
            }
            else
                QMessageBox::warning(this, "Erreur !", "La requ�te n'a pas pu �tre ex�cut�e ! \nDes r�unions peut-�tre rattach�es � cette salle ...");
        }
    }
}

void TabItemRoom::editItem(){
    QModelIndex index = view->selectionModel()->currentIndex();
    index = index.sibling(index.row(), 0); // ici je force le n� de colonne � 0, pour etre sur le premier champ
    if (index.row() != -1){
        int id_room = proxyModel->data(index).toInt();

        TabItemRoomActions *roomEdit = new TabItemRoomActions(EDIT, this, id_room);
        connect(roomEdit, SIGNAL(notifyRefreshList()), this, SLOT(refreshList()));
        roomEdit->exec();
    }
}

void TabItemRoom::addItem(){
    TabItemRoomActions *roomAdd = new TabItemRoomActions(ADD, this);
    connect(roomAdd, SIGNAL(notifyRefreshList()), this, SLOT(refreshList()));
    roomAdd->exec();
}
