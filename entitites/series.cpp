#include "series.h"

series::series() : entities()
{
    table = "series";
    id_name = "id";
    keys << "id"
         << "uid"
         << "starttime"
         << "finishtime"
         << "toqueue"
         << "capture"
         << "sent"
         << "confirmed"
         << "image_number"
         << "id_sweeps"
         << "id_studies"
         << "intent"
            ;
}

series::~series()
{

}

QList<int> series::listeIDFromStudy(int id){
    QList<int> result;
    QSqlQuery query("SELECT id FROM series WHERE id_studies = '"+QString::number(id)+"' ORDER BY id");
    while (query.next())
        result.append(query.value(0).toInt());
    return result;
}

QList<int> series::listeIDtoQueue(){
    QList<int> result;
    QSqlQuery query("SELECT id FROM series WHERE toqueue = 1 AND sent = 0 ORDER BY starttime");
    while (query.next())
        result.append(query.value(0).toInt());
    return result;
}


bool series::updateSerie(QHash<QString, QString> data, QString uid){
    return  db->update(data,table,"uid",uid);
}

int series::id_serie(){
    return getValue("id").toInt();
}
int series::id_study(){
    return getValue("id_studies").toInt();
}
int series::sent(){
    return getValue("sent").toInt();
}
int series::checked(){
    return getValue("confirmed").toInt();
}
int series::toqueue(){
    return getValue("toqueue").toInt();
}
int series::capture(){
    return getValue("capture").toInt();
}

QString series::uid(){
    return getValue("uid").toString();
}
QString series::datetime(){
    return getValue("starttime").toString();
}
int series::serieName(){
    return getValue("id_sweeps").toInt();
}
QString series::serieNote(){
    return "";
}

QString series::serieNameValue(){
    QString q = "SELECT name FROM sweeps WHERE id = "+getValue("id_sweeps").toString()+" LIMIT 0,1 ";    

    QSqlQuery query(q);
    while (query.next()){        
        return serieEliminateAccent(query.value(0).toString());
    }
    return "";

}

//----------------------------------------------------------
// CR: 17/01/23
QString series::serieEliminateAccent(QString s)
{
    s.replace("á","a");
    s.replace("é","e");
    s.replace("í","i");
    s.replace("ó","o");
    s.replace("ú","u");

    s.replace("Á","A");
    s.replace("É","E");
    s.replace("Í","I");
    s.replace("Ó","O");
    s.replace("Ú","U");

    s.replace("Ñ","N");
    s.replace("ñ","n");

    return s;
}
//----------------------------------------------------------

int series::getSerieNumber(){
    QString q = "SELECT id FROM series WHERE id_studies = "+getValue("id_studies").toString()+" ORDER BY id";
    qDebug() << q;
    QSqlQuery query(q);
    int i = 1;
    while (query.next()){
        if (query.value(0).toInt() == getValue("id").toInt()){
            return i;
        }
        i++;
    }
    return 0;
}

bool series::haveVideo(){
    QString f1 = "studies/"+getValue("id_studies").toString()+"/"+getValue("id").toString()+"/"+compressedvideoname;
    QString f2 = "studies/"+getValue("id_studies").toString()+"/"+getValue("id").toString()+"/"+uncompressedvideoname;


    if(QFile::exists(f1) || QFile::exists(f2) )
        return true;
    else return false;
}

QString series::getVideoFile(){
    QString f1 = "studies/"+getValue("id_studies").toString()+"/"+getValue("id").toString()+"/"+compressedvideoname;
    QString f2 = "studies/"+getValue("id_studies").toString()+"/"+getValue("id").toString()+"/"+uncompressedvideoname;
    if(QFile::exists(f1))
        return f1;
    if(QFile::exists(f2))
        return f2;
    return "";
}

