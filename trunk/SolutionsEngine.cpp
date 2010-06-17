#include "SolutionsEngine.h"
#include <iostream>
#include <sstream>

SolutionsEngine::SolutionsEngine() {}

SolutionsEngine::~SolutionsEngine() {}

QDateTime SolutionsEngine::findHours(QDateTime date, QTime duration, int id_room, QSet<int> id_people, bool available, bool extend)
{
    int begin = -1;
    if (duration.hour() > 11)
    {
        date.setTime(QTime(0,0,0,0));
        return date;
    }
    while(begin < 0 )
    {
        std::ostringstream room;
        room << id_room;
        //Requette retournant toutes les réunions à un jour donné
        QSqlQuery *req = new QSqlQuery();
        req->prepare("SELECT meeting_begin, meeting_end FROM meeting m WHERE m.room_id = :id_room AND strftime('%Y', m.meeting_begin) = :year AND strftime('%m', m.meeting_begin) = :month AND strftime('%d', m.meeting_begin) = :day");
        req->bindValue(":year", date.date().toString("yyyy"));
        req->bindValue(":month", date.date().toString("MM"));
        req->bindValue(":day", date.date().toString("dd"));
        req->bindValue(":id_room", QString::fromStdString(room.str()));
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
           foreach (int user_id, id_people)
            {
                req->prepare("SELECT * FROM user u INNER JOIN havemeeting hm ON u.user_id = hm.user_id INNER JOIN meeting m ON m.meeting_id = hm.meeting_id WHERE u.user_id = :id_people  AND strftime('%Y', m.meeting_begin) = :year AND strftime('%m', m.meeting_begin) = :month AND strftime('%d', m.meeting_begin) = :day");
                req->bindValue(":id_peolple", QString::number(user_id));
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

        // Mise à jour du bitarray pour laisser des demi heures entre les réunions
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

        // Mise à jour du bitarray
        int noRow_c = (date.time().hour() - 8) * 4 + date.time().minute() / 15 - 1;
        for (int i = 0; i <= noRow_c && i < 48; ++i)
            qba_room.setBit(i, true);

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

int SolutionsEngine::findRoom(int nb_guest, bool guest, QList<int> equipment_id, bool equipment)
{
    std::ostringstream nb;
    if (guest)
        nb << nb_guest;
    else
        nb << 0;

    QSqlQuery *req = new QSqlQuery();

    if (equipment) {
        QString requete("SELECT DISTINCT(r.room_id) FROM room r INNER JOIN haveequipment he ON r.room_id = he.room_id WHERE ");
        for (int i = 0; i < equipment_id.size(); ++i) {
            if (i == equipment_id.size() - 1)
                requete+="r.room_id IN (SELECT room_id FROM haveequipment WHERE equip_id = " + QString::number(equipment_id[i]) + ")";
            else
                requete+="r.room_id IN (SELECT room_id FROM haveequipment WHERE equip_id = " + QString::number(equipment_id[i]) + ") AND ";
        }
        req->prepare(requete);
        req->exec();

        if (guest) {
            while (req->next()) {
                QSqlQuery *req1 = new QSqlQuery();
                //Requette retournant toutes les salles triées par capacité, > à la demande.
                req1->prepare("SELECT * FROM room r WHERE r.room_capacity >= :nb AND r.room_id = :id ORDER BY r.room_capacity");
                req1->bindValue(":nb", QString::fromStdString(nb.str()));
                req1->bindValue(":id", req->value(0).toInt());
                req1->exec();

                if (req1->first())
                    return req1->value(0).toInt();
            }
            return 0;
        }
        req->first();
        return req->value(0).toInt();
    }
    //Requette retournant toutes les salles triées par capacité, > à la demande.
    req->prepare("SELECT * FROM room r WHERE r.room_capacity >= :nb ORDER BY r.room_capacity");
    req->bindValue(":nb", QString::fromStdString(nb.str()));
    req->exec();

    req->first();

    return req->value(0).toInt();
}

int SolutionsEngine::verif(QDateTime begin, QDateTime end, int id_room, QSet<int> id_people, bool available, bool guest)
{
    int hour = -1;

    std::ostringstream room;
    room << id_room;
    //Requette retournant toutes les réunions à un jour donné
    QSqlQuery *req = new QSqlQuery();
    req->prepare("SELECT meeting_begin, meeting_end FROM meeting m WHERE m.room_id = :id_room AND strftime('%Y', m.meeting_begin) = :year AND strftime('%m', m.meeting_begin) = :month AND strftime('%d', m.meeting_begin) = :day");
    req->bindValue(":year", begin.date().toString("yyyy"));
    req->bindValue(":month", begin.date().toString("MM"));
    req->bindValue(":day", begin.date().toString("dd"));
    req->bindValue(":id_room", QString::fromStdString(room.str()));
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
       foreach (int user_id, id_people)
        {
            req->prepare("SELECT * FROM user u INNER JOIN havemeeting hm ON u.user_id = hm.user_id INNER JOIN meeting m ON m.meeting_id = hm.meeting_id WHERE u.user_id = :id_people  AND strftime('%Y', m.meeting_begin) = :year AND strftime('%m', m.meeting_begin) = :month AND strftime('%d', m.meeting_begin) = :day");
            req->bindValue(":id_peolple", QString::number(user_id));
            req->bindValue(":year", begin.date().toString("yyyy"));
            req->bindValue(":month", begin.date().toString("MM"));
            req->bindValue(":day", begin.date().toString("dd"));
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

    // Mise à jour du bitarray pour laisser des demi heures entre les réunions
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

    //Mise à jour du bitarray si le jour cherché est le jour courrant pour empècher la recherche avant l'heure courrante
    if (begin.date() == QDate::currentDate()) {
        int noRow_c = (QTime::currentTime().hour() - 8) * 4 + QTime::currentTime().minute() / 15;
        for (int i = 0; i < noRow_c && i < 48; ++i)
            qba_room.setBit(i, true);
    }


    int qba_room_size = ((end.time().hour() - 8) * 4) + 1;
    for (int i = ((begin.time().hour() - 8) * 4); i < qba_room_size; ++i)
    {
        if (!qba_room[i])
        {
            int time = (i+((end.time().hour() - 8) * 4 - (begin.time().hour() - 8) * 4));
            hour = i;
            for (int j = i; j < time; ++j)
            {
                if (qba_room[j])
                {
                    j = time;
                    hour = -1;
                }
            }
            if (hour >= 0)
                i = qba_room_size;
        }
    }
    if (hour >= 0)
        return 1;

    return 0;
}

int SolutionsEngine::rateUser(QDate date, int user_id) {
    // recherche de la date de début de semaine
    while(date.dayOfWeek() != 1){ // on cherche le lundi de la semaine
        date = date.addDays(-1);
    }
    float rate = 0;
    while (date.dayOfWeek() != 7) {
        float rateDay = 0;
        QSqlQuery *req = new QSqlQuery();
        req->prepare("SELECT * FROM user u INNER JOIN havemeeting hm ON u.user_id = hm.user_id INNER JOIN meeting m ON m.meeting_id = hm.meeting_id INNER JOIN room r ON r.room_id = m.room_id WHERE hm.hm_state = '1' AND u.user_id = :user_id AND ((strftime('%Y', m.meeting_begin) = :year AND strftime('%m', m.meeting_begin) = :month AND strftime('%d', m.meeting_begin) = :day) OR (m.meeting_periodic > 0)) ORDER BY meeting_begin");
        req->bindValue(":id_peolple", QString::number(user_id));
        req->bindValue(":year", date.toString("yyyy"));
        req->bindValue(":month", date.toString("MM"));
        req->bindValue(":day", date.toString("dd"));
        req->exec();
        QSqlRecord rec = req->record();

        QBitArray qba_room(48);

        while (req->next())
        {
            // Si c'est un rendez-vous periodique hebdomadaire.
            if(req->value(rec.indexOf("meeting_periodic")).toInt() == 1)
            {
                QDateTime meeting_begin = QDateTime::fromString(req->value(rec.indexOf("meeting_begin")).toString(), "yyyy-MM-dd hh:mm");
                if(meeting_begin.date().dayOfWeek() != date.dayOfWeek() || meeting_begin.date().daysTo(date) < 0)
                    continue;
            }
            // Si c'est un rendez-vous periodique mensuel.
            else if(req->value(rec.indexOf("meeting_periodic")).toInt() == 2)
            {
                QDateTime meeting_begin = QDateTime::fromString(req->value(rec.indexOf("meeting_begin")).toString(), "yyyy-MM-dd hh:mm");
                if(meeting_begin.date().daysTo(date) % 28 != 0 || meeting_begin.date().daysTo(date) < 0)
                    continue;
            }

            QDateTime qdtB = QDateTime::fromString(req->value(rec.indexOf("meeting_begin")).toString(), "yyyy-MM-dd hh:mm");
            QDateTime qdtE = QDateTime::fromString(req->value(rec.indexOf("meeting_end")).toString(), "yyyy-MM-dd hh:mm");

            QTime qtB = QTime::fromString(qdtB.toString("hh:mm"), "hh:mm");
            QTime qtE = QTime::fromString(qdtE.toString("hh:mm"), "hh:mm");

            int noRow_b = (qtB.hour() - 8) * 4 + qtB.minute() / 15; // numéro de la ligne où le RV commence
            int noRow_e = (qtE.hour() - 8) * 4 + qtE.minute() / 15 - 1; // numéro de la ligne où le RV termine (le -1 indique qu'on ne commence pas le dernier quart d'heure)

            //Remplissage du bitarray de l'invité
            for (int j = 0; j < 48; ++j)
            {
                if (j >= noRow_b && j <= noRow_e) {
                    qba_room.setBit(j, true);
                    ++rateDay;
                }
            }
        }
        rate += rateDay;
        date = date.addDays(1);
    }
    rate /= 336;
    rate *= 100;
    return (int)rate;

}
