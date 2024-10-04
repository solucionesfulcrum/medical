#ifndef QMEDICALBOXWIDGET_H
#define QMEDICALBOXWIDGET_H




#include <form/touchcombobox.h>
#include <form/qvklineedit.h>
#include <dialog/dialogpatient.h>
#include "entitites/patient.h"
#include "entitites/studies.h"
#include "entitites/series.h"
#include "entitites/config.h"
#include "entitites/studydesc.h"
#include "checkbandwith.h"
#include "queuewidget.h"
#include "qheaderwidget.h"
#include <QtWidgets>



class QMedicalBoxWidget : public QWidget
{
     Q_OBJECT
public:
    QMedicalBoxWidget(QWidget *parent = 0);
    ~QMedicalBoxWidget();
    void setQueueWidget(QueueWidget *);


protected slots:
    void sendToQueue(int);

protected:

    QVBoxLayout *mainLayout;
    QueueWidget *_queuewidget;
    studies _studies;
    series _series;
    config _config;
    QProcess *videoplayer;
};

#endif // QMEDICALBOXWIDGET_H
