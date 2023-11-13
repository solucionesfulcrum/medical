#include "sites.h"

sites::sites() : entities()
{
    table = "sites";
    id_name = "id";
    keys << "name" << "enable";
}


sites::~sites()
{

}

QStringList sites::getSites(void)
{
    QStringList sites;
    QString q = "SELECT name FROM "+table+" WHERE enable=1";

    QSqlQuery query(q);
    while (query.next())
    {
        sites.append(query.value(0).toString());
    }

    return sites;
}

QList<int> sites::listeID(QString s){
    QList<int> result;
    QSqlQuery query("SELECT id FROM "+table+" WHERE name LIKE '%"+s+"%' ORDER BY name");
    while (query.next())
        result.append(query.value(0).toInt());
    return result;
}

void sites::eliminateSite(int id)
{
    bool result;
    QString str;
    QSqlQuery query;

    str = "delete from " + table + " where id=" + QString::number(id);
    qDebug()<<str;
    query.prepare(str);
    result = query.exec();

}
