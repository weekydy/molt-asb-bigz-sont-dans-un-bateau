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

        QDateTime findHours(QDateTime date, int duration, int id_room, QList<int> id_people);
        int findRoom(int id_people);
};

#endif // SOLUTIONSENGINE_H
