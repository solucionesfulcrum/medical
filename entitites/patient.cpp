#include "patient.h"

patient::patient() : entities(){
    table = "patients";
    id_name = "id";
    keys    << "id" << "idp" <<"name" << "last_name" << "birthday" << "sex" << "size"
            << "phone" << "cellphone" << "email" << "height" << "weight" ;
}

patient::~patient(){

}

QList<int> patient::listeID(QString s){
    QList<int> result;
    QSqlQuery query("SELECT id FROM patients WHERE name LIKE '%"+s+"%' OR last_name  LIKE '%"+s+"%' OR idp LIKE '%"+s+"%' ORDER BY last_name");
    while (query.next()) {
        result.append(query.value(0).toInt());
    }
    return result;
}




bool patient::exist(QString id){
    QSqlQuery query("SELECT COUNT(id) as TOTAL FROM patients WHERE idp = '"+id+"' ");
    qDebug() << "SELECT COUNT(id) as TOTAL FROM patients WHERE idp = '"+id+"' ";
    query.next();
    qDebug() << "RESULT COUNT" << query.value(0).toInt();
    if(query.value(0).toInt() > 0) return true;
    else return false;
}

bool patient::existOther(QString id,int i){
    QSqlQuery query("SELECT COUNT(id) as TOTAL FROM patients WHERE idp = '"+id+"' AND id != "+QString::number(i));
    query.next();
    if(query.value(0).toInt() > 0) return true;
    else return false;
}


QString patient::lastName(){
    return getValue("last_name").toString();
}
QString patient::name(){
    return getValue("name").toString();
}
QString patient::birthday(){
    return getValue("birthday").toString();
}
QString patient::id(){
    return getValue("idp").toString();
}
QString patient::sex(){
    return getValue("sex").toString();
}
QString patient::size(){
   return getValue("size").toString();
}
QString patient::phone(){
   return getValue("phone").toString();
}
QString patient::cellphone(){
   return getValue("cellphone").toString();
}
QString patient::email(){
   return getValue("email").toString();
}
QString patient::height(){
   return getValue("height").toString();
}
QString patient::weight(){
   return getValue("weight").toString();
}

int patient::age(){
    QDate bday = QDate::fromString(getValue("birthday").toString(),"yyyyMMdd");
    QDate today = QDate::currentDate();
    int y = 0, m = 0;

    y = today.year() - bday.year();
    if ((m = today.month() - bday.month()) < 0)
    {
        y--;
        m+=12;
    }
    if ((today.day() - bday.day()) < 0)
        if (--m < 0) y--;

    return y;
}

bool patient::updatePatient(QHash<QString, QString> data, QString patient_id){
    return  db->update(data,table,"id",patient_id);
}

