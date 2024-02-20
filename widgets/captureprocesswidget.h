#ifndef CAPTUREPROCESSWIDGET_H
#define CAPTUREPROCESSWIDGET_H

#include "QtWidgets"
#include "capture/captureprocess.h"
#include <QtMultimedia/QCamera>

class captureProcessWidget : public QWidget
{
    Q_OBJECT
public:
    explicit captureProcessWidget(QWidget *parent = nullptr);

signals:

public slots:
    void refresh();
    void refreshDevice();

private slots:
    void killProcess();
    void checkDevice();

private:
    bool isDevice();
    QCamera cam;
    QPushButton * info;
    QPushButton * device;
    QTimer poller;
    QTimer pollerDevice;

};

#endif // CAPTUREPROCESSWIDGET_H
