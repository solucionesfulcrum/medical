#include "studies.h"


studies::studies() : entities(){
    table = "studies";
    id_name = "id";
    keys << "id"
         << "uid"
         << "starttime"
         << "finishtime"
         << "reason"
         << "state"
         << "report_link"
         << "id_protocols"
         << "id_patients"
         << "id_operators"
         << "data"
         << "urgent"
         << "new_report";
}

studies::~studies(){
}

QList<int> studies::listeID(QString s){
    QList<int> result;
    QString q;
    if(s == "")
        q = "SELECT id FROM studies ORDER BY starttime desc";
    else q = s;
    //qDebug() << q;
    QSqlQuery query(q);
    while (query.next())
        result.append(query.value(0).toInt());
    return result;
}

QStringList studies::getAllNotReported(){
    QStringList result;
    QString q;
    q += "SELECT uid ";
    q += "FROM studies as s ";
    q += "WHERE state < 3 ";
    QSqlQuery query(q);
    while (query.next())
        result.append(query.value(0).toString());
    return result;
}

QStringList studies::getToConfirm(){
    QStringList result;
    QString q;
    q += "SELECT uid ";
    q += "FROM studies as s ";
    q += "WHERE(SELECT COUNT(ser.id) as total FROM series as ser WHERE ser.id_studies = s.id AND sent = 1) = (SELECT COUNT(sw.id) as tt FROM sweeps as sw WHERE sw.id_protocols = s.id_protocols) AND state < 3 ";
    //qDebug() << q;
    QSqlQuery query(q);
    while (query.next())
        result.append(query.value(0).toString());
    return result;
}

QStringList studies::getNotFinished(){
    QStringList result;
    QString q;
    q += "SELECT id ";
    q += "FROM studies as s ";
    q += "WHERE(SELECT COUNT(ser.id) as total FROM series as ser WHERE ser.id_studies = s.id AND sent = 1) < (SELECT COUNT(sw.id) as tt FROM sweeps as sw WHERE sw.id_protocols = s.id_protocols) ";
    //qDebug() << q;
    QSqlQuery query(q);
    while (query.next())
        result.append(query.value(0).toString());
    return result;
}

QStringList studies::getPending(){
    QStringList result;
    QString q;
    q += "SELECT id ";
    q += "FROM studies as s ";
    q += "WHERE state = -1";
    //qDebug() << q;
    QSqlQuery query(q);
    while (query.next())
        result.append(query.value(0).toString());
    return result;
}

bool studies::updateStudy(QHash<QString, QString> data, QString uid){
    return  db->update(data,table,"uid",uid);
}

QString studies::uid(){
    return getValue("uid").toString();
}
QString studies::datetime(){
    return getValue("starttime").toString();
}

QString studies::reason(){
    return getValue("reason").toString();
}

QString studies::desc(){
    //protocols
    return "";
}

int studies::id_patient(){
    return getValue("id_patients").toInt();
}
int studies::id_doctor(){
    return getValue("id_operators").toInt();
}

QString studies::getState(){
    return studies::getStateName(getValue("state").toInt());
}


QString studies::getData(){
    return db->stripslashs(getValue("data").toString());
}

QString studies::getStateName(int v){
    switch(v){
    case -1: return "Pendiente"; break;
    case 0: return "Terminado"; break;
    case 1: return "Confirmado"; break;
    case 2: return "Asignado"; break;
    case 3: return "Diagnosticado"; break;
    }
}

int studies::getIdFromUID(QString uid){
    QString q;
    q += "SELECT id ";
    q += "FROM studies as s ";
    q += "WHERE uid = '"+uid+"' ";
    QSqlQuery query(q);
    qDebug() << q << query.size();
    if(query.next())
        return query.value(0).toInt();
    else return -1;
}

bool studies::deleteStudy(int id){
    studies _studies;
    QString studyId = QString::number(id);
    QDir d("studies/" + studyId);
    bool removed = d.removeRecursively();
    qDebug() << "The folder o studies" << studyId << "is deleted? " << removed;
    if (removed){
        _studies.execute("DELETE FROM studies WHERE id = "+ studyId);
        _studies.execute("DELETE FROM series WHERE id_studies = "+ studyId);
    }
    return removed;
}

bool studies::deleteFolder(QString uid){
    qDebug() << "Delete folder "+ uid;
    studies _studies;
    QString studyId = QString::number(_studies.getIdFromUID(uid));
    if(studyId != "-1"){
        QDir d("studies/" + studyId);
        qDebug() << "Delete studies/" + studyId + " Folder";
        return  d.removeRecursively();
    }
    else
       return false;
}









