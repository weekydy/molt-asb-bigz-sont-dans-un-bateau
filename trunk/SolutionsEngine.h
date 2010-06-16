#ifndef SOLUTIONSENGINE_H
#define SOLUTIONSENGINE_H

#include <QtCore>
#include <QtGui>
#include <QtSql>

class SolutionsEngine
{
public :
        SolutionsEngine();
        ~SolutionsEngine();

        QDateTime findHours(QDateTime date, QTime duration, int id_room, QSet<int> id_people, bool available, bool extend);
        int findRoom(int nb_people, bool guest, QList<int> equipment_id, bool equipment);
        int verif(QDateTime begin, QDateTime end, int room, QSet<int> people, bool available, bool guest);
        int rateUser(QDate date, int user_id);
};

#endif // SOLUTIONSENGINE_H
