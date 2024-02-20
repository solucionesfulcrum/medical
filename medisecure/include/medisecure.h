#ifndef MEDISECURE_H
#define MEDISECURE_H

#include "medisecure_global.h"
#include "aescript.h"
#include <QCoreApplication>
#include <QFile>
#include <QStringList>
#include <QDebug>
#include <QDateTime>
#include <iostream>
#include "aescript.h"

enum operation{
    crypt,
    decrypt
};

enum error{
    good,
    badfile,
    filenotexist,

};

class MEDISECURESHARED_EXPORT Medisecure
{

    public:
    Medisecure();
    void setRawKey(QVector<int>);
    void setFile(QString);
    void setOperation(int);
    int start();

    private:
    QByteArray getAesKey();
    QVector<int> rawKey;
    QString filename;
    int operation;

};

#endif // MEDISECURE_H
