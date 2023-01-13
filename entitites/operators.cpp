#include "operators.h"

operators::operators() : entities()
{
    table = "operators";
    id_name = "id";
    keys << "id" << "name" << "pass" << "admin" <<"enable" ;
    isLogin = false;
    name = "";
}


operators::~operators()
{

}

void operators::session_destroy(){
    QHash<QString,QString> up;
    up.insert("logged","0");
    db->update(up,"session","id",1);
}

QList<int> operators::listeID(QString s){
    QList<int> result;
    QSqlQuery query("SELECT id FROM "+table+" WHERE name LIKE '%"+s+"%' ORDER BY name");
    while (query.next())
        result.append(query.value(0).toInt());
    return result;
}

QString operators::opName(){
    QString q = "SELECT id_operators FROM session WHERE id = 1";
    QSqlQuery query(q);
    if(query.next()){
        loadData(query.value("id_operators").toInt());
        return data.value("name").toString();
    }
    else return "";
}

int operators::lastOp(){
    QString q = "SELECT id_operators FROM session WHERE id = 1";
    QSqlQuery query(q);
    if(query.next()){
        return query.value("id_operators").toInt();
    }
    else return -1;
}

bool operators::islogged(){
    bool res = false;
    QString q = "SELECT logged FROM session WHERE id = 1";
    QSqlQuery query(q);
    if(query.next()){
        if (query.value("logged").toInt() == 1)
            res = true;
    }
    return res;
}

bool operators::logIn(int n, QString p)
{
    isLogin = false;

    if (n != 0 && p != ""){
        QString q = "SELECT * FROM "+table+" WHERE id = '"+QString::number(n)+"' AND pass = '"+p+"' LIMIT 0,1";
        qDebug() << q;
        QSqlQuery query(q);
        if(query.next()){
            QHash<QString,QString> up;
            up.insert("id_operators",query.value("id").toString());
            up.insert("logged","1");
            db->update(up,"session","id",1);
            isLogin = true;
        }
        else isLogin = false;
    }
    return isLogin;
}

bool operators::isAdmin(){
    operators ope;
    ope.loadData(ope.lastOp());
    qDebug() << "ISADMIN" <<ope.getValue("admin").toInt();
    if (ope.getValue("admin").toInt() == 1 )
        return true;
    else return false;
}
