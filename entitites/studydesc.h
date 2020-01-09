#ifndef STUDYDESC_H
#define STUDYDESC_H

#include <entitites/entities.h>
#include<QNetworkAccessManager>
#include<QHttpMultiPart>
#include<QHttpPart>
#include<QNetworkReply>
#include <entitites/config.h>
#include <include.h>

#include<QObject>


class studydesc : public entities
{

public:
    studydesc();
    ~studydesc();

    QList<int> get();
    QStringList getProtocols();
    QStringList getSweep(QString);
    QString getSweepName(int id);
    QList<int> getSweepId(int);
    QString getJson();
    bool insertSweeps(QHash<QString,QString>);




};

class studyDescUpdate : public QObject
{
    Q_OBJECT
public:
    static QString API;
    studyDescUpdate();
    void protocolsUpdate();
    void addPart(QHttpMultiPart *,QString, QString);
    QString err;
public slots:
    void cancel();

signals:
    void finished();
private slots:
    void finished(QNetworkReply*);
    void dl(qint64,qint64);
private:
    QNetworkReply *rp;
    QNetworkRequest request;
    QUrl url;
    QNetworkAccessManager m_WebCtrl;
    config _cfg;

};

#endif // STUDYDESC_H
