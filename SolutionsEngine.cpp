#include "SolutionsEngine.h"
#include <iostream>
#include <sstream>

SolutionsEngine::SolutionsEngine() {}

SolutionsEngine::~SolutionsEngine() {}

QDateTime SolutionsEngine::findHours(QDateTime date, QTime duration, int id_room, QList<int> id_people, bool available, bool extend)
{
    int begin = -1;
    qDebug() << duration;
    if (duration.hour() > 11)
    {
        date.setTime(QTime(0,0,0,0));
        return date;
    }
    while(begin < 0 )
    {
        //Requette retournant toutes les réunions à un jour donné
        QSqlQuery *req = new QSqlQuery();
        req->prepare("SELECT meeting_begin, meeting_end FROM meeting m WHERE m.room_id = 2 AND strftime('%Y', m.meeting_begin) = :year AND strftime('%m', m.meeting_begin) = :month AND strftime('%d', m.meeting_begin) = :day");
        req->bindValue(":year", date.date().toString("yyyy"));
        req->bindValue(":month", date.date().toString("MM"));
        req->bindValue(":day", date.date().toString("dd"));
        req->exec();

        QBitArray qba_room(48);

        //Recherche des disponibilités d'une salle
        while (req->next())
        {
            QDateTime qdtB = QDateTime::fromString(req->value(0).toString(), "yyyy-MM-dd hh:mm");
            QDateTime qdtE = QDateTime::fromString(req->value(1).toString(), "yyyy-MM-dd hh:mm");

            QTime qtB = QTime::fromString(qdtB.toString("hh:mm"), "hh:mm");
            QTime qtE = QTime::fromString(qdtE.toString("hh:mm"), "hh:mm");

            int noRow_b = (qtB.hour() - 8) * 4 + qtB.minute() / 15; // numéro de la ligne où le RV commence
            int noRow_e = (qtE.hour() - 8) * 4 + qtE.minute() / 15 - 1; // numéro de la ligne où le RV termine (le -1 indique qu'on ne commence pas le dernier quart d'heure)

            //Remplissage du bitarray de la salle
            for (int i = 0; i < qba_room.size(); ++i)
            {
                if (i >= noRow_b && i <= noRow_e)
                    qba_room.setBit(i, true);
            }
        }

        //Liste des bitarray des invités
        QList<QBitArray*> ql_qba;
        if (available)
        {
            //Recherche des disponibilités des invités
            for (int i = 0; i < id_people.size(); ++i)
            {
                req->prepare("SELECT * FROM user u INNER JOIN havemeeting hm ON u.user_id = hm.user_id INNER JOIN meeting m ON m.meeting_id = hm.meeting_id WHERE u.user_id = :id_people  AND strftime('%Y', m.meeting_begin) = :year AND strftime('%m', m.meeting_begin) = :month AND strftime('%d', m.meeting_begin) = :day");
                req->bindValue(":id_peolple", QString::number(i));
                req->bindValue(":year", date.date().toString("yyyy"));
                req->bindValue(":month", date.date().toString("MM"));
                req->bindValue(":day", date.date().toString("dd"));
                req->exec();
                QSqlRecord rec = req->record();

                while (req->next())
                {
                    QDateTime qdtB = QDateTime::fromString(req->value(rec.indexOf("meeting_begin")).toString(), "yyyy-MM-dd hh:mm");
                    QDateTime qdtE = QDateTime::fromString(req->value(rec.indexOf("meeting_end")).toString(), "yyyy-MM-dd hh:mm");

                    QTime qtB = QTime::fromString(qdtB.toString("hh:mm"), "hh:mm");
                    QTime qtE = QTime::fromString(qdtE.toString("hh:mm"), "hh:mm");

                    QBitArray* qba_people = new QBitArray(48);

                    int noRow_b = (qtB.hour() - 8) * 4 + qtB.minute() / 15; // numéro de la ligne où le RV commence
                    int noRow_e = (qtE.hour() - 8) * 4 + qtE.minute() / 15 - 1; // numéro de la ligne où le RV termine (le -1 indique qu'on ne commence pas le dernier quart d'heure)

                    //Remplissage du bitarray de l'invité
                    for (int j = 0; j < qba_people->size(); ++j)
                    {
                        if (j >= noRow_b && j <= noRow_e)
                            qba_people->setBit(j, true);
                        //std::cout << qba_people->at(j);
                    }
                    ql_qba.push_back(qba_people);
                }
            }

            QList<QBitArray*>::iterator it_end = ql_qba.end();
            for (QList<QBitArray*>::iterator it = ql_qba.begin(); it != it_end; ++it)
            {
                qba_room |= **it;
            }
        }

        for (int i = 0; i < qba_room.size() - 1; ++i)
        {
            if (qba_room[i] && !qba_room[i+1]) {
                qba_room.setBit(i+1, true);
                qba_room.setBit(i+2, true);
                i += 2;
            }
            if (!qba_room[i] && qba_room[i+1]) {
                qba_room.setBit(i, true);
                if (i)
                    qba_room.setBit(i-1, true);
            }
        }

        int qba_room_size = qba_room.size() - duration.hour() * 4;
        for (int i = 0; i < qba_room_size; ++i)
        {
            if (!qba_room[i])
            {
                int time = i+duration.hour() * 4;
                begin = i;
                for (int j = i; j < time; ++j)
                {
                    if (qba_room[j])
                    {
                        j = time;
                        begin = -1;
                    }
                }
                if (begin >= 0)
                    i = qba_room_size;
            }
        }
        ql_qba.clear();
        date = date.addDays(1);
        if (!extend && begin < 0)
        {
            date.setTime(QTime(0,0,0,0));
            return date;
        }
    }

    int minute = (begin % 4) * 15 ; // numéro de la ligne où le RV commence
    int hour = begin / 4 + 8; // numéro de la ligne où le RV termine (le -1 indique qu'on ne commence pas le dernier quart d'heure)
    date = date.addDays(-1);
    date.setTime(QTime(hour, minute, 0, 0));

    return date;
}

int SolutionsEngine::findRoom(int nb_guest)
{
    std::ostringstream out;
    out << nb_guest;
    //Requette retournant toutes les réunions à un jour donné
    QSqlQuery *req = new QSqlQuery();
    req->prepare("SELECT * FROM room r WHERE r.room_capacity >= :nb ORDER BY r.room_capacity");
    req->bindValue(":nb", QString::fromStdString(out.str()));
    req->exec();

    req->first();

    return req->value(0).toInt();
}
