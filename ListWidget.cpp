#include <QtGui>

#include "ListWidget.h"

ListWidget::ListWidget(QString _url, QWidget *parent)
    : QListWidget(parent)
{
    url = _url;
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
        QListWidgetItem *item = new QListWidgetItem();
        item->setText(event->mimeData()->text());
        item->setData(Qt::UserRole, event->mimeData()->data("id").toInt());
        addItem(item);
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
        mimeData->setData("id", item->data(Qt::UserRole).toByteArray());

        QDrag *drag = new QDrag(this);
        drag->setMimeData(mimeData);
        drag->setPixmap(QPixmap(url));
        if (drag->start(Qt::MoveAction) == Qt::MoveAction){
            delete item;
            emit notifyNewItem();
        }
    }
}
