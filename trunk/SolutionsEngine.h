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
        int findRoom(int id_people, bool guest);
};

#endif // SOLUTIONSENGINE_H
