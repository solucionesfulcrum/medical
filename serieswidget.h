#ifndef SERIESWIDGET_H
#define SERIESWIDGET_H

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
protected:
    void closeEvent(QCloseEvent *);
signals:
    void finished(bool);
    void sendToQueue(int);
    void finishedStudy(bool);
private slots:
    void startRecord();
    void stopRecord();
    void send();
    void look();
    void refreshCaptureTime();
    void setToolsEnabled(bool b);
    void processFinished(int);
    void processStarted();
    void processData();
    void finishStudy();
    void restartSerie();
    void cptSlideFinished();
private:
    void initCaptureButton();
    void createToolBox();
    void createCaptureBox();
    void createListBox();
    void setActual(int);

    QString ffmpegErr;

    SlidingStackedWidget *cpt;

    QTime t;
    sweepsLine * _sweepsline;
    captureButton * _captureButton;

    studies _studies;
    series _series;
    config _config;

    QWidget
    *toolBox,
    *listBox;
    studydesc sd;
    int idStudy;

    sendbutton * sendButton ;

    QToolButton
    *lookButton,
    *restartButton;
    bool isCapturing;

    QPushButton * finishStudyButton;

    QLabel * resultLabel;

    QString f;
    QTimer *poller;
    QTime _time;
    captureProcess * _captureProcess;


};

#endif // SERIESWIDGET_H
