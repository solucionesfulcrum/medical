#include "battery.h"

battery::battery(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout *v = new QHBoxLayout(this);
    v->setMargin(0);
    v->setSpacing(0);
    setFixedHeight(60);
    connect(&poller,SIGNAL(timeout()),this,SLOT(refresh()));

    pg = new QProgressBar;
    pg->setObjectName("batteryProgress");
    pg->setFixedSize(49,26);
    pg->setRange(0,100);
    pg->setTextVisible(false);
    pg->setOrientation(Qt::Horizontal);

    ac = new QLabel(this);
    ac->setPixmap(QPixmap(":/icon/res/img/battery.png"));
    ac->move(-5,5);
    bat = new QLabel("",this);
    bat->setFixedSize(3,10);
    bat->setObjectName("battery");
    v->addSpacing(10);
    v->addWidget(pg,0,Qt::AlignVCenter | Qt::AlignCenter);
    v->addWidget(bat,0,Qt::AlignLeft | Qt::AlignCenter);

    poller.start(1000);
    refresh();
}

void battery::refresh(){

    GetSystemPowerStatus(&status);
    int p = (int) status.BatteryLifePercent;
    if(p == 255)
        pg->setValue(100);
    else pg->setValue(p);

    if(status.ACLineStatus == 1 )
        ac->show();
    else ac->hide();
    ac->raise();
}
