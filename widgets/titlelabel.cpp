#include "titlelabel.h"

titlelabel::titlelabel(QString t, QWidget *parent) : QWidget(parent)
{
    title = t;
    setFixedSize(300,60);
    line = true;
    update();
    lineWidth = 120;
    textalign = Qt::AlignCenter;
}

void titlelabel::setText(QString t){
    title = t;
    update();
}
void titlelabel::setLine(bool t){
    line = t;
    update();
}

void titlelabel::setLineWidth(int i){
    lineWidth = i;
    update();
}

void titlelabel::setAlignment(Qt::Alignment a){
    textalign = a;
    update();
}

void titlelabel::paintEvent(QPaintEvent *){
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::HighQualityAntialiasing );

    QPen pen(QColor("#00D4D8"), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);


    pen.setColor(QColor("#444444"));
    /*Draw Time*/
    painter.setPen(pen);

    painter.setFont(QFont("Arial",15));
    painter.drawText(QRect(0,0,width(),40),textalign | Qt::AlignVCenter ,title);

    int diff = (width()-lineWidth)/2;
    if(textalign == Qt::AlignLeft)
        diff = 0;
    pen.setColor(QColor("#00D4D8"));
    painter.setPen(pen);
    if(line)
        painter.drawRect(diff,38,lineWidth,2);

}
