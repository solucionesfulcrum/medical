#ifndef QHISTWIDGET_H
#define QHISTWIDGET_H

#include <QtWidgets>
#include <entitites/config.h>
#include <entitites/patient.h>
#include <entitites/series.h>
#include <entitites/studies.h>
#include <entitites/studydesc.h>
#include <entitites/operators.h>
#include "accesor.h"
#include "dialog/myffplay.h"
#include "widgets/titlelabel.h"


class sweepInfo : public QWidget
{
    Q_OBJECT
public:
    explicit sweepInfo(int id, QWidget *parent = 0);
private slots:
    void refresh();
    void view();
private:
    QLabel
    * _toqueue,
    * _capture,
    * _sent,
    * _confirmed;
    series _series;
    QPushButton * lookVid;
    studydesc _studydesc;
};


class studyInfo : public QDialog
{
    Q_OBJECT
public:
    explicit studyInfo(int id, QDialog *parent = 0);
    //~studyInfo();
protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void download();
    void refresh();
    void replyFinished(QNetworkReply*);
    void dl(qint64 a ,qint64 n);

private :
    void createStudyBox();
    void createSweepsBox();
    void openFile();
    QPushButton *dlButton ;
    QPushButton *refreshButton;
    uint8_t typeDownload;
    patient _patient;
    studies _studies;
    series _series;
    studydesc _studydesc;
    operators _operators;

    QString filename;
    QFile report;

    QNetworkAccessManager *manager;
    QNetworkReply *pReply;

    int studyID;

    QScrollArea * area;

    QWidget
    * studyBox,
    * sweepsBox;

};

class studyAIDiagnotics : public QDialog
{
    Q_OBJECT
public:
    explicit studyAIDiagnotics(int id, QDialog *parent = 0);

protected:
    void closeEvent(QCloseEvent *event);

private:
    studies _studies;

};


class QHistWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QHistWidget(int id, QWidget *parent = 0);
    ~QHistWidget();
signals:
    void loadStudyId(int);
    void deletedStudy(QHistWidget*);


protected:
    void mouseReleaseEvent(QMouseEvent *);

private slots:
    void openInfo(void);
    void loadStudy(void);
    void deleteStudy(void);
    void loadAIDiagnostic(void);

private :
    QPushButton * getInfo;
    QLabel *statut;
    patient _patient;
    studies _studies;
    series _series;
    studydesc _studydesc;
    config _cfg;
    QPushButton *pb;
    int sid;
};

#endif // QHISTWIDGET_H
