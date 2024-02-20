#include "qvklineedit.h"

QVkLineEdit::QVkLineEdit(bool inc,bool show, QWidget * parent) :
    QLineEdit(parent)
{
    vk = QVirtualKeyboard::vk;
    isIncluded = inc;
    vk->hide();
    hide = false;

//---------------------------------------------------------------------
//  CR: 02/07/23
    if(show==true)
    {
        QAction *action = addAction(QIcon(":/icon/res/img/hide.png"), QLineEdit::TrailingPosition);
        button = qobject_cast<QToolButton *>(action->associatedWidgets().last());
        connect(button, SIGNAL(clicked()), this, SLOT(onClicked()));
        //connect(button, SIGNAL(pressed()), this, SLOT(onPressed()));
        //connect(button, SIGNAL(released()), this, SLOT(onReleased()));
    }

//---------------------------------------------------------------------
}

QVkLineEdit::~QVkLineEdit()
{
}

void QVkLineEdit::onClicked(void)
{
    hide = !hide;

    if(hide)
    {
        button->setIcon(QIcon(":/icon/res/img/view.png"));
        setEchoMode(QLineEdit::Normal);
    }
    else
    {
        button->setIcon(QIcon(":/icon/res/img/hide.png"));
        setEchoMode(QLineEdit::Password);
    }
}

/*
//---------------------------------------------------------------------
//  CR: 02/07/23
void QVkLineEdit::onPressed(void)
{    
    button->setIcon(QIcon(":/icon/res/img/view.png"));
    setEchoMode(QLineEdit::Normal);
}

void QVkLineEdit::onReleased(void)
{    
    button->setIcon(QIcon(":/icon/res/img/hide.png"));
    setEchoMode(QLineEdit::Password);
}
*/
//---------------------------------------------------------------------

void QVkLineEdit::changeValue(QString s){
    setText(s);
}

void QVkLineEdit::mousePressEvent(QMouseEvent * event){
    if(event->button() == Qt::LeftButton ){
        vk->show();
        vk->setLineEdit((QLineEdit*) this);
        vk->setText(text());        
    }
}

QVirtualKeyboard* QVkLineEdit::virtualKeyboard(){
    return vk;
}

void QVkLineEdit::focusOutEvent(QFocusEvent * e){
    vk->close();
    QLineEdit::focusOutEvent(e);
}

void QVkLineEdit::keyPressEvent(QKeyEvent *event){
    QLineEdit::keyPressEvent(event);
    setText(text().toUpper());
}
