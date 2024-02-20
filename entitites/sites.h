#ifndef SITES_H
#define SITES_H

#include <entitites/entities.h>

class sites : public entities
{
public:
    sites();
    ~sites();

    QList<int> listeID(QString );
    QStringList getSites(void);
    void eliminateSite(int id);

/*
    bool logIn(int, QString);
    bool islogged();
    QString opName();
    void session_destroy();
    int lastOp();
    QList<int> listeID(QString );
    static bool isAdmin();
    static bool isSuperAdmin();
private:
    bool isLogin;
    QString name;
    */
};


#endif // SITES_H
