#ifndef STUDYCHECK_H
#define STUDYCHECK_H

#include<QNetworkAccessManager>
#include<QHttpMultiPart>
#include<QHttpPart>
#include<QNetworkReply>
#include <entitites/series.h>
#include <entitites/patient.h>
#include <entitites/studies.h>
#include <entitites/config.h>
#include <medisecure/include/medisecure.h>
#include <QThread>
#include <include.h>

class saveJson : public QThread
{
    Q_OBJECT
public:
    void setJson(QByteArray);
protected:
    void run();
private:
    QByteArray res;
    series _series;
    patient _patient;
    studies _studies;
};



class studyCheck: public QObject
{
    Q_OBJECT
public:

    static QString API;
    studyCheck();
    ~studyCheck();

    void stop();
    void start();
    void setSleepTime(int);
    bool isFinished(void);
    void refreshStudies(void);


private slots:
    void finished(QNetworkReply*);
    void send();
    void timeout();

    //void getError(QNetworkReply::NetworkError);
    //void finish();

signals:
    void studyupdated(int id);
    void isFinished(int,int);
    void error(int);
private:
    saveJson * saver;
    void addPart(QString name, QString value, QString  type = "form-data");

    bool sendFinished;
    bool err;

    QNetworkReply *rp;
    QNetworkRequest request;
    QUrl url;
    QNetworkAccessManager *m_WebCtrl;
    series _series;
    patient _patient;
    studies _studies;
    config _cfg;
    QTimer m_timer;
    bool m_timeout;

    QHttpMultiPart *mtp;

    int sleepTime;
};

#endif // STUDYCHECK_H
