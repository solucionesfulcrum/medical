#ifndef ENTITIES_H
#define ENTITIES_H

#include <QtWidgets>
#include <QtSql>
#include "sqlite.h"
#include <stdio.h>



class entities
{
public:
    entities();
    ~entities();

    void loadData(int i);
    bool eliminar(int id);
    bool update(QHash<QString, QString> data, int id);
    bool execute(QString);
    bool insert(QHash<QString, QString> data);



    QStringList showAll();
    QVariant getValue(QString);
    QString tableName();
    QString idName();
    int lastOp();

    int getLastId();
    void setKeyList(QStringList);

    static QString doUID();
    static QString datetimetoFormat(QString dt, QString s = "dd/MM/yyyy HH:mm:ss");
    static QString getCurrentDateTime();

    static QString JsonToString(QJsonValue);


protected:
    sqlite *db;
    QString table, id_name;
    QStringList keys;
    QHash<QString,QVariant> data;
};

#endif // ENTITIES_H
