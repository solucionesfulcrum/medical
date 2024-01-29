#include "entities.h"

entities::entities()
{
    db = new sqlite();
    db->openDB();


}

entities::~entities()
{
    db->closeDB();
    delete db;
}

QString entities::tableName(){
    return table;
}

QString entities::idName(){
    return id_name;
}

QStringList entities::showAll(){
    QStringList r;
    foreach(QString k, keys)
        r.append(getValue(k).toString());
    return r;
}

bool entities::update(QHash<QString, QString> data, int id){
    return db->update(data, table, id_name, id);
}
bool entities::execute(QString query){
    return db->exec(query);
}


bool entities::insert(QHash<QString, QString> data){
    return db->insert(data, table);
}

bool entities::eliminar(int id){
    return db->del(table,id_name,id);
}

void entities::setKeyList(QStringList s){
    keys  =s ;
}

int entities::getLastId(){
    return db->getLastId();
}

QVariant entities::getValue(QString k){
    return data[k];
}


void entities::loadData(int id){
    if (id!=0){
        QString q = "SELECT * FROM "+table+" WHERE "+id_name+" = "+QString::number(id)+" LIMIT 0,1";
        qDebug() << q;
        QSqlQuery query(q);
        while (query.next()){
            foreach(QString s , keys){                
                data[s] = query.value(s);
            }
        }
    }
}

//-------------------------------------------------

QString entities::doUID(){
    QString uid = "1.2.826.0.1.3680043.2.";
    qint64 time = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000;
    QString HDD = "";
    DWORD dwVolSerial;
    bool bIsRetrieved = GetVolumeInformation(TEXT("C:\\"), NULL, NULL, &dwVolSerial, NULL, NULL, NULL, NULL);
    if (bIsRetrieved){
        HDD =  QString::number(dwVolSerial,16);
        bool bStatus = false;
        int nHex = HDD.toUInt(&bStatus,16);
        HDD =  QString::number(nHex);
    }
    uid += HDD+"."+QString::number(time);
    return uid;
}

QString entities::datetimetoFormat(QString dati, QString s){
    QDateTime dt;
    bool ok;
    qint64 timestamp = dati.toLongLong(&ok);
    dt.setMSecsSinceEpoch(timestamp);

    return dt.toString(s);
}


QString entities::getCurrentDateTime(){
    return QString::number(QDateTime::currentDateTime().toMSecsSinceEpoch());
}


QString entities::JsonToString(QJsonValue val){
    QString res = "";
    if (val.isString())
       res = val.toString();
    if (val.isDouble())
        res = QString::number(val.toInt());
    return res;

}
