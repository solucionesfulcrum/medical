#include "diskspace.h"

diskSpace::diskSpace(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout *v = new QHBoxLayout(this);
    v->setMargin(0);
    v->setSpacing(0);
    setFixedSize(100,60);
    connect(&poller,SIGNAL(timeout()),this,SLOT(refresh()));



    bat = new QLabel("",this);
    bat->setObjectName("diskspacelabel");
    bat->setPixmap(QPixmap(":/icon/res/img/footer/disk.png"));


    pg = new QProgressBar;
    pg->setObjectName("diskspace");
    pg->setFixedHeight(26);
    pg->setRange(0,100);
    pg->setTextVisible(true);
    pg->setAlignment(Qt::AlignCenter);
    pg->setOrientation(Qt::Horizontal);



    v->addWidget(bat,0,Qt::AlignVCenter);
    v->addSpacing(1);
    v->addWidget(pg,5,Qt::AlignVCenter);

    poller.start(60000);
    refresh();

}

void diskSpace::refresh(){
    QStorageInfo storage = QStorageInfo::root();
    int freespace = (double)storage.bytesAvailable()/ (double) storage.bytesTotal() * 100;
    pg->setValue(100-freespace);

    if(freespace < 10)
        pg->setProperty("alert","true");
    else  pg->setProperty("alert","false");
    pg->style()->unpolish(pg);
    pg->style()->polish(pg);
    bat->raise();

}
