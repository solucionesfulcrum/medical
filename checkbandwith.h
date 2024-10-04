#ifndef CHECKBANDWITH_H
#define CHECKBANDWITH_H

#include <QtWidgets>
#include <QNetworkAccessManager>
#include <QHttpMultiPart>
#include <QHttpPart>

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
    void send();

public slots:
    void start();    

private slots:
    void Finished(int ExitCode, QProcess::ExitStatus ExitStatus);
    void ReadyReadStandardError(void);
    void ReadyReadStandardOutput(void);

signals:
    void Wifi_status(int8_t);

private:
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
