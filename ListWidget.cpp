#include <QtGui>

#include "ListWidget.h"

ListWidget::ListWidget(QWidget *parent)
    : QListWidget(parent)
{
    setAcceptDrops(true);
}

void ListWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        startPos = event->pos();
    QListWidget::mousePressEvent(event);
}

void ListWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        int distance = (event->pos() - startPos).manhattanLength();
        if (distance >= QApplication::startDragDistance())
            startDrag();
    }
    QListWidget::mouseMoveEvent(event);
}

void ListWidget::dragEnterEvent(QDragEnterEvent *event)
{
    ListWidget *source =
            qobject_cast<ListWidget *>(event->source());
    if (source && source != this) {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}

void ListWidget::dragMoveEvent(QDragMoveEvent *event)
{
    ListWidget *source =
            qobject_cast<ListWidget *>(event->source());
    if (source && source != this) {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}

void ListWidget::dropEvent(QDropEvent *event)
{
    ListWidget *source =
            qobject_cast<ListWidget *>(event->source());
    if (source && source != this) {
        addItem(event->mimeData()->text());
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}

void ListWidget::startDrag()
{
    QListWidgetItem *item = currentItem();
    if (item) {
        QMimeData *mimeData = new QMimeData;
        mimeData->setText(item->text());

        QDrag *drag = new QDrag(this);
        drag->setMimeData(mimeData);
        drag->setPixmap(QPixmap(":/images/person.png"));
        if (drag->start(Qt::MoveAction) == Qt::MoveAction)
            delete item;
    }
}
