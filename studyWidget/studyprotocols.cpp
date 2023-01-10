#include "studyprotocols.h"


studyProtocols::studyProtocols( QWidget *parent)  : QWidget(parent){
    layout = new QGridLayout(this);
    layout->setSpacing(10);
    layout->setMargin(0);
    dsc = new studyDescUpdate;
    load();
}

void studyProtocols::updateProtocols(){
    if (operators::isAdmin()) {
        dg = new QDialog();
        dg->setWindowFlags( Qt::Tool |Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
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
    } else {
        QMessageBox::information(this,tr("Protocolos"),tr("No cuentas con los permisos suficientes para actualizar protocolos."),QMessageBox::Ok);
    }
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

//----------------------------------------------------------------
//  Christiam
void studyProtocols::loadWithSex(char sex, int age)
{
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
        QString protocolType = sd.getValue("name").toString();

        QList<QString> listpt; //Se modifico para que acepte cualquier palabra con obstetrico
        listpt.append("obstetrico");
        listpt.append("obstétrico");
        listpt.append("protocolo obstetrico");
        listpt.append("protocolo obstétrico");
        listpt.append("obstetric");
        listpt.append("obstetric protocol");
        QSet<QString> setPt = listpt.toSet();
//----------------------------------------------------------------------------------------------------
//      CR: 04/01/23
        if((setPt.contains(protocolType.toLower())==true) && ( (sex == 'M') || (age<9)))    continue;
        if((protocolType.toLower()=="pulmonar") && (age<1)) continue;
//----------------------------------------------------------------------------------------------------

        TouchButton *tb = new TouchButton(protocolType);
        connect(tb,SIGNAL(sendId(int)),this,SLOT(save(int)));
        tb->setId(s);
        tb->setText(sd.getValue("name").toString());
        touchButtons.append(tb);
        layout->addWidget(tb,x/3,x%3);
        x++;
    }
    return;
}
//----------------------------------------------------------------

//  Load clinical studies: Obstetrico, CSD, Tiroides.
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
        qDebug()<< sd.getValue("name").toString();
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

