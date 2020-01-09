#include "studyprotocols.h"


studyProtocols::studyProtocols( QWidget *parent)  : QWidget(parent){
    layout = new QGridLayout(this);
    layout->setSpacing(10);
    layout->setMargin(0);
    dsc = new studyDescUpdate;
    load();
}

void studyProtocols::updateProtocols(){
    dg = new QDialog();
    dg->setWindowFlags( Qt::Tool |
                        Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    dg->setModal(true);
    dg->setObjectName("upProtDialog");
    dg->setFixedSize(400,200);
    dsc->protocolsUpdate();
    connect(dsc,SIGNAL(finished()),this,SLOT(updated()));
    QPushButton * cl = new QPushButton(QIcon(":/icon/res/img/form/cancel.png"),tr("Anular"));
    cl->setObjectName("redButton");
    cl->setFixedSize(200,60);
    connect(cl,SIGNAL(clicked()),dg,SLOT(close()));
    connect(cl,SIGNAL(clicked()),dsc,SLOT(cancel()));

    QVBoxLayout *hl = new QVBoxLayout(dg);
    updateStatus = new QLabel(tr("Actualización..."));
    updateStatus->setAlignment(Qt::AlignCenter);
    hl->addWidget(updateStatus,10);
    hl->addWidget(cl,0,Qt::AlignCenter);
    dg->show();
}

void studyProtocols::updated(){

    if(dsc->err == ""){
        load();
        dg->close();
    }
    else{
        updateStatus->setText(dsc->err);
    }


}

void studyProtocols::load(){

    foreach(TouchButton *tb, touchButtons){
        layout->removeWidget(tb);
        delete tb;
    }
    items.clear();
    touchButtons.clear();
    studydesc sdc;
    items = sdc.get();
    int x = 0;
    foreach(int s, items){
        sd.loadData(s);
        TouchButton *tb = new TouchButton(sd.getValue("name").toString());
        connect(tb,SIGNAL(sendId(int)),this,SLOT(save(int)));
        tb->setId(s);
        tb->setText(sd.getValue("name").toString());
        touchButtons.append(tb);
        layout->addWidget(tb,x/3,x%3);
        x++;
    }


}

void studyProtocols::save(int s){
    value = s;
    foreach(TouchButton *t,touchButtons){
        if(value == t->getId()){
            emit selected();
            t->setChecked(true);
        }
        else t->setChecked(false);
    }
}

studyProtocols::~studyProtocols(){
    delete dsc;
}

int studyProtocols::getValue(){
    return value;
}

QString studyProtocols::text(){
    sd.loadData(value);
    return sd.getValue("name").toString();
}
