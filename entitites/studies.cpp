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
         << "trainning"
         << "ConsentimientoInformado"
         << "new_report"
         << "study_state"
         << "campaign";
}

studies::~studies(){
}

int studies::getLastElement(QString element){
    QString query = "SELECT "+ element +" FROM studies ORDER BY ID DESC LIMIT 1";
    QSqlQuery q;
    int id=-1;

    if(q.exec(query)==false) return -1;

    while(q.next()){
        id = q.value(0).toInt();
    }
    return id;

}

QList<int> studies::listeID(QString s){
    QList<int> result;
    QString q;
    if(s == "")
        q = "SELECT id FROM studies ORDER BY starttime desc";
    else q = s;

    qDebug() << q;

    QSqlQuery query(q);
    while (query.next())
        result.append(query.value(0).toInt());
    return result;
}

//------------------------------------------------------------------
//  CR: 09/07/23
 QList<int> studies::GetIncommpleteAndNotsendStudies(void)
{
    QList<int> result;
    QList<int> r;
    QString q;
    QDateTime DateToday = QDateTime::currentDateTime();
    QDateTime DatePast = QDateTime::currentDateTime();
    DateToday.setTime(QTime(23,59,59));
    DatePast.setTime(QTime(0,0,0));
    DatePast = DatePast.addDays(-30);

    uint timestampTo = DateToday.toTime_t();
    uint timestampFrom = DatePast.toTime_t();

    q += "SELECT id ";
    q += "FROM studies as s ";
    q += "WHERE s.starttime > "+QString::number(timestampFrom)+" ";
    q += "AND s.starttime < "+QString::number(timestampTo)+" ";
    q += "AND s.state = -1";
    QSqlQuery query(q);
    while (query.next())
        r.append(query.value(0).toInt());
    result.append(r.length());

    r.clear();
    q.clear();
    q += "SELECT id ";
    q += "FROM studies as s ";
    q += "WHERE s.starttime > "+QString::number(timestampFrom)+" ";
    q += "AND s.starttime < "+QString::number(timestampTo)+" ";
    q += "AND s.state = 0";
    query.clear();
    query.exec(q);
    while (query.next())
        r.append(query.value(0).toInt());
    result.append(r.length());

    return result;
}

//------------------------------------------------------------------

QStringList studies::getAllNotReported(){
    QStringList result;
    QString q;
    q += "SELECT uid FROM studies as s WHERE (state > 0 AND state < 5) OR state ==8";
    QSqlQuery query(q);
    while (query.next())
        result.append(query.value(0).toString());
    return result;
}

//-----------------------------------------------------
//  CR: 28/02/21
QStringList studies::getStudiesNotReported(void){
    QStringList result;
    QString q;
    q += "SELECT uid ";
    q += "FROM studies as s ";
    q += "WHERE study_state == 1 ";
    QSqlQuery query(q);
    while (query.next())
        result.append(query.value(0).toString());
    return result;
}

bool studies::UpdateLastElement(QHash<QString,QString> data){
    return  db->UpdateLastElement(data,table);
}

//-----------------------------------------------------

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
/*
//-------------------------------------------------
//  CR: 15/05/23
    case -1:    return "Incompleto"; break;
    case 0:     return "Sin enviar"; break;
    case 1:     return "Enviado"; break;
//-------------------------------------------------
    case 2:     return "Asignado"; break;
    case 3:     return "Diagnosticado"; break;
    case 4:     return "Enviado"; break;
    case 5:     return "Firmado"; break;
    case 6:     return "Rechazado"; break;
    case 7:     return "Desactivado"; break;
    default: break;
    }
*/

//  CR: 04/06/23
//  CR: 26/07/23
    case -1:    return "Incompleto"; break;
    case 0:     return "En transmisión"; break;
    case 1:     return "Enviado"; break;
    case 2:     return "Asignado"; break;
    case 3:     return "Diagnosticado"; break;
    case 4:     return "Descargado"; break;
    case 7:     return "Desactivado"; break;
    case 8:     return "Adenda";
//-------------------------------------------------
    default:    return "";          break;
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


QString studies::operatorName(){
    int id = getValue("id_operators").toInt();
    QString q = "SELECT name FROM operators WHERE id = " + QString::number(id);
    QSqlQuery query(q);
    if(query.next()){
        return query.value("name").toString();
    }
    else return "";
}

//29-11-2023 - Se recupera el ID del paciente asociado al estudio
QString studies::getPatientIdFromUID(QString uid){
    QString q;
    q += "SELECT id_patients ";
    q += "FROM studies as s ";
    q += "WHERE uid = '"+uid+"' ";
    QSqlQuery query(q);
    qDebug() << q << query.size();
    if(query.next())
        return query.value(0).toString();
    else return "";
}




