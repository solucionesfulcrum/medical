#ifndef STUDIES_H
#define STUDIES_H

#include <entitites/entities.h>

class studies : public entities
{
public:
    studies();
    ~studies();


    QString uid();
    QString datetime();
    QString desc();
    QString reason();
    int id_patient();
    int id_doctor();
    QList<int> listeID(QString query = "");
    QStringList getToConfirm();
    QStringList getAllNotReported();

//---------------------------------------------
//  CR: 28/02/21
    bool UpdateLastElement(QHash<QString,QString> data);
    QStringList getStudiesNotReported(void);
//---------------------------------------------

    QStringList getNotFinished();
    QStringList getPending();
    QString getState();
    QString getData();
    int getLastElement(QString name);

    bool updateStudy(QHash<QString, QString> data, QString uid);
    int getIdFromUID(QString);
    static QString getStateName(int);
    static bool deleteStudy(int);
    static bool deleteFolder(QString);

private:
    QString _uid, _datetime, _desc, _reason;
    int _id_patient, _id_doctor;
};

#endif // STUDIES_H
