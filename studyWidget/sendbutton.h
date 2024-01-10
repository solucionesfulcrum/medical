#ifndef SENDBUTTON_H
#define SENDBUTTON_H

#include <QtWidgets>
#include <QCoreApplication>

class sendbutton : public QPushButton
{
public:
    sendbutton(QWidget * parent = 0);
    void setTime(QString);
    QString text;

protected:
    void paintEvent(QPaintEvent *);

private:
    QString time;
};

#endif // SENDBUTTON_H
