#ifndef STUDY_H
#define STUDY_H

#include <qmedicalboxwidget.h>
#include <serieswidget.h>
#include <entitites/operators.h>
#include <studyWidget/studyprotocols.h>
#include <studyWidget/studyinfowidget.h>
#include <studyWidget/studyprotocolsform.h>
#include <studyWidget/clinicdatawidget.h>
#include <widgets/slidingstackedwidget.h>

class study : public QMedicalBoxWidget
{
    Q_OBJECT
public:
    study(QMedicalBoxWidget *parent = nullptr);
    ~study();
public slots:
    void patientLoaded(int);
    void loadStudy(int);
    void startStudy();
    void newStudy(bool = false);
    void changePatient();
    void changeProtocol();
    bool isCapturing();
    void isnewStudy(bool = false);

signals:
    void studyStarted(bool);
private slots:
    void protocolSelected();

private:
    void setForm();
    void setStudyProtocolsForm();
    void setClinicsData();
    void setPatientForm();

    bool Falta_FUR_o_FPP();
    bool Falta_trimestre();
    void MuestraUltimoUltrasonido();
    uint8_t validateCardiacBeat();

    QPushButton * start;

    SlidingStackedWidget * studyForm;
    SeriesWidget *_seriesWidget;

    studyInfoWidget * studyInfo;
    studyProtocols * _studyDesc;
    studyProtocolsForm *_studyProtocolsFrom;
    dialogPatient * patientWidget;
    QWidget * _clinicdata;
    clinicDataWidget * _clinicdatawidget;
    QVkLineEdit * _reason;

    operators ope;

    int studyId;
    int _patient_id;
    int last_serie;
    bool started;


};

#endif // STUDY_H
