#include "checkbandwith.h"

checkBandwith::checkBandwith(QWidget *parent)
    :QWidget(parent)
{
    loader = new QMovie(":/icon/res/img/loading.gif");
    loader->start();

    IsRunning = false;

    _startCheck = new QPushButton(QIcon(":/icon/res/img/footer/band.png"),tr(""));
    _startCheck->setFixedSize(50,50);
    _startCheck->setIconSize(QSize(50,50));
    connect(_startCheck,SIGNAL(clicked()),this,SLOT(start()));

    _valueLabel = new QLabel(tr("Desconocido"));
    _valueLabel->setFixedWidth(300);
    _valueLabel->setStyleSheet("background:#fafafa; padding:5px;");


    QHBoxLayout * layout = new QHBoxLayout(this);
    layout->addWidget(_startCheck);
    layout->addWidget(_valueLabel);

//---------------------------------------------------------------------------------------------------------
//  CR: 07/03/21
    connect(&mProcess,&QProcess::readyReadStandardOutput,this,&checkBandwith::ReadyReadStandardOutput);
    connect(&mProcess,&QProcess::readyReadStandardError,this,&checkBandwith::ReadyReadStandardError);
    connect(&mProcess,QOverload<int,QProcess::ExitStatus>::of(&QProcess::finished),this,&checkBandwith::Finished);
    //connect(&mProcess,&QProcess::finished(int,QProcess::ExitStatus),this,&checkBandwith::Finished(int,QProcess::ExitStatus));
//---------------------------------------------------------------------------------------------------------

    layout->setMargin(0);
    layout->setSpacing(1);
}

//--------------------------------------------------------------------------------

void checkBandwith::ReadyReadStandardOutput(void){
    mProcessMsg.append(mProcess.readAllStandardOutput());
}

void checkBandwith::ReadyReadStandardError(void){
    mProcessMsg.append(mProcess.readAllStandardError());
}

void checkBandwith::send(){
    QStringList params;
    params << "-n" << "1" << "8.8.8.8";
    mProcess.start("ping",params,QIODevice::ReadOnly);
}

void checkBandwith::Finished(int, QProcess::ExitStatus ){

    int8_t status = -1;

    if(mProcessMsg.contains("recibidos = 1")){
        status = 0;
        _valueLabel->setText("Conectado");
    }
    else{
        _valueLabel->setText("No conectado");
    }
    IsRunning = false;

    emit Wifi_status(status);
}

//--------------------------------------------------------------------------------

checkBandwith::~checkBandwith()
{
}

void checkBandwith::start(){

    if(IsRunning==true) return;

    conf.load();

    _valueLabel->setText(tr(""));
    _valueLabel->setMovie (loader);    

    IsRunning = true;
    mProcessMsg = "";
    send();

}


