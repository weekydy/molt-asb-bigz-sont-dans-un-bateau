#include "TabItemUser.h"

TabItemUser::TabItemUser(int _user_id, QWidget *parent) : QWidget(parent)
{
    user_id = _user_id;



    table = new QTableWidget(0,6,this);

    table->verticalHeader()->hide();
    table->hideColumn(0); // On cache la clé
    table->setSelectionMode(QAbstractItemView::NoSelection);
    //table->setAlternatingRowColors(true);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);

    refreshList();


    QVBoxLayout *layout_main = new QVBoxLayout();
    layout_main->addWidget(table);

    setLayout(layout_main);

    connect(table, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(createMenu(QPoint)));

}

void TabItemUser::refreshList(){
    table->clear();
    table->setRowCount(0);

    QStringList title;
    title << "id_meeting" << "Nom" << "Prénom" << "Pseudonyme" << "Groupe(s)" << "Administrateur";
    table->setHorizontalHeaderLabels(title);

    int i=0, j;
    QSqlQuery *req = new QSqlQuery();
    QSqlRecord rec;
    req->prepare("SELECT * FROM user ORDER BY user_surname, user_name");
    req->exec();
    rec = req->record();

    QSignalMapper *mapper = new QSignalMapper();

    while(req->next()){
        int id_user = req->value(rec.indexOf("user_id")).toInt();
        if(id_user != user_id){ // on affiche tout le monde sauf nous
            QString surname = req->value(rec.indexOf("user_surname")).toString();
            QString name = req->value(rec.indexOf("user_name")).toString();
            QString nickname = req->value(rec.indexOf("user_nickname")).toString();
            bool access;
            if(req->value(rec.indexOf("user_access")).toInt() == 2)
                access = true;
            else
                access = false;


            QComboBox *cb_group = new QComboBox();
            QSqlQuery *req2 = new QSqlQuery();
            req2->prepare("SELECT * FROM user u INNER JOIN belongtogroup bg ON u.user_id = bg.user_id INNER JOIN grp g ON g.grp_id = bg.grp_id WHERE u.user_id = :user_id ORDER BY g.grp_name");
            req2->bindValue(":user_id", id_user);            
            req2->exec();
            QSqlRecord rec2 = req2->record();
            while(req2->next()){
                cb_group->addItem(req2->value(rec2.indexOf("grp_name")).toString());
            }
            if(cb_group->count() == 0)
                cb_group->addItem("Aucun");


            QCheckBox *cb_admin = new QCheckBox();
            cb_admin->setChecked(access);

            connect(cb_admin, SIGNAL(stateChanged(int)), mapper, SLOT(map()));
            mapper->setMapping(cb_admin, i);


            QTableWidgetItem *twi_id_user = new QTableWidgetItem();
            QTableWidgetItem *twi_surname = new QTableWidgetItem(surname);
            QTableWidgetItem *twi_name = new QTableWidgetItem(name);
            QTableWidgetItem *twi_nickname = new QTableWidgetItem(nickname);

            twi_id_user->setData(Qt::DisplayRole, id_user);
            twi_id_user->setFlags(twi_id_user->flags() & ~Qt::ItemIsEditable);
            twi_surname->setFlags(twi_surname->flags() & ~Qt::ItemIsEditable);
            twi_name->setFlags(twi_name->flags() & ~Qt::ItemIsEditable);
            twi_nickname->setFlags(twi_nickname->flags() & ~Qt::ItemIsEditable);

            j = 0;
            table->insertRow(i);

            table->setItem(i, j, twi_id_user);
            table->setItem(i, j+1, twi_surname);
            table->setItem(i, j+2, twi_name);
            table->setItem(i, j+3, twi_nickname);
            table->setCellWidget(i, j+4, cb_group);
            table->setCellWidget(i, j+5, cb_admin);

            i++;
        }
    }

    connect(mapper, SIGNAL(mapped(int)), this, SLOT(setAdmin(int)));

}

void TabItemUser::createMenu(QPoint pos){
    QMenu *menu = new QMenu(this);

    QModelIndex index = table->currentIndex();
    index = index.sibling(index.row(), 0); // ici je force le n° de colonne à 0, pour etre sur le premier champ
    if (index.row() != -1){

        QAction *action_del = new QAction("&Supprimer", this);
        menu->addAction(action_del);

        connect(action_del, SIGNAL(triggered()), this, SLOT(deleteItem()));
    }

    menu->exec(table->mapToGlobal(pos));
}

void TabItemUser::deleteItem(){
    /*
    QModelIndex index = view->selectionModel()->currentIndex();
    index = index.sibling(index.row(), 0); // ici je force le n° de colonne à 0, pour etre sur le premier champ
    if (index.row() != -1){
        int id_user = proxyModel->data(index).toInt();
        QSqlQuery *req = new QSqlQuery();
        req->prepare("DELETE FROM user WHERE user_id = :id_user");
        req->bindValue("id_user", id_user);

        int rep = QMessageBox::question(this, "Confirmation", "Etes-vous sûr de vouloir supprimer cet utilisateur ?", QMessageBox::Yes | QMessageBox::No);
        if(rep == QMessageBox::Yes){
            if (req->exec()){
                refreshList();
                QMessageBox::information(this, "Requête exécutée avec succès !", "La salle a été supprimé de la base de données !");
            }
            else
                QMessageBox::warning(this, "Erreur !", "La requête n'a pas pu être exécutée !");
        }
    }*/
}

void TabItemUser::setAdmin(int row){
    int access = 1;
    int id_user = table->item(row, 0)->data(Qt::DisplayRole).toInt();

    bool admin = qobject_cast<QCheckBox*>(table->cellWidget(row, 5))->isChecked();
    if(admin){
        access = 2;
    }

    QSqlQuery *req = new QSqlQuery();
    req->prepare("UPDATE user SET user_access = :access WHERE user_id = :id_user");
    req->bindValue(":access", access);
    req->bindValue(":id_user", id_user);    
    req->exec();
    refreshList();
}
