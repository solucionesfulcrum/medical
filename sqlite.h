#ifndef SQLITE_H
#define	SQLITE_H
#include <QtWidgets>
#include <QObject>
#include <QDate>
#include <QHash>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QFile>



class sqlite {
public:
    sqlite();
    ~sqlite();
    void init();
    bool openDB();
    bool deleteDB();
    void closeDB();
    QSqlError lastError();
    QString changeDate(QString s);
    QDate getDate(QString s);
    QString addslashs(QString s);
    QString stripslashs(QString s);
    bool insert(QHash<QString,QString> hash, QString table);
    bool update(QHash<QString,QString> hash, QString table, QString id, int idValue);
    bool update(QHash<QString,QString> hash, QString table, QString id, QString idValue);
    bool exec(QString query);
    bool del(QString table, QString id, int idValue );
    int getLastId();
    QSqlDatabase getDB();
private:
    QString base;
    QSqlDatabase db;
    int lastId;
};

#endif	/* SQLITE_H */
