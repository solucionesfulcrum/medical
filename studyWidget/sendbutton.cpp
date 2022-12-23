#include "sendbutton.h"

sendbutton::sendbutton(QWidget * parent) : QPushButton(parent)
{
    setFixedSize(400,400);
    time = "00:00";
}
void sendbutton::setTime(QString s){
    time = s;
}

void sendbutton::paintEvent(QPaintEvent *){

    QRect textRect(0,100,400,80);
    QRect ButtonRect(120,200,160,60);
    QRect ButtonTextRect(195,200,85,60);
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::HighQualityAntialiasing );

    //Draw time Circle
    QRect r(44,44,316,316);
    QConicalGradient gradient;
    gradient.setCenter(r.center());
    gradient.setAngle(90);
    gradient.setColorAt(0, QColor("#00D4D8"));
    gradient.setColorAt(0.6, QColor("#009BA8"));
    gradient.setColorAt(1, QColor("#00D4D8"));


    QPen pen(QBrush(gradient), 16, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter.setPen(pen);
    painter.drawEllipse(r);

    pen.setColor(QColor("#00D4D8"));
    painter.setPen(pen);


    painter.drawRoundedRect(ButtonRect,5,5);
    painter.fillRect(ButtonRect,QBrush(QColor("#00D4D8")));

    pen.setWidth(3);
    pen.setColor(QColor("#FFFFFF"));
     painter.setPen(pen);
    QPoint a(140,228);
    QPoint b(170,215);
    QPoint c(157,248);
    QPoint d(152,235);

    QPointF playPoints[4] = {a,b,c,d};
    painter.drawPolygon(playPoints,4);

    painter.setPen(pen);
    painter.drawText(ButtonTextRect,Qt::AlignLeft | Qt::AlignVCenter,tr("Enviar \ny seguir"));


    pen.setColor(QColor("#333333"));
    painter.setPen(pen);
    painter.setFont(QFont("Arial",12));
    painter.drawText(textRect,Qt::AlignCenter,tr("Grabación \nFinalizada")+"\n"+time);


}
