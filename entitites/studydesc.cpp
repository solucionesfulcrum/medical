#include "studydesc.h"
#include "networkutils.h" //24-11-2023 Añadir MAC ADDRESS
#include <QDebug>

studydesc::studydesc() : entities()
{
    table = "protocols";
    id_name = "id";
    keys << "id" << "name" << "status" << "data";

}

studydesc::~studydesc()
{

}

QList<int> studydesc::get(){
    QList<int> result;
    QSqlQuery query("SELECT id FROM protocols",db->getDB());
    while (query.next())
        result.append(query.value(0).toInt());
    return result;
}

QStringList studydesc::getProtocols(){
    QStringList result;
    QSqlQuery query("SELECT name FROM protocols",db->getDB());
    while (query.next())
        result.append(query.value(0).toString());
    return result;
}

QStringList studydesc::getSweep(QString p){
    QStringList result;
    QSqlQuery query("SELECT sw.name "
                    "FROM sweeps as sw, protocols as sp "
                    "WHERE sw.id_protocols = sp.id AND sp.name = '"+p+"' ORDER BY name",db->getDB());
    while (query.next())
        result.append(db->stripslashs(query.value(0).toString()));
    return result;
}

QString studydesc::getSweepName(int id){
    QString q = "SELECT name FROM sweeps WHERE id = '"+QString::number(id)+"'";
    QSqlQuery query(q,db->getDB());
    qDebug() << q;
    if(query.next())
        return (db->stripslashs(query.value(0).toString()));
    else return "";
}

QList<int> studydesc::getSweepId(int p){
    QList<int> result;
    QSqlQuery query("SELECT sw.id , sw.name "
                    "FROM sweeps as sw "
                    "WHERE sw.id_protocols = '"+QString::number(p)+"' ",db->getDB());
    while (query.next())
        result.append(query.value(0).toInt());
    return result;
}

QString studydesc::getJson(){
    return db->stripslashs(getValue("data").toString());
}

bool studydesc::insertSweeps(QHash<QString,QString> data){
    return db->insert(data,"sweeps");
}


studyDescUpdate::studyDescUpdate() : QObject(){
    connect(&m_WebCtrl,SIGNAL(finished(QNetworkReply*)),this,SLOT(finished(QNetworkReply*)));
    err = "";
}

QString studyDescUpdate::API = apiurlprotocols;

void studyDescUpdate::protocolsUpdate(){
    _cfg.load();
    url.setUrl(_cfg.getValue("ip_server").toString()+studyDescUpdate::API);
    request.setUrl(url);

    QHttpMultiPart *mtp = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    addPart(mtp, "namebox", _cfg.getValue("name").toString());
    addPart(mtp, "password", _cfg.getValue("pass").toString());
    addPart(mtp, "api-key",apikey);
    addPart(mtp, "check",check);

    //24-11-2023 Añadir MAC ADDRESS
    QString mac_address = NetworkUtils::obtenerDireccionMAC();
    addPart(mtp,"mac_address",mac_address);
    //qDebug() << "Dirección MAC: " << mac_address;

    qDebug() << "Update:" << url.toString() << request.url().toString();
    rp = m_WebCtrl.post(request,mtp);
    connect(rp,SIGNAL(uploadProgress(qint64,qint64)),this,SLOT(dl(qint64,qint64)));
}

void studyDescUpdate::addPart(QHttpMultiPart * mtp, QString key , QString v){
    QHttpPart textPart;
    textPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\""+key+"\""));
    textPart.setBody(v.toStdString().c_str());
    mtp->append(textPart);
}

void studyDescUpdate::cancel(){
    rp->abort();
}


void studyDescUpdate::finished(QNetworkReply* pReply){
    QByteArray res = pReply->readAll();
    err = "";
    //qDebug() << res << pReply->error();
    if(pReply->error() == QNetworkReply::NoError){
        studydesc sd;
        QJsonDocument itemDoc = QJsonDocument::fromJson(res);
        QJsonObject rootObject = itemDoc.object();
        QJsonArray  parray =  rootObject.value("protocols").toArray();

        if(parray.size() > 0) {
            qDebug() << sd.execute("DELETE FROM protocols");
            qDebug() << sd.execute("DELETE FROM sweeps");
            for(int x = 0; x< parray.size(); x++ ){

                QJsonObject protocols = parray.at(x).toObject();
                qDebug() << protocols;
                QHash<QString, QString> data;
                data.insert("name",protocols.value("name").toString());
                QJsonArray datajson = protocols.value("history_template").toArray();

                QJsonDocument d(datajson);

                data.insert("data",QString(d.toJson()));
                data.insert("id",_cfg.JsonToString(protocols.value("id")));
                data.insert("status",_cfg.JsonToString(protocols.value("status")));
                qDebug() << data;
                sd.insert(data);
                QJsonArray sweeps = protocols.value("sweeps").toArray();
                for(int y = 0; y< sweeps.size(); y++ ){
                    QHash<QString, QString> dataSweeps;
                    QString name = sweeps.at(y).toObject().value("name").toString();
                    QString id= _cfg.JsonToString(sweeps.at(y).toObject().value("id"));
                    QString protocolid= _cfg.JsonToString(sweeps.at(y).toObject().value("protocol_id"));

                //---------------------------------------------------------------------------------
                //  CR: 01/02/24

                    if(!QDir("sweeps/"+protocolid+"/").exists())
                        QDir().mkdir("sweeps/"+protocolid+"/");


                    QString base64String = sweeps.at(y).toObject().value("image64").toString();

                    if(!base64String.isEmpty())
                    {
                        QByteArray imgBytes = QByteArray::fromBase64(base64String.toUtf8());
                        QImage img;

                        img.loadFromData(imgBytes);

                        if(img.save("sweeps/"+protocolid+"/"+ QString::number(y+1) + ".bmp", "BMP"))
                            qDebug()<<"Image saved";
                        else {
                            qDebug()<<"Image error save";
                        }
                    }
                //---------------------------------------------------------------------------------
                    dataSweeps.insert("name",name);
                    dataSweeps.insert("id",id);
                    dataSweeps.insert("id_protocols",protocolid);

                    sd.insertSweeps(dataSweeps);
                }
            }
        }
    }
    else err = pReply->errorString();
    emit finished();
}


void studyDescUpdate::dl(qint64 a ,qint64 n){
    //qDebug() << "Upload: " << a << n;
}


