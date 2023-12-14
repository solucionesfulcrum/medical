#include "checkbandwith.h"
#include "networkutils.h" //24-11-2023 Añadir MAC ADDRESS
#include <QDebug>

checkBandwith::checkBandwith(QWidget *parent)
    :QWidget(parent)
{
    loader = new QMovie(":/icon/res/img/loading.gif");
    loader->start();

    IsRunning = false;
    _cfg.load();

    _startCheck = new QPushButton(QIcon(":/icon/res/img/footer/band.png"),tr(""));
    _startCheck->setFixedSize(35,35);
    _startCheck->setIconSize(QSize(26,26));
    connect(_startCheck,SIGNAL(clicked()),this,SLOT(start()));

    _valueLabel = new QLabel(tr("Desconocido"));
    _valueLabel->setFixedWidth(300);
    _valueLabel->setStyleSheet("background:#fafafa; padding:5px;");

    m_WebCtrl = new QNetworkAccessManager;
    connect(m_WebCtrl,SIGNAL(finished(QNetworkReply*)),this,SLOT(FinishedMessage(QNetworkReply*)));
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(send()));
    //send();

    QHBoxLayout * layout = new QHBoxLayout(this);
    layout->addWidget(_startCheck);
    layout->addWidget(_valueLabel);

    layout->setMargin(0);
    layout->setSpacing(1);



}

checkBandwith::~checkBandwith()
{
}

void checkBandwith::send()
{
    if(IsRunning==false)
    {
        IsRunning = true;
        request.setUrl(url);
        url.setUrl(_cfg.ip()+apiurlcheckConn);
        request.setUrl(url);
        mtp = new QHttpMultiPart(QHttpMultiPart::FormDataType);

        addPart(mtp,"api-key",apikey);
        addPart(mtp,"namebox",_cfg.getValue("name").toString());
        addPart(mtp,"password",_cfg.getValue("pass").toString());
        addPart(mtp,"check",check);

        //24-11-2023 Añadir MAC ADDRESS
        QString mac_address = NetworkUtils::obtenerDireccionMAC();
        addPart(mtp,"mac_address",mac_address);
        //qDebug() << "Dirección MAC: " << mac_address;

        rp = m_WebCtrl->post(request,mtp);
    }
}

void checkBandwith::FinishedMessage(QNetworkReply* pReply)
{
    int8_t status = -1;

    QJsonDocument jsonDocument = QJsonDocument::fromJson(pReply->readAll());
    QJsonObject jsonObject = jsonDocument.object();
    QJsonValue response = jsonObject.value("message");
    if(response=="200")
    {
        status = 0;
        _valueLabel->setText("Conectado");
        _startCheck->setStyleSheet("background-color: #FFFFFF;  border-radius:3px; ");
    }
    else
    {
        _valueLabel->setText("No conectado");
        _startCheck->setStyleSheet("background-color: #FF5454;  border-radius:3px; ");
    }

    IsRunning = false;
    emit Wifi_status(status);
}


void checkBandwith::addPart( QHttpMultiPart *m,QString name, QString value, QString type)
{
    QHttpPart textPart;
    textPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(type+"; name=\""+name+"\""));
    textPart.setBody(value.toStdString().c_str());
    m->append(textPart);
}

void checkBandwith::start(){

    if(IsRunning==true) return;

    if(!m_timer.isActive())
        m_timer.start(30000);

    //send();

    _valueLabel->setText(tr(""));
    _valueLabel->setMovie(loader);


}

/*
void checkBandwith::Finished(int, QProcess::ExitStatus ){

    int8_t status = -1;

    if(mProcessMsg.contains("recibidos = 1")){
        status = 0;
        _valueLabel->setText("Conectado");
        _startCheck->setStyleSheet("background-color: #FFFFFF;  border-radius:3px; ");
    }
    else{
        _valueLabel->setText("No conectado");
        _startCheck->setStyleSheet("background-color: #FF5454;  border-radius:3px; ");
    }
    IsRunning = false;



    emit Wifi_status(status);
}*/

//--------------------------------------------------------------------------------
