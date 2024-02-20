#ifndef SERIESWIDGET_H
#define SERIESWIDGET_H

#define VALID_VIDEO 0
#define FILE_NOT_FOUND 1
#define INVALID_PROTOCOL 2
#define NO_SIGNAL 3
#define VIDEO_SUSPENDED 4

#include <qmedicalboxwidget.h>
#include <studyWidget/sweepsline.h>
#include <studyWidget/capturebutton.h>
#include <studyWidget/sendbutton.h>
#include <dialog/myffplay.h>
#include <widgets/slidingstackedwidget.h>
#include "capture/captureprocess.h"
#include <QSound>



class SeriesWidget : public QWidget
{
    Q_OBJECT
public:
    SeriesWidget(QWidget *parent = nullptr);
    ~SeriesWidget();
    void setStudy(int);
    bool isCapturingVideo();
    sendbutton * sendButton ;
    bool StudiesFinished;
    int protocol_id;

    sweepsLine * _sweepsline;

    QPushButton *backButton,*nextButton;
    QPushButton *finishStudyButton,*sendStudyButton;
    captureProcess * _captureProcess;



protected:
    void closeEvent(QCloseEvent *);

signals:
    void finished(bool);
    void sendToQueue(int);
    void finishedStudy(bool);
    void changePicture(int,int);

private slots:
    void startRecord();
    void stopRecord();
    void send();
    void sendStudy();
    void look();
    void refreshCaptureTime();
    void setToolsEnabled(bool b);
    void processFinished(int);
    void processStarted();
    void processData();
    void finishStudy();
    void restartSerie();
    void cptSlideFinished();    
    void backButtonSlot();
    void nextButtonSlot();

private:
    void initCaptureButton();
    void createToolBox();
    void createCaptureBox();
    void createListBox();
    void setActual(int);
    int validateVideo(QString);

    QString ffmpegErr;

    SlidingStackedWidget *cpt;

    QTime t;


    studies _studies;
    series _series;
    config _config;

    QWidget
    *toolBox,
    *listBox;
    studydesc sd;
    int idStudy;


    QToolButton
    *lookButton,
    *restartButton;
    bool isCapturing;

    captureButton * _captureButton;


    QLabel * resultLabel;

    QString f;
    QTimer *poller;
    QTime _time;
};

#endif // SERIESWIDGET_H
