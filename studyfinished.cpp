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

/*
QTimer timer;
timer.setSingleShot(true);

QEventLoop loop;
connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
timer.start(30000);   // 30 secs. timeout
loop.exec();

if(timer.isActive()) {
    timer.stop();
    if(m_reply->error() > 0) {
      ... // handle error
    }
    else {
      int v = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

      if (v >= 200 && v < 300) {  // Success
        ...
      }
    }
} else {
   // timeout
   disconnect(reply, SIGNAL(finished()), &loop, SLOT(quit()));

   reply->abort();
}*/
