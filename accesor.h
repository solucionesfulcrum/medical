#ifndef ACCESOR_H
#define ACCESOR_H

#include <QMainWindow>
#include <QGraphicsBlurEffect>
#include <QGraphicsEffect>
class accesor
{
public:
    static QMainWindow* mw;

    static void startEffect(int r = 5);
    static void stopEffect();
};

#endif // ACCESOR_H
