#include "studycheck.h"
#include "networkutils.h" //24-11-2023 Añadir MAC ADDRESS
#include <QDebug>

QString studyCheck::API =   apiurlcheckstudy;


void saveJson::setJson(QByteArray r){
    res = r;
}

void saveJson::run(){
    config _cfg;
    QJsonDocument itemDoc = QJsonDocument::fromJson(res);
    QJsonObject rootObject = itemDoc.object();
    QJsonObject json_studies = rootObject.value("studies").toObject();

    foreach(QString s, json_studies.keys()){
        QJsonObject study = json_studies.value(s).toObject();
        QHash<QString, QString> data;
        QHash<QString, QString> patient_data;

        //29-11-2023 Se recupera el ID del paciente para actualizar cuando hubo modificacion en la plataforma web
        QString patient_id = _studies.getPatientIdFromUID(s);
        QString modify_patient = _cfg.JsonToString(study.value("modify_patient"));
        if ( modify_patient == "1" ){
            patient_data.insert("idp",study.value("patient_id").toString());
            patient_data.insert("name",study.value("patient_first_name").toString());
            patient_data.insert("last_name",study.value("patient_second_name").toString());
            patient_data.insert("size",study.value("patient_size").toString());
            patient_data.insert("sex",study.value("patient_sex").toString());

            QDate fecha = QDate::fromString(study.value("patient_bday").toString(""), "yyyy-MM-dd");

            if (fecha.isValid()) {
                patient_data.insert("birthday",fecha.toString("yyyyMMdd"));
            }

            patient_data.insert("height",study.value("patient_height").toString());
            patient_data.insert("weight",study.value("patient_weight").toString());
            patient_data.insert("phone",study.value("patient_phone").toString());
            patient_data.insert("cellphone",study.value("patient_cellphone").toString());
            patient_data.insert("email",study.value("patient_email").toString());
            _patient.updatePatient(patient_data,patient_id);
        }

        QString status = _cfg.JsonToString(study.value("status"));
    //-------------------------------------------------------------------------
    //  CR:11/06/23
        /*if(status == "0")
            data.insert("state",state_incomplete);
        else
            data.insert("state",status);*/

        if((status>="1" && status <"5")|| (status=="8") || (status=="7") )
            data.insert("state",status);
    //-------------------------------------------------------------------------
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

    sendFinished = true;
    err = false;

    m_WebCtrl = new QNetworkAccessManager;
    sleepTime = 30000;
    connect(m_WebCtrl,SIGNAL(finished(QNetworkReply*)),this,SLOT(finished(QNetworkReply*)));

    connect(&m_timer,SIGNAL(timeout()),this,SLOT(timeout()));

    //connect(&m_timer, SIGNAL(timeout()), this, SLOT(send()));
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
    return sendFinished;
}

void studyCheck::send(){
    if (sendFinished){
        sendFinished = false;
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

        //24-11-2023 Añadir MAC ADDRESS
        QString mac_address = NetworkUtils::obtenerDireccionMAC();
        addPart("mac_address",mac_address);
        //qDebug() << "Dirección MAC: " << mac_address;

        rp = m_WebCtrl->post(request,mtp);
    }
}

void studyCheck::addPart(QString name, QString value, QString type){    
    QHttpPart textPart;
    textPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(type+"; name=\""+name+"\""));
    textPart.setBody(value.toStdString().c_str());
    mtp->append(textPart);
}

void studyCheck::finished(QNetworkReply* pReply){

    QByteArray res = pReply->readAll();
    if(pReply->error() == QNetworkReply::NoError)
    {
        err = false;
        saver->setJson(res);
        saver->start();
    }
    else
    {
        err = true;
        delete mtp;
    }
    sendFinished = true;
    pReply->deleteLater();
}

void studyCheck::refreshStudies(void)
{
    send();

    m_timeout = false;
    m_timer.setInterval(500);
    m_timer.start();

    do{
        QCoreApplication::processEvents();
    }while(m_timeout==false);

    do{
        QCoreApplication::processEvents();
    }while(isFinished()==false);


/*    do {
        m_timer.singleShot(1000,this);
        QCoreApplication::processEvents();
    } while (isFinished()==false);
*/

}

void studyCheck::timeout()
{
    m_timeout = true;
}
//    finish();
//    pReply->deleteLater();

/*
    if(pReply->error() == QNetworkReply::NoError){
        saver->setJson(res);
        saver->start();
    }
    else finish();
    pReply->deleteLater();
*/


/*
void studyCheck::finish(){
    sendFinished = true;
    delete mtp;
}
*/

