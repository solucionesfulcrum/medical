#ifndef STUDYFINISHED_H
#define STUDYFINISHED_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QHttpMultiPart>
#include <QNetworkReply>
#include <QTimer>

#include <entitites/series.h>
#include <entitites/patient.h>
#include <entitites/studies.h>
#include <entitites/config.h>
#include <include.h>

class StudyFinished : public QObject
{
    Q_OBJECT
public:
    explicit StudyFinished(QObject *parent = nullptr);

private:
    QNetworkAccessManager *pWebCtrl;
    QUrl Url;
    QHttpMultiPart *mtp;
    QNetworkRequest *request;
    QNetworkReply *netReply;

    QTimer *pTimer;
    config _cfg;

    studies _studies;

    void addPart(QString name, QString value, QString  type = "form-data");

public:
    void start(void);

private slots:
    void MessageFinished(QNetworkReply* p);
    void SendStudystatus(void);

signals:


};

#endif // STUDYFINISHED_H





