#ifndef CAPTUREPROCESS_H
#define CAPTUREPROCESS_H

#include "QtWidgets"
#include "entitites/config.h"
#include "include.h"


class captureProcess : public QProcess
{
public:
    captureProcess();
    void setFolder(QString);
    QString getFolder();
    bool startRecord();
    bool stopRecord();
    bool killProcess();
    bool isRunning();

    static captureProcess* _captureProcess;

private:
    QString folder;

};

#endif // CAPTUREPROCESS_H
