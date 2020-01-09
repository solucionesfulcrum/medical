#ifndef CHECKBANDWITH_H
#define CHECKBANDWITH_H

#include <QtWidgets>
#include<QNetworkAccessManager>
#include<QHttpMultiPart>
#include<QHttpPart>
#include<QNetworkReply>
#include <entitites/config.h>
#include "include.h"

static const qint64 sec = 1000000000;
static const int bit = 8;


class checkBandwith : public QWidget
{
    Q_OBJECT
public:
    checkBandwith(QWidget * parent = 0);
    ~checkBandwith();
    int getBit();
    static int Bit();
    void checking();
    void findBestPack();
    void setStep();
    void setURL(QString);
    qint64 getBestPack();
    static qint64 getVideoBitrate();
    void showResult();
public slots:
    void start();
    void send(int s);
    void finished(QNetworkReply*);
signals:
    void result(QString);
    void changed();
private:
    config conf;
    int pruebas;
    QMovie *loader;
    QLabel
    *_title,
    *_logo,
    *_valueLabel,
    *_information,
    *_threadInfo;
    QPushButton *_startCheck;
    QNetworkRequest request;
    int
    maxtime
    ,nextvalue
    ,size
    ,getAverage;
    bool isfinish;
    QList<qint64> step;
    qint64 bestPack
    ,elapsedTime;
    QElapsedTimer timer;
    QUrl url;
    QNetworkAccessManager m_WebCtrl;
};

#endif // CHECKBANDWITH_H
