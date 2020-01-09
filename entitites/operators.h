#ifndef OPERATORS_H
#define OPERATORS_H

#include <entitites/entities.h>

class operators : public entities
{
public:
    operators();
    ~operators();
    bool logIn(int, QString);
    bool islogged();
    QString opName();
    void session_destroy();
    int lastOp();
    QList<int> listeID(QString );
    static bool isAdmin();
private:
    bool isLogin;
    QString name;
};


extern operators ope;

#endif // OPERATORS_H
