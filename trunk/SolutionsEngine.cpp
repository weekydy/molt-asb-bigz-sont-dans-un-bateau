#include "SolutionsEngine.h"

SolutionsEngine::SolutionsEngine() {}

void SolutionsEngine::findHours(QDateTime date, int duration, int id_room, QList<int> id_people)
{
    QSqlQuery *req = new QSqlQuery();
    req->prepare("SELECT meeting_begin, meeting_end FROM meeting m WHERE m.room_id = 2 AND strftime('%Y', m.meeting_begin) = :year AND strftime('%m', m.meeting_begin) = :month AND strftime('%d', m.meeting_begin) = :day");
    req->bindValue(":year", date.date().year());
    req->bindValue(":month", date.date().month());
    req->bindValue(":day", date.date().day());
    req->exec();

    QBitArray qba_room(48);

    while (req->next())
    {

        QDateTime qdtB = QDateTime::fromString(req->value(0).toString(), "yyyy-MM-dd hh:mm");
        QDateTime qdtE = QDateTime::fromString(req->value(1).toString(), "yyyy-MM-dd hh:mm");

        QTime qtB = QTime::fromString(qdtB.toString("hh:mm"));
        QTime qtE = QTime::fromString(qdtE.toString("hh:mm"));

        int noRow_b = (qtB.hour() - 8) * 4 + qtB.minute() / 15; // numéro de la ligne où le RV commence
        int noRow_e = (qtE.hour() - 8) * 4 + qtE.minute() / 15 - 1; // numéro de la ligne où le RV termine (le -1 indique qu'on ne commence pas le dernier quart d'heure)

        for (int i = 0; i < qba_room.size(); ++i)
        {
            if (i >= noRow_b && i <= noRow_e)
                qba_room.setBit(i, true);
        }
    }

    QList<QBitArray*> ql_qba;

    for (int i = 0; i < id_people.size(); ++i)
    {
        req->prepare("SELECT * FROM user u INNER JOIN havemeeting hm ON u.user_id = hm.user_id INNER JOIN meeting m ON m.meeting_id = hm.meeting_id WHERE u.user_id = :id_people");
        req->bindValue(":id_peolple", i);
        req->exec();

        while (req->next())
        {

            QDateTime qdtB = QDateTime::fromString(req->value(0).toString(), "yyyy-MM-dd hh:mm");
            QDateTime qdtE = QDateTime::fromString(req->value(1).toString(), "yyyy-MM-dd hh:mm");

            QTime qtB = QTime::fromString(qdtB.toString("hh:mm"));
            QTime qtE = QTime::fromString(qdtE.toString("hh:mm"));

            QBitArray* qba_room = new QBitArray(48);

            int noRow_b = (qtB.hour() - 8) * 4 + qtB.minute() / 15; // numéro de la ligne où le RV commence
            int noRow_e = (qtE.hour() - 8) * 4 + qtE.minute() / 15 - 1; // numéro de la ligne où le RV termine (le -1 indique qu'on ne commence pas le dernier quart d'heure)

            for (int j = 0; j < qba_room->size(); ++j)
            {
                if (j >= noRow_b && j <= noRow_e)
                    qba_room->setBit(j, true);
            }
            ql_qba.push_back(qba_room);
        }


    }
}
