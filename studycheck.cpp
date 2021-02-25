#include "studycheck.h"

QString studyCheck::API = apiurlcheckstudy;


void saveJson::setJson(QByteArray r){
    res = r;
}

void saveJson::run(){
    config _cfg;
    QJsonDocument itemDoc = QJsonDocument::fromJson(res);
    QJsonObject rootObject = itemDoc.object();
    QJsonObject json_studies = rootObject.value("studies").toObject();
    // qDebug() << json_studies;
    foreach(QString s, json_studies.keys()){
        QJsonObject study = json_studies.value(s).toObject();
        QHash<QString, QString> data;


        QString status = _cfg.JsonToString(study.value("status"));
        if(status == "0")
            data.insert("state","-1");
        else
            data.insert("state",status);
        data.insert("report_link",study.value("url").toString());
        if(status == "3")
            data.insert("new_report","1");
        _studies.updateStudy(data,s);

        QJsonArray series = study.value("series").toArray();
        for(int y=0; y<series.size() ; y++){
            QJsonObject serie = series.at(y).toObject();
            QHash<QString, QString> data;
            data.insert("confirmed","1");
            data.insert("image_number" , _cfg.JsonToString(study.value("number_images")));
            _series.updateSerie(data,serie.value("uid").toString());            
        }

        if(status == "3"){
        //  DELETE VIDEO FILES
            studies::deleteFolder(s);
        }

    }
}

studyCheck::studyCheck(){
    saver = new saveJson;
    connect(saver, SIGNAL(finished()), this, SLOT(finish()));
    ischecking = false;
    m_WebCtrl = new QNetworkAccessManager;
    sleepTime = 30000;
    connect(m_WebCtrl,SIGNAL(finished(QNetworkReply*)),this,SLOT(finished(QNetworkReply*)));
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(send()));
    send();
}

studyCheck::~studyCheck()
{
    saver->quit();
    saver->deleteLater();
}



void studyCheck::stop(){
    m_timer.stop();
}

void studyCheck::start(){
    if(!m_timer.isActive()){
        //qDebug() << "Start timer checking" << sleepTime << " ms";
        m_timer.start(sleepTime);
    }
}

void studyCheck::setSleepTime(int t){
    sleepTime = t;
    m_timer.setInterval(t);
}

bool studyCheck::isFinished(){
    return ischecking;
}

void studyCheck::send(){
    if (!ischecking){
        ischecking = true;
        _cfg.load();
        url.setUrl(_cfg.ip()+studyCheck::API);
        request.setUrl(url);
        mtp = new QHttpMultiPart(QHttpMultiPart::FormDataType);
        QString tosend;
        foreach (QString s, _studies.getAllNotReported())
            tosend += s+",";
        tosend = tosend.left(tosend.length()-1);
        addPart("uids",tosend);
        addPart("api-key",apikey);
        addPart("namebox",_cfg.getValue("name").toString());
        addPart("password",_cfg.getValue("pass").toString());
        addPart("check",check);
        rp = m_WebCtrl->post(request,mtp);
    }
}

void studyCheck::addPart(QString name, QString value, QString type){
    //qDebug() << name << "=>" <<value;
    QHttpPart textPart;
    textPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(type+"; name=\""+name+"\""));
    textPart.setBody(value.toStdString().c_str());
    mtp->append(textPart);
}

void studyCheck::finished(QNetworkReply* pReply){
    QByteArray res = pReply->readAll();
    if(pReply->error() == QNetworkReply::NoError){
        saver->setJson(res);
        saver->start();
    }
    else finish();
    pReply->deleteLater();
}

void studyCheck::finish(){
    ischecking = false;
    delete mtp;
}

