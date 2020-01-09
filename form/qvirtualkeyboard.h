#ifndef QVIRTUALKEYBOARD_H
#define QVIRTUALKEYBOARD_H

#include <QtWidgets>
#include "accesor.h"



class QVirtualKeyboard : public QWidget
{
    Q_OBJECT
public:
    QVirtualKeyboard(QWidget * parent = 0);
    ~QVirtualKeyboard();
    void setText(QString);
    void setLineEdit(QLineEdit *);
    static QVirtualKeyboard *vk;
    void setVisibleKeyboard(bool);
    void show();
    bool visibleKeyBoard();
protected:
    void showEvent(QShowEvent * event);
    //void focusOutEvent(QFocusEvent *);
public slots:
    void finish();
private slots:
   // void finish();
     void remove();
    void doBackspace();
    void doSend();
    void writeletter(QAbstractButton *b);
    void cancel();
    void validate();

signals:
    void send(QString s);
private:
    bool _visibleKeyboard;
    QPropertyAnimation *animation;
    QLineEdit *toEdit;
    QLineEdit * edit;
    QPushButton * createButton(QString );
    QButtonGroup *keyboard;
    QString entervalue;
};



#endif // QVIRTUALKEYBOARD_H
