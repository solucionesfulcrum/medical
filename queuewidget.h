#ifndef QUEUEWIDGET_H
#define QUEUEWIDGET_H

#include <QtWidgets>
#include <checkbandwith.h>
#include <entitites/series.h>
#include <entitites/patient.h>
#include <entitites/studies.h>
#include <entitites/config.h>
#include <entitites/operators.h>
#include <entitites/studydesc.h>
#include <QNetworkAccessManager>
#include <QAuthenticator>
#include <QHttpMultiPart>
#include <QHttpPart>
#include <QNetworkReply>
#include <medisecure/include/medisecure.h>
#include "include.h"
#include "accesor.h"
#include "widgets/titlelabel.h"


class HTTPsender : public QObject
{
    Q_OBJECT
public:
    static QString API;
    HTTPsender( QObject *o = 0);
    ~HTTPsender();
    void state();
    void send(int);

private slots:
    void finished(QNetworkReply*);

    void dl(qint64,qint64);
    void showError(QNetworkReply::NetworkError);

    void GetReply(void);
    void UploadError(QNetworkReply::NetworkError);

    void provideAuth(QNetworkReply*,QAuthenticator*);
    void sslErrors(QNetworkReply*,const QList<QSslError>&);
    void handleNetworkAccessibleChange(QNetworkAccessManager::NetworkAccessibility accessible);

signals:
    void isFinished(int,int);
    void progress(qint64,qint64,int);
    void isError(int);
private:
    void addPart(QString k, QString v, QString type = "form-data");
    void addDevicePart(QString k, QFile* v, QString type = "form-data");
    bool stateConnection;
    QHttpMultiPart *mtp;
    QNetworkReply *rp;
    QNetworkRequest request;
    QUrl url;
    QNetworkAccessManager m_WebCtrl;
    series _series;
    patient _patient;
    studies _studies;
    operators _operators;
    config _cfg;
    QFile *filemeta, *file;
    //QFile errfile;
    int id;
};


class Queue : public QThread
{
    Q_OBJECT
public:
    void add(int);
    void stop();
    void removeID(int);
    QString EliminateAccent(QString s);

public slots:
    void next();
protected:
    void run();

signals:
    void isCompressed(int);
    void isCrypted(int);
    void isError(int);
    void isWaiting(bool);
    void isFinish();
private:
    int getFileSize(QString);
    QByteArray createMetaData(int );
    QString dateTimeToDCM(QString, QString = "date");
    bool finish, wait;
    series _series;
    patient _patient;
    studies _studies;

    QList<int> queues;



};

class QueueObject : public QWidget
{
    Q_OBJECT
public:
    explicit QueueObject(int id, QWidget *parent = 0);
    ~QueueObject();
    int id();

    void isSent();
    void isCompressed();
    void isCrypted();
    void isQueued();

    void error();
    void removeID(int);
    bool isFinished();
signals:
     void deleteWidg(int);
public slots:
    void setProgress(qint64,qint64);
    void deleteQueue();

private:
    bool finished;
    QPushButton * del;
    int numError;
    QProgressBar * pb;
    series _series;    
    int _id;
    QList<QLabel *> statut;
    QLabel
    * compressed,
    * crypted,
    * sent;


};

class QueueWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QueueWidget(QWidget *parent = 0);
    ~QueueWidget();
    void add(int id);
    void start();
    void stop();
    bool IsThreadRunning();
    QWidget * infoWidget();
signals:
    void isFinished(int);
    void isThreadFinished();
    void hideQueue();
public slots:
    void setUpProgress(qint64,qint64,int);
    void deleteQueue(int);
    void waitingThread(bool);
    void clean();
    void refreshInfo();
private slots:
    void isCompressed(int);
    void isCrypted(int);
    void isError(int);
    void isFinished(int,int);
    void threadIsFinished();
private:
    QueueObject* queueObject(int id);
    bool isRunning;

    QLabel
    *infoLabel,
    *queueStatut,
    *numberStatut;
    QWidget * _infoWidget;

    QWidget *QueueWidgetList;
    HTTPsender *_httpsend;
    Queue q;
    void load();
    QVBoxLayout *queueLayout;

    checkBandwith * cb;
    QScrollArea * _area;
    QList<QueueObject *> queuesObjects;
    studies _studies;

public:
    series _series;

};

#endif // QUEUEWIDGET_H
