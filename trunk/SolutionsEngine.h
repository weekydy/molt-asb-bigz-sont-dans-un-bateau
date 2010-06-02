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

        void findHours(QDateTime date, int duration, int id_room, QList<int> id_people);
};

#endif // SOLUTIONSENGINE_H
