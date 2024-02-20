#ifndef BATTERY_H
#define BATTERY_H

#include <QtWidgets>
#include <windows.h>


class battery : public QWidget
{
    Q_OBJECT
public:
    explicit battery(QWidget *parent = 0);

signals:

private slots:
    void refresh();


private:
    QProgressBar *pg;
    SYSTEM_POWER_STATUS status;
    QTimer poller;
    QLabel * bat;
     QLabel * ac;
};

#endif // BATTERY_H
