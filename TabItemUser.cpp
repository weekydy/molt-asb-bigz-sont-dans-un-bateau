#include "TabItemUser.h"

// internal private delegate
class CheckBoxDelegate : public QItemDelegate
{
private:
    int checkboxCol; // index de la colonne ayant la checkbox
public:
    CheckBoxDelegate(QObject *parent): QItemDelegate(parent), checkboxCol(5){}

    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const{
        if(index.isValid() && index.column() == checkboxCol){
            QCheckBox *editor = new QCheckBox(parent);
            editor->installEventFilter(const_cast<CheckBoxDelegate*>(this));
            return editor;
        }
        else{
            return QItemDelegate::createEditor(parent, option, index);
        }
    }

    void setEditorData(QWidget *editor, const QModelIndex &index) const{
        if(index.isValid() && index.column() == checkboxCol){
            int value = index.model()->data(index, Qt::DisplayRole).toInt();

            QCheckBox *checkBox = static_cast<QCheckBox*>(editor);
            if(value == 2) // 2 = droit administrateur dans la DB
                checkBox->setCheckState(Qt::Checked);
            else
                checkBox->setCheckState(Qt::Unchecked);
            }
        else{
            QItemDelegate::setEditorData(editor, index);
        }
    }

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const{
        if(index.isValid() && index.column() == checkboxCol){
            QCheckBox *checkBox = static_cast<QCheckBox*>(editor);
            int value;
            if(checkBox->checkState() == Qt::Checked)
                value = 2;
            else
                value = 1;

            model->setData(index, value);
        }
        else{
            QItemDelegate::setModelData(editor, model, index);
        }
    }

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const{
        if(index.isValid() && index.column() == checkboxCol)
            editor->setGeometry(option.rect);
        else
            QItemDelegate::updateEditorGeometry(editor, option, index);
    }
};

TabItemUser::TabItemUser(QWidget *parent) : QWidget(parent)
{
    model = new QSqlQueryModel();

    QLabel *lb_message = new QLabel("Faites un clic droit sur les élèments pour avoir accès aux actions.");

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
/*
    model = new QStandardItemModel();

    view = new QTableView();
    view->setDragEnabled(true);
    view->verticalHeader()->hide();
    view->setSelectionBehavior(QAbstractItemView::SelectRows);
    view->setSelectionMode(QAbstractItemView::SingleSelection);
    view->setAlternatingRowColors(true);
    view->setSortingEnabled (true);
    view->setContextMenuPolicy(Qt::CustomContextMenu);

    proxyModel = new QSortFilterProxyModel();
----
    table = new QTableWidget(0,5,this);
    table->verticalHeader()->hide();
    table->hideColumn(0); // On cache la clé
    table->setSelectionMode(QAbstractItemView::NoSelection);
    table->setAlternatingRowColors(true);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setContextMenuPolicy(Qt::CustomContextMenu);
*/

    refreshList();

    QVBoxLayout *layout_main = new QVBoxLayout();
    layout_main->addWidget(lb_message);
    layout_main->addWidget(view);

    setLayout(layout_main);

    connect(view, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(createMenu(QPoint)));

}

void TabItemUser::refreshList(){

    model->setQuery("SELECT * FROM user ORDER BY user_surname, user_name");
    model->setHeaderData(1, Qt::Horizontal, "Nom");
    model->setHeaderData(2, Qt::Horizontal, "Prénom");
    model->setHeaderData(3, Qt::Horizontal, "Identifiant");
    model->setHeaderData(5, Qt::Horizontal, "Admin");

    proxyModel->setSourceModel(model);

    view->setModel(proxyModel);
    CheckBoxDelegate *delegate = new CheckBoxDelegate(this);
    view->setItemDelegate(delegate);
    view->hideColumn(0); // id
    view->hideColumn(4); // mot de passe


    /*table->clear();
    table->setRowCount(0);

    QStringList title;
    title << "" << "Nom" << "Prénom" << "Identifiant" << "Admin";

    table->setHorizontalHeaderLabels(title);

    int i=0, j;
    QSqlQuery req;
    req.prepare("SELECT * FROM user ORDER BY surname, name");
    req.exec();
    QSqlRecord record = req.record();

    while (req.next()) {
        int id_user = req.value(record.indexOf("id")).toInt();
        qDebug() << id_user;
        QString surname = req.value(record.indexOf("surname")).toString();
        QString name = req.value(record.indexOf("name")).toString();
        QString nickname = req.value(record.indexOf("nickname")).toString();

        bool admin = req.value(record.indexOf("access")).toInt() == 2 ? true : false;

        QTableWidgetItem *item_id = new QTableWidgetItem(id_user);
        QTableWidgetItem *item_surname = new QTableWidgetItem(surname);
        QTableWidgetItem *item_name = new QTableWidgetItem(name);
        QTableWidgetItem *item_nickname = new QTableWidgetItem(nickname);

        item_id->setFlags(item_id->flags() & ~Qt::ItemIsEditable);
        item_surname->setFlags(item_surname->flags() & ~Qt::ItemIsEditable); // & ET logique ~ negation
        item_name->setFlags(item_name->flags() & ~Qt::ItemIsEditable); // & ET logique ~ negation
        item_nickname->setFlags(item_nickname->flags() & ~Qt::ItemIsEditable); // & ET logique ~ negation

        QCheckBox *checkBox = new QCheckBox;

        checkBox->setChecked(admin);

        j = 0;
        table->insertRow(i);

        table->setItem(i, j, item_id);
        table->setItem(i, j+1, item_surname);
        table->setItem(i, j+2, item_name);
        table->setItem(i, j+3, item_nickname);
        table->setCellWidget(i, j+4, checkBox);

        i++;
    }*/

/*
    model->setHeaderData(1, Qt::Horizontal, "Nom");
    model->setHeaderData(2, Qt::Horizontal, "Prénom");
    model->setHeaderData(3, Qt::Horizontal, "Identifiant");
    model->setHeaderData(4, Qt::Horizontal, "Admin");

    int i=0, j;
    QSqlQuery req;
    req.prepare("SELECT * FROM user ORDER BY surname, name");
    req.exec();
    QSqlRecord record = req.record();


    while (req.next()) {
        int id_user = req.value(record.indexOf("id")).toInt();
        qDebug() << id_user;
        QString surname = req.value(record.indexOf("surname")).toString();
        QString name = req.value(record.indexOf("name")).toString();
        QString nickname = req.value(record.indexOf("nickname")).toString();
        bool admin = req.value(record.indexOf("access")).toInt() == 2 ? true : false;

        QStandardItem *item_id = new QStandardItem(id_user);
        QStandardItem *item_surname = new QStandardItem(surname);
        QStandardItem *item_name = new QStandardItem(name);
        QStandardItem *item_nickname = new QStandardItem(nickname);
        QStandardItem *item_access = new QStandardItem(admin);

        item_id->setFlags(item_id->flags() & ~Qt::ItemIsEditable);
        item_surname->setFlags(item_surname->flags() & ~Qt::ItemIsEditable); // & ET logique ~ negation
        item_name->setFlags(item_name->flags() & ~Qt::ItemIsEditable); // & ET logique ~ negation
        item_nickname->setFlags(item_nickname->flags() & ~Qt::ItemIsEditable); // & ET logique ~ negation
        item_access->setFlags(item_access->flags() & Qt::ItemIsEditable); // & ET logique ~ negation

        item_access->setCheckable(true);
        item_access->setCheckState(Qt::Checked);

        j = 0;

        model->setItem(i, j, item_id);
        model->setItem(i, j+1, item_surname);
        model->setItem(i, j+2, item_name);
        model->setItem(i, j+3, item_nickname);
        model->setItem(i, j+4, item_access);

        i++;
    }

    proxyModel->setSourceModel(model);

    view->setModel(proxyModel);
    //view->hideColumn(0);

*/
}

void TabItemUser::createMenu(QPoint pos){
    QMenu *menu = new QMenu(this);

    QModelIndex index = view->selectionModel()->currentIndex();
    index = index.sibling(index.row(), 0); // ici je force le n° de colonne à 0, pour etre sur le premier champ
    if (index.row() != -1){

        QAction *action_del = new QAction("&Supprimer", this);
        menu->addAction(action_del);

        connect(action_del, SIGNAL(triggered()), this, SLOT(deleteItem()));
    }

    menu->exec(view->mapToGlobal(pos));
}

void TabItemUser::deleteItem(){
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
    }
}
