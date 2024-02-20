#ifndef CHECKBANDWITH_H
#define CHECKBANDWITH_H

#include <QtWidgets>

#include <QNetworkAccessManager>
#include <QHttpMultiPart>
#include <QHttpPart>
#include<QNetworkReply>

#include <entitites/config.h>
#include <include.h>

class checkBandwith : public QWidget
{
    Q_OBJECT
public:
    checkBandwith(QWidget * parent = 0);
    ~checkBandwith();    

//--------------------------------------
//  CR: 07/03/21
private:

    void addPart(QHttpMultiPart *m,QString name, QString value, QString  type = "form-data");


public slots:
    void start();    

private slots:
    void send(void);
    void FinishedMessage(QNetworkReply* pReply);

signals:
    void Wifi_status(int8_t);

private:
    QNetworkAccessManager *m_WebCtrl;
    QTimer m_timer;
    QHttpMultiPart *mtp;
    QNetworkRequest request;
    QUrl url;
    config _cfg;
    QNetworkReply *rp;



    config conf;
    QMovie *loader;
    QLabel *_valueLabel;
    QPushButton *_startCheck;

    QString mProcessMsg;
    bool IsRunning;
    QProcess mProcess;
//---------------------------------------------
};

#endif // CHECKBANDWITH_H
