#include "checkbandwith.h"

checkBandwith::checkBandwith(QWidget *parent)
    :QWidget(parent)
{
    loader = new QMovie(":/icon/res/img/loading.gif");
    loader->start();
    isfinish = false;
    QObject::connect(&m_WebCtrl,SIGNAL(finished(QNetworkReply*)),this,SLOT(finished(QNetworkReply*)));
    setStep();

    _startCheck = new QPushButton(QIcon(":/icon/res/img/footer/band.png"),tr(""));
    _startCheck->setFixedSize(50,50);
    _startCheck->setIconSize(QSize(50,50));

    connect(_startCheck,SIGNAL(clicked()),this,SLOT(start()));
    _valueLabel = new QLabel(tr("Desconocido"));
    _valueLabel->setFixedWidth(100);
    _valueLabel->setStyleSheet("background:#fafafa; padding:5px;");

    showResult();

    QHBoxLayout * layout = new QHBoxLayout(this);
    layout->addWidget(_startCheck);
    layout->addWidget(_valueLabel);

    layout->setMargin(0);
    layout->setSpacing(1);
}

checkBandwith::~checkBandwith()
{
}


void checkBandwith::setURL(QString u){
    url.setUrl(u);
    request.setUrl(url);
}

void checkBandwith::setStep(){
    step.append(10000);
    step.append(20000);
    step.append(40000);
    step.append(80000);
    step.append(160000);
    step.append(320000);
    step.append(640000);
    step.append(128000);
    step.append(256000);
    step.append(512000);
    step.append(1024000);
    step.append(2048000);
    step.append(4096000);
}

void checkBandwith::send(int s){
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    QByteArray ba;
    size = s;
    for(int x=0; x<size; x++)
        ba.append("a");
    QHttpPart textPart;
    textPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"api-key\""));
    textPart.setBody(apikey.toStdString().c_str());
    multiPart->append(textPart);

    //timer.start();
    //qDebug()<<m_WebCtrl.post(request,multiPart)->error();
}

void checkBandwith::finished(QNetworkReply* pReply){
    qint64 tnano = timer.nsecsElapsed();
    elapsedTime = tnano;
    //qDebug() << pReply->error() << pReply->errorString() << pReply->readAll();
    if(pReply->error() == QNetworkReply::NoError){
        findBestPack();
    }
    else {
        if (pruebas < 3){
            pruebas++;
            send(bestPack);
        }
        else{
            _valueLabel->setText(tr("Sin conexión"));
            isfinish = true;
        }
    }
}

void checkBandwith::findBestPack(){
    if(elapsedTime < sec && nextvalue < (step.size()-1)){
        nextvalue++;
        bestPack = step.at(nextvalue);
        QString res = QString::number(bestPack)+tr("b/sec");
        if (bestPack > 1000)
            res =  QString::number(bestPack/1000)+tr("Kb/sec");
        if (bestPack > 1000000)
            res =  QString::number(bestPack/1000000)+tr("Mb/sec");
        if (bestPack > 1000000000)
            res =  QString::number(bestPack/1000000000)+tr("Gb/sec");
        pruebas = 0;
        send(bestPack);
    }
    else{
        if (pruebas < 3){
            pruebas++;
            send(bestPack);
            return;
        }
        bestPack = (bestPack*sec)/elapsedTime;


        isfinish = true;
        /*conf.load();
        int maxtime = conf.maxTimeSend();
        qint64 maxsize = bestPack*maxtime;
        if (maxsize > conf.maxFileSize() )
            maxsize = conf.maxFileSize();
        if (maxsize < conf.minFileSize())
            maxsize   = conf.minFileSize();*/

        //qDebug() << "Kb/sec elegido" << bestPack/1000;
        QHash<QString,QString> data;
        data.insert("bandwith",QString::number(bestPack));
        conf.update(data);
        showResult();
        emit changed();
    }
}

qint64  checkBandwith::getVideoBitrate(){
    config conf;
    conf.load();
    qint64 videoSizeAvg = conf.maxTimeSend();
    qint64 videobitrate = (conf.fileSize()/videoSizeAvg)*checkBandwith::Bit()/1000;
    return videobitrate;
}

void checkBandwith::showResult(){
    conf.load();
    qint64 maxsize = conf.getValue("bandwith").toInt();
    QString t = "";
    QString res = QString::number(maxsize)+tr("b")+t;
    if (maxsize >= 1000)
        res =  QString::number(maxsize/1000)+tr("Kb")+t;
    if (maxsize >= 1000000)
        res =  QString::number(maxsize/1000000)+tr("Mb")+t;
    if (maxsize >= 1000000000)
        res =  QString::number(maxsize/1000000000)+tr("Gb")+t;
    _valueLabel->setText(res);
}

qint64 checkBandwith::getBestPack(){
    return bestPack;
}

void checkBandwith::start(){

    //qDebug() << "Start CBW";
    conf.load();
    pruebas = 0;
    _valueLabel->setText(tr(""));
    _valueLabel->setMovie (loader);    
    setURL(conf.ip()+"/api/check");
    nextvalue = 0;
    bestPack = step.at(nextvalue);
    send(bestPack);

}

int checkBandwith::getBit(){
    return bit;
}

int checkBandwith::Bit(){
    return bit;
}

