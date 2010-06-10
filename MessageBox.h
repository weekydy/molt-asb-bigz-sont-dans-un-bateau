#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#include <QtCore>
#include <QtGui>

class MessageBox : public QMessageBox
{
    public:
        MessageBox();
        virtual ~MessageBox();
    protected:
    private:
};

#endif // MESSAGEBOX_H
