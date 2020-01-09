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
    bool exist(QString id);
    bool existOther(QString id,int i);
    QList<int> listeID(QString seek = "");
private:
    QString _name, _lastName, _birthday, _id, _sex, _size;

};

#endif // PATIENT_H
