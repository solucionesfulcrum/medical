#include "capturebutton.h"

captureButton::captureButton(QWidget * parent) : QWidget(parent)
{
    time = tr("00:00");
    msTime = 0;
    info = tr("");
    isRecord = false;
    mousePressed = false;
    isBloqued = false;
    setFixedSize(400,400);

    tiempoMax = 60000;

    playWidth = 30;
    playHeight = 30;
    timeWidth = 100;
    timeHeight = 100;
    wstart = 185;
    hstart = 185;

    changeH = playHeight/2;
    textRect = QRectF(0,260,400,30);
    infoRect = QRectF(0,370,400,30);


    connect(&poller,SIGNAL(timeout()),this,SLOT(repaint()));
    update();
}

captureButton::~captureButton()
{

}

void captureButton::setMaxTime(int i){
    tiempoMax = i;
}

void captureButton::setTimeMs(int ms){
    msTime = ms;
}

void captureButton::mousePressEvent(QMouseEvent  * event){

    if(event->button() == Qt::LeftButton ){
        mousePressed = true;
        update();
    }
}

void captureButton::mouseReleaseEvent(QMouseEvent * event){
    //qDebug() << event << isBloqued;
    if(event->button() == Qt::LeftButton ){
        mousePressed = false;
        if(!isBloqued)
            emit clicked();
        update();
    }
}

void captureButton::paintEvent(QPaintEvent *){
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::HighQualityAntialiasing );

    //Draw large circle
    QPen largeCirclePen(QColor("#CBE8EA"), 8, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter.setPen(largeCirclePen);
    painter.drawEllipse(40,40,320,320);

    //Draw time Circle
    int angle = - (msTime*5760/tiempoMax);
    QRect r(44,44,316,316);
    QConicalGradient gradient;
        gradient.setCenter(r.center());
        gradient.setAngle(90);
        gradient.setColorAt(0, QColor("#00D4D8"));
        gradient.setColorAt(0.6, QColor("#009BA8"));
        gradient.setColorAt(1, QColor("#00D4D8"));

    int initangle = 90*16;
    QPen largeCircleOverPen(QBrush(gradient), 16, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter.setPen(largeCircleOverPen);
    painter.drawArc(r,initangle,angle);


    //Draw little circle
    QPen smallCirclePen(QColor("#00D4D8"), 6, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter.setPen(smallCirclePen);
    painter.drawEllipse(150,150,100,100);

    //Draw Button


    //Square
    a.setX(wstart);
    a.setY(hstart);

    b.setX(wstart+playWidth);
    b.setY(hstart);

    c.setX(wstart+playWidth);
    c.setY(hstart+playHeight);

    d.setX(wstart);
    d.setY(hstart+playHeight);


    int tri = 5;
     //triangle
     if (!isRecord){
         a.setX(wstart+tri);
         b.setX(wstart+tri+playWidth);
         b.setY(hstart+(playWidth/2));
         c.setX(wstart+tri+playWidth);
         c.setY(hstart+(playWidth/2));
         d.setX(wstart+tri);
     }




    QPointF playPoints[4] = {a,b,c,d};
    QPen pen(QColor("#00D4D8"), 4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

    /*Play draw*/
    if(isRecord)
        pen.setColor(QColor(231,76,60,255));

    if(mousePressed)
        pen.setColor(QColor(241,196,15));

    if(isBloqued)
        pen.setColor(QColor(52,152,219,255));

    painter.setBrush(QBrush(QColor("#00D4D8")));
    painter.setPen(pen);
    painter.drawPolygon(playPoints,4);

    pen.setColor(QColor("#333333"));
    painter.setPen(pen);


    /*Draw Time*/
    painter.setFont(QFont("Arial",15));
    painter.drawText(textRect,Qt::AlignCenter,time);

    /*Draw Time*/
    painter.setFont(QFont("Arial",15));
    painter.drawText(infoRect,Qt::AlignCenter,info);



}

void captureButton::setTime(QString s){
    time = s;
    update();
}

void captureButton::setInfo(QString s){
    info = s;
    update();
}

void captureButton::setRecord(bool b){
    isRecord = b;
    //poller.start(2);
    update();
}

void captureButton::setBlock(bool b){
    isBloqued = b;
    update();
}

bool captureButton::isBlocked(){
    return isBloqued;
}

