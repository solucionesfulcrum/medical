#ifndef PATIENT_H
#define PATIENT_H

#include <entitites/entities.h>

class patient : public entities
{

public:
    patient();
    ~patient();
    QString name();
    QString lastName();
    QString birthday();
    QString id();
    QString sex();
    QString size();
    QString phone();
    QString cellphone();
    QString email();
    QString height();
    QString weight();
    bool exist(QString id);
    bool existOther(QString id,int i);
    QList<int> listeID(QString seek = "");
    int age();
private:
    QString _name, _lastName, _birthday, _id, _sex, _size, _phone, _cellphone, _email, _height, _weight;

};

#endif // PATIENT_H
