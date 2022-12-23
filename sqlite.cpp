#include "sqlite.h"

sqlite::sqlite() {
    base = QDir::currentPath()+"/medicalbox.db";
    if (QFile::exists(base))
        init();
    else {
        QMessageBox::critical(0,QObject::tr("¡Base de datos inexistente!"),QObject::tr("La base de datos es inexistente"));
    }
}

void sqlite::init()
{
    if (db.connectionNames().empty()) {
        db = QSqlDatabase::addDatabase("QSQLITE");
#ifdef Q_OS_LINUX
        QString path(QDir::home().path());
        path.append(QDir::separator()).append(base);
        path = QDir::toNativeSeparators(path);
        db.setDatabaseName(path);
#else
        db.setDatabaseName(base);
#endif
    }
}

sqlite::~sqlite() {
    if (db.isOpen()) db.close();
    db.removeDatabase(db.connectionName());
}

QString sqlite::addslashs(QString s) {
    s = s.replace("'", "&QUOTE;");
    s = s.replace("\"", "&DQUOTE;");
    return s;
}

QString sqlite::changeDate(QString s) {
    QString y = s.mid(6,4);
    QString m = s.mid(3,2);
    QString d = s.left(2);
    QString result = y+"-"+m+"-"+d;
    return result;
}

QDate sqlite::getDate(QString s) {
    int d = s.right(2).toInt();
    int m = s.mid(5,2).toInt();
    int y = s.left(4).toInt();
    QDate result(y,m,d);
    return result;
}

QString sqlite::stripslashs(QString s) {
    s = s.replace("&QUOTE;", "'");
    s = s.replace("&DQUOTE;", "\"");
    return s;
}

bool sqlite::openDB() {
    if (!db.isOpen())
        return db.open();
    else
        return true;
}

QSqlError sqlite::lastError() {
    return db.lastError();
}

void sqlite::closeDB() {
    db.close();
}

bool sqlite::deleteDB() {
    // Close database
    db.close();
#ifdef Q_OS_LINUX
    QString path(QDir::home().path());
    path.append(QDir::separator()).append(base);
    path = QDir::toNativeSeparators(path);
    return QFile::remove(path);
#else
    return QFile::remove(base);
#endif
}

bool sqlite::insert(QHash<QString, QString> hash, QString table) {
    QString query;
    QList<QString> key = hash.keys();
    query = "INSERT INTO " + table + "( ";
    for (int i = 0; i < key.size(); ++i) {
        query += key[i];
        if (i + 1 < key.size())
            query += ", ";
    }
    query += " ) VALUES ( ";
    for (int i = 0; i < key.size(); ++i) {
        query += "'" + this->addslashs(hash.value(key[i])) + "'";
        if (i + 1 < key.size())
            query += ", ";
    }
    query += ")";
    QSqlQuery q;

    if (q.prepare(query)){
        if (q.exec(query)){
            lastId = q.lastInsertId().toInt();
            return true;
        }
        else return false;
    }
    else
        return false;

}

int sqlite::getLastId(){
    return lastId;
}

QSqlDatabase sqlite::getDB(){
    return db;
}

bool sqlite::update(QHash<QString, QString> hash, QString table, QString id, QString idValue) {
    QString query;
    QList<QString> key = hash.keys();
    query = "UPDATE " + table + " SET ";
    for (int i = 0; i < key.size(); ++i) {
        query += "" + key[i] + " = ";
        query += "'" + this->addslashs(hash.value(key[i])) + "'";
        if (i + 1 < key.size())
            query += ", ";
    }
    query += " WHERE " + id + " = '" + idValue + "' ";

    QSqlQuery q;
    if (q.prepare(query))
        return q.exec(query);
    else
        return false;
}



bool sqlite::update(QHash<QString, QString> hash, QString table, QString id, int idValue) {
    QString query;
    QList<QString> key = hash.keys();
    query = "UPDATE " + table + " SET ";
    for (int i = 0; i < key.size(); ++i) {
        query += "" + key[i] + " = ";
        query += "'" + this->addslashs(hash.value(key[i])) + "'";
        if (i + 1 < key.size())
            query += ", ";
    }
    query += " WHERE " + id + " = " + QString::number(idValue);
    qDebug() << query;
    QSqlQuery q;
    if (q.prepare(query))
        return q.exec(query);
    else
        return false;
}


bool sqlite::UpdateLastElement(QHash<QString,QString> hash,QString table){
    QString query;
    QList<QString> key = hash.keys();
    query = "UPDATE " + table + " SET ";
    for (int i = 0; i < key.size(); ++i) {
        query += "" + key[i] + " = ";
        query += "'" + this->addslashs(hash.value(key[i])) + "'";
        if (i + 1 < key.size())
            query += ", ";
    }
    //query += " WHERE " + id + " = " + QString::number(idValue);
    query += " where id = (SELECT max(ID) as last FROM "+table+" )";
    qDebug() << query;
    QSqlQuery q;
    if (q.prepare(query))
        return q.exec(query);
    else
        return false;



}

bool sqlite::exec(QString query) {
    //qDebug() << query;
    QSqlQuery q;
    if (q.prepare(query))
        return q.exec(query);
    else
        return false;
}

bool sqlite::del(QString table, QString id, int idValue) {
    QString query;
    query = "DELETE FROM " + table + " WHERE " + id + " = " + QString::number(idValue);
    QSqlQuery q;
    //qDebug() << query;
    if (q.prepare(query))
        return q.exec(query);
    else
        return false;
}
