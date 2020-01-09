#include "accesor.h"

QMainWindow* accesor::mw = NULL;

void accesor::startEffect(int r){
    QGraphicsColorizeEffect* e =(QGraphicsColorizeEffect* )accesor::mw->graphicsEffect();
    e->setStrength(1.0);
    e->setColor(QColor(255,255,255,220));

}

void accesor::stopEffect(){
    QGraphicsColorizeEffect* e =(QGraphicsColorizeEffect* )accesor::mw->graphicsEffect();
    e->setStrength(0);
}
