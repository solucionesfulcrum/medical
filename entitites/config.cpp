#include "config.h"

config::config() : entities()
{
    table = "configuration";
    id_name = "id";
    keys << "id"
         << "ip_server"
         << "VIDEOINPUT"
         << "name"
         << "pass"
         << "DEFAULTVIDEOBITRATE"
         << "bandwith"
         << "fps"
         << "SIZE"
         << "PIXELCONF"
         << "VBITRATEMIN"
         << "VBITRATEMAX"
         << "modelUS"
         << "keep_uncompressed"
         << "device"
         << "consent"
         << "timeout"
            ;
    load();
}

config::~config()
{
}

QStringList config::serverList(){
    QStringList serversList;
    QFile file("server.json");
    if (!file.open(QIODevice::ReadOnly)){
        return serversList;
     }

     QByteArray json = file.readAll();
     QJsonDocument itemDoc = QJsonDocument::fromJson(json);
     foreach(QString s, itemDoc.object().keys())
         serversList.append(itemDoc.object().value(s).toString());
     return serversList;
}

bool config::update(QHash<QString, QString> data){
    bool b =  entities::update(data,1);
    load();
    return b;
}

bool config::load(){
    QString q = "SELECT * FROM configuration WHERE id = 1 LIMIT 0,1";
    QSqlQuery query(q);
    while (query.next()){
        foreach(QString s , keys){
            data[s] = query.value(s);
        }
    }
}

void config::loadData(int id){
    load();
}


QString config::ip(){
    return getValue("ip_server").toString();
}

int config::videoInput(){
    return getValue("VIDEOINPUT").toInt();
}

int config::maxTimeSend(){
    return 1;
}

int config::maxFileSize(){
    return 1;
}

int config::minFileSize(){
    return 1;
}
int config::fileSize(){
    return 1;
}

QString config::AETitle(){
    return "";
}
