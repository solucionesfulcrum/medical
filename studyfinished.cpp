//-------------------------------------------------------------------
//CR: 28/02/21
//-------------------------------------------------------------------
#include "studyfinished.h"

StudyFinished::StudyFinished(QObject *parent) : QObject(parent)
{
    pWebCtrl = new QNetworkAccessManager(this);
    pTimer = new QTimer(this);

    _cfg.load();
    Url.setUrl(_cfg.ip()+apiurlstudyConfirm);

    connect(pTimer, SIGNAL(timeout()), this, SLOT(SendStudystatus()));
    connect(pWebCtrl,SIGNAL(finished(QNetworkReply*)),this,SLOT(MessageFinished(QNetworkReply*)));
}


void StudyFinished::MessageFinished(QNetworkReply *p){

    QByteArray res = p->readAll();
    if(res == "OK"){

    //  Update db of uids sended
        QHash<QString, QString> data;
        data.insert("study_state",study_sended);
        //data.insert("state",state_send);
        foreach (QString s, _studies.getStudiesNotReported()){
            _studies.updateStudy(data,s);
        }
    }

    delete mtp;
    p->deleteLater();
    pTimer->start();

}


void StudyFinished::SendStudystatus(void){

    pTimer->stop();

    QString data;
    foreach (QString s, _studies.getStudiesNotReported())
        data += s + ",";
    data = data.left(data.length()-1);

    if(data==""){
        pTimer->start();
        return;
    }

    request = new QNetworkRequest;
    request->setUrl(Url);
    mtp = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    addPart("uids",data);
    addPart("api-key",apikey);
    addPart("namebox",_cfg.getValue("name").toString());
    addPart("password",_cfg.getValue("pass").toString());
    addPart("check",check);
    netReply = pWebCtrl->post(*request,mtp);
    return;
}

void StudyFinished::addPart(QString name, QString value, QString type){
    QHttpPart textPart;
    textPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(type+"; name=\""+name+"\""));
    textPart.setBody(value.toStdString().c_str());
    mtp->append(textPart);
}

void StudyFinished::start(void){
    pTimer->start(5000);
}
//-------------------------------------------------------------------


