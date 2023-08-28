#ifndef QVKLINEEDIT_H
#define QVKLINEEDIT_H

#include <QtWidgets>
#include <QAction>
#include <QToolButton>
#include <form/qvirtualkeyboard.h>

class QVkLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    QVkLineEdit(bool inc = false,bool show=false, QWidget *parent = 0);
    QVirtualKeyboard *vk;
    ~QVkLineEdit();
    void mousePressEvent(QMouseEvent *);
    QVirtualKeyboard * virtualKeyboard();


protected:
    void focusOutEvent(QFocusEvent *e);
    void keyPressEvent(QKeyEvent *event);

public slots:
    void onClicked(void);
    //void onPressed(void);
    //void onReleased(void);


private slots:
    void changeValue(QString s);


private:
    bool hide;
    bool isIncluded;
    QToolButton *button;
};

#endif // QVKLINEEDIT_H
