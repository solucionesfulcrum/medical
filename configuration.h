#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <qmedicalboxwidget.h>
#include "accesor.h"
#include "entitites/operators.h"
#include "dialog/myffplay.h"
#include <QtMultimedia/QCamera>
#include "ejecuta.h"
#include "windows.h"

class cleaner : public QThread
{
    Q_OBJECT
public:
    void setList(QStringList);
protected:
    void run();
signals:
    void progress(int,int);

private:


    QStringList ids;
    studies _studies;
};

enum labels{
    IP,
    NOMBRE,
    PASS,
    VIDEOINPUT,
    VBITRATE,
    FPS,
    VSIZE,
    PIXCONF,
    VBITRATEMIN,
    VBITRATEMAX,
    MODELUS,
    KEEP_UNCOMPRESSED,
    VIDEODEVICE
};


class configuration : public QMedicalBoxWidget
{
    Q_OBJECT
public:
    explicit configuration(QMedicalBoxWidget *parent = 0);
    ~configuration();
    void updateCams();
    TouchComboBox   * _TimeoutInactivity;
    QCheckBox       *_consent;

signals:
    void Signal_Consent(bool);
    void Signal_Timeout(int);

private slots:
    void save();
    void checkVideo();
    void setVideoDevice();
    void cleanStudies();
    void progressClean(int, int);
    void cleanFinished();
    void isChanged(QString);
    void isChanged(bool);
public slots:
    void refreshOpe();

private:
    void setLayoutForm(QLayout *);
    void setInputs();
    void createQLabels();
    void setServerForm();
    void setAcquisitionForm();
    void setCompressionForm();
    void setMaintenanceForm();
    void setConfigurationForm();
    void load();

    void RunCmd(QString comando);
    QString CheckAdquisitionDevice(void);

    QTabWidget * configWidget;

    QCamera cam;
    TouchComboBox *equipo;

    bool changed;
    QWidget
    * _serverForm,
    * _acquisitionForm,
    * _compressionForm,
    * _maintenanceForm,
    * _configurationForm;

    QVkLineEdit
    * _pass,
    * _name,
    * _modelUS;

    QCheckBox
    * _keep_uncompressed;


    TouchComboBox
    * _serverIp,
    * _defaultvideobitrate,
    * _videoInput,
    * _fps,
    * _size,
    * _pixelConf,
    * _vbitratemin,
    * _vbitratemax;

    QPushButton * saveButton;
    QPushButton *cleanButton;
    studyDescUpdate *dsc;
    QList<QLabel*> labelList;

};

#endif // CONFIGURATION_H
