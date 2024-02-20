#include "captureprocesswidget.h"

captureProcessWidget::captureProcessWidget(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout *v = new QHBoxLayout(this);
    v->setMargin(0);
    v->setSpacing(0);
    info = new QPushButton("",this);
    info->setFixedSize(26,26);
    info->setIconSize(QSize(26,26));
    info->setIcon(QIcon(":/icon/res/img/footer/capture.png"));

    device = new QPushButton("",this);
    device->setFixedSize(26,26);
    device->setIconSize(QSize(26,26));
    device->setIcon(QIcon(":/icon/res/img/footer/plug.png"));

    connect(info,SIGNAL(clicked()),this,SLOT(killProcess()));
    connect(device,SIGNAL(clicked()),this,SLOT(checkDevice()));

    connect(&poller,SIGNAL(timeout()),this,SLOT(refresh()));
    connect(&pollerDevice,SIGNAL(timeout()),this,SLOT(refreshDevice()));
    v->addWidget(info,0,Qt::AlignVCenter | Qt::AlignCenter);
    v->addSpacing(20);
    v->addWidget(device,0,Qt::AlignVCenter | Qt::AlignCenter);
    poller.start(100);
    pollerDevice.start(2000);
    refresh();
    refreshDevice();
}

void captureProcessWidget::refresh(){
    if(captureProcess::_captureProcess->isRunning())
        info->setStyleSheet("background-color: #FF5454;  border-radius:3px; border:0px solid #666;");
    else
        info->setStyleSheet("background-color: transparent;  border-radius:3px; border:0px solid #666;");
}

void captureProcessWidget::refreshDevice(){
    if(!isDevice())
        device->setStyleSheet("background-color: #FF5454;  border-radius:3px; ");
    else
        device->setStyleSheet("background-color: #00D4D8;  border-radius:3px;");

}

bool captureProcessWidget::isDevice(){
    config c;
    c.load();
    bool existDevice = false;

    QList<QByteArray> listaCamaras = cam.availableDevices();
    for (int i=0; i<listaCamaras.size(); i++) {
        if(c.getValue("device").toString() == cam.deviceDescription(listaCamaras.at(i)))
            existDevice = true;
    }
    return existDevice;
}

void captureProcessWidget::killProcess(){
    if(captureProcess::_captureProcess->isRunning()){
        if(QMessageBox::question(this,tr("Detener el proceso"), tr("Desea detener el proceso de captura"),QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
            captureProcess::_captureProcess->killProcess();

    }
}

void captureProcessWidget::checkDevice(){
    config c;
    c.load();
    QString dev = c.getValue("device").toString();
    if(!isDevice())
        QMessageBox::warning(this,tr("Capturador no detectado"), tr("No se ha detectado el capturador: ")+dev);
    else
        QMessageBox::information(this,tr("Capturador detectado"), tr("Se ha detectado el capturador: ")+dev);
}

