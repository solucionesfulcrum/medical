#ifndef QVKLINEEDIT_H
#define QVKLINEEDIT_H

#include <QtWidgets>
#include <form/qvirtualkeyboard.h>

class QVkLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    QVkLineEdit(bool inc = false, QWidget *parent = 0);
    QVirtualKeyboard *vk;
    ~QVkLineEdit();
    void mousePressEvent(QMouseEvent *);
    QVirtualKeyboard * virtualKeyboard();


protected:
    void focusOutEvent(QFocusEvent *e);
    void keyPressEvent(QKeyEvent *event);

private slots:
    void changeValue(QString s);

private:
   bool isIncluded;
};

#endif // QVKLINEEDIT_H
