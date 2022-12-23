#include "qvklineedit.h"

QVkLineEdit::QVkLineEdit(bool inc, QWidget * parent) :
    QLineEdit(parent)
{
    vk = QVirtualKeyboard::vk;
    isIncluded = inc;
    vk->hide();    
}

QVkLineEdit::~QVkLineEdit()
{
}

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
