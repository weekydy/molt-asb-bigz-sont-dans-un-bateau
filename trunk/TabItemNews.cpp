#include "TabItemNews.h"

TabItemNews::TabItemNews(int user_id, QWidget *parent) : QWidget(parent)
{
    id = user_id;

    lb_message = new QLabel();

    table = new QTableWidget(0,8,this);

    table->verticalHeader()->hide();
    table->hideColumn(0); // On cache la clé
    table->setSelectionMode(QAbstractItemView::NoSelection);
    table->setAlternatingRowColors(true);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);

    refreshList();

    QVBoxLayout *layout_main = new QVBoxLayout();
    layout_main->addWidget(lb_message);
    layout_main->addWidget(table);

    setLayout(layout_main);
}

void TabItemNews::refreshList(){
    table->clear();
    table->setRowCount(0);

    QStringList title;
    title << "id_meeting" << "Libellé" << "Début" << "Fin" << "Périodicité" << "Organisateur" << "Accepter" << "Refuser";
    table->setHorizontalHeaderLabels(title);

    int i=0, j;
    QSqlQuery *req = new QSqlQuery();
    QSqlRecord rec;
    req->prepare("SELECT * FROM meeting m INNER JOIN havemeeting hm ON hm.user_id = hm.user_id INNER JOIN user u ON u.user_id = m.user_id WHERE hm.user_id = :user_id AND hm.hm_state = :state ORDER BY meeting_begin DESC");
    req->bindValue(":user_id", id);
    req->bindValue(":state", MAY_ATTEND);
    req->exec();
    rec = req->record();

    QSignalMapper *mapper_accept = new QSignalMapper();
    QSignalMapper *mapper_decline = new QSignalMapper();

    while(req->next()){
        int id_meeting = req->value(rec.indexOf("meeting_id")).toInt();
        QString label = req->value(rec.indexOf("meeting_label")).toString();
        QString begin = QDateTime::fromString(req->value(rec.indexOf("meeting_begin")).toString(), "yyyy-MM-dd hh:mm").toString("dd/MM/yyyy hh:mm");
        QString end = QDateTime::fromString(req->value(rec.indexOf("meeting_end")).toString(), "yyyy-MM-dd hh:mm").toString("dd/MM/yyyy hh:mm");
        QString periodic;
        if(req->value(rec.indexOf("meeting_periodic")).toInt() == 0){
            periodic = "Ponctuel";
        }
        else if(req->value(rec.indexOf("meeting_periodic")).toInt() == 1){
            periodic = "Hebdomadaire";
        }
        else{
            periodic = "Mensuel";
        }

        QString organizer = req->value(rec.indexOf("user_surname")).toString() + " " + req->value(rec.indexOf("user_name")).toString();

        QPushButton *btn_accept = new QPushButton("Participera");
        QPushButton *btn_decline = new QPushButton("Ne participera pas");

        connect(btn_accept, SIGNAL(clicked()), mapper_accept, SLOT(map()));
        mapper_accept->setMapping(btn_accept, id_meeting);

        connect(btn_decline, SIGNAL(clicked()), mapper_decline, SLOT(map()));
        mapper_decline->setMapping(btn_decline, id_meeting);

        QTableWidgetItem *twi_id_meeting = new QTableWidgetItem(id_meeting);
        QTableWidgetItem *twi_label = new QTableWidgetItem(label);
        QTableWidgetItem *twi_begin = new QTableWidgetItem(begin);
        QTableWidgetItem *twi_end = new QTableWidgetItem(end);
        QTableWidgetItem *twi_periodic = new QTableWidgetItem(periodic);
        QTableWidgetItem *twi_organizer = new QTableWidgetItem(organizer);

        j = 0;
        table->insertRow(i);

        table->setItem(i, j, twi_id_meeting);
        table->setItem(i, j+1, twi_label);
        table->setItem(i, j+2, twi_begin);
        table->setItem(i, j+3, twi_end);
        table->setItem(i, j+4, twi_periodic);
        table->setItem(i, j+5, twi_organizer);
        table->setCellWidget(i, j+6, btn_accept);
        table->setCellWidget(i, j+7, btn_decline);

        i++;
    }

    connect(mapper_accept, SIGNAL(mapped(int)), this, SLOT(acceptMeeting(int)));
    connect(mapper_decline, SIGNAL(mapped(int)), this, SLOT(declineMeeting(int)));

    if(i == 0)
        lb_message->setText("Pas de nouveaux évenements.");
    else
        lb_message->setText("Vous avez des notifications à traiter.");
}

void TabItemNews::acceptMeeting(int id_meeting){

}

void TabItemNews::declineMeeting(int id_meeting){

}
