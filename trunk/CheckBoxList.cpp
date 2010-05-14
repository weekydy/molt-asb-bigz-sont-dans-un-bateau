#include "CheckBoxList.h"
#include <QtGui>

// internal private delegate
class CheckBoxListDelegate : public QItemDelegate
{
public:
    CheckBoxListDelegate(QObject *parent = 0) : QItemDelegate(parent){}

    QWidget *CheckBoxListDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/* option */, const QModelIndex &/* index */) const
    {
     QCheckBox *editor = new QCheckBox(parent);

     return editor;
    }

     void CheckBoxListDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
 {

     QCheckBox *checkBox = static_cast<QCheckBox*>(editor);
    checkBox->setText(index.data(Qt::DisplayRole).toString());

    checkBox->setChecked(index.data(Qt::UserRole).toBool());

 }
/*
 void setEditorData(QWidget *editor, const QModelIndex &index) const{

    //set editor data

    QCheckBox *checkBox = static_cast<QCheckBox*>(editor);

    checkBox->setText(index.data(Qt::DisplayRole).toString());

    checkBox->setChecked(index.data(Qt::UserRole).toBool());

}*/

  void CheckBoxListDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
 {
     QCheckBox *checkBox = static_cast<QCheckBox*>(editor);
     //spinBox->interpretText();
     bool value = checkBox->isChecked();

    QMap<int,QVariant> data;
    data.insert(Qt::DisplayRole,checkBox->text());
    data.insert(Qt::UserRole,value);

     model->setItemData(index, data);
 }
  void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const{

            //Get item data
            bool value = index.data(Qt::UserRole).toBool();
            QString text = index.data(Qt::DisplayRole).toString();

            // fill style options with item data
            const QStyle *style = QApplication::style();
            QStyleOptionButton opt;
            opt.state |= value ? QStyle::State_On : QStyle::State_Off;
            opt.state |= QStyle::State_Enabled;
            opt.text = text;
            opt.rect = option.rect;

            // draw item data as CheckBox
            style->drawControl(QStyle::CE_CheckBox,&opt,painter);

      }
/*
  void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
       {
             //get the value from the editor (CheckBox)
             QCheckBox *myEditor = static_cast<QCheckBox*>(editor);
             bool value = myEditor->isChecked();

             //set model data
             QMap<int,QVariant> data;
             data.insert(Qt::DisplayRole,myEditor->text());
             data.insert(Qt::UserRole,value);
             model->setItemData(index,data);
       }

  */


 void CheckBoxListDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
 {
     editor->setGeometry(option.rect);
 }

/*
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        //Get item data
        bool value = index.data(Qt::UserRole).toBool();
        QString text = index.data(Qt::DisplayRole).toString();

        // fill style options with item data
        const QStyle *style = QApplication::style();
        QStyleOptionButton opt;
        opt.state |= value ? QStyle::State_On : QStyle::State_Off;
        opt.state |= QStyle::State_Enabled;
        opt.text = text;
        opt.rect = option.rect;

        // draw item data as CheckBox
        style->drawControl(QStyle::CE_CheckBox,&opt,painter);
    }





    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
    {
        //get the value from the editor (CheckBox)
        QCheckBox *myEditor = static_cast<QCheckBox*>(editor);
        bool value = myEditor->isChecked();

        //set model data
        QMap<int,QVariant> data;
        data.insert(Qt::DisplayRole,myEditor->text());
        data.insert(Qt::UserRole,value);
        model->setItemData(index,data);
    }
    void setEditorData(QWidget *editor, const QModelIndex &index) const
    {
        //set editor data
        QCheckBox *myEditor = static_cast<QCheckBox*>(editor);
        myEditor->setText(index.data(Qt::DisplayRole).toString());
        myEditor->setChecked(index.data(Qt::UserRole).toBool());
    }
*/
};



CheckBoxList::CheckBoxList(QWidget *parent) : QComboBox(parent), m_DisplayText(0)
{
        // set delegate items view
	view()->setItemDelegate(new CheckBoxListDelegate(this));
	
	// Enable editing on items view
	view()->setEditTriggers(QAbstractItemView::CurrentChanged);
	
	// set "CheckBoxList::eventFilter" as event filter for items view 
        view()->viewport()->installEventFilter(this);
	
	// it just cool to have it as defualt ;)
        //view()->setAlternatingRowColors(true);

}


CheckBoxList::~CheckBoxList(){}


/*bool CheckBoxList::eventFilter(QObject *object, QEvent *event)
{
    // don't close items view after we release the mouse button
    // by simple eating MouseButtonRelease in viewport of items view
    if(event->type() == QEvent::MouseButtonRelease && object==view()->viewport())
    {
        return true;
    }
    return QComboBox::eventFilter(object,event);
}

*/
/*
void CheckBoxList::paintEvent(QPaintEvent *)
{
    QStylePainter painter(this);
    painter.setPen(palette().color(QPalette::Text));

    // draw the combobox frame, focusrect and selected etc.
    QStyleOptionComboBox opt;
    initStyleOption(&opt);

	// if no display text been set , use "..." as default
	if(m_DisplayText.isNull())
            opt.currentText = "...";
        else
            opt.currentText = m_DisplayText;
    painter.drawComplexControl(QStyle::CC_ComboBox, opt);

    // draw the icon and text
    painter.drawControl(QStyle::CE_ComboBoxLabel, opt);
}

void CheckBoxList::setDisplayText(QString text)
{
    m_DisplayText = text;
}

QString CheckBoxList::getDisplayText() const
{
	return m_DisplayText;
}*/
