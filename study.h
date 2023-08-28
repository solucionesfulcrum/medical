#ifndef STUDY_H
#define STUDY_H


#define CardiacFreqMin 0
#define CardiacFreqMax 300
#define RespiratoryFreqMin 0
#define RespiratoryFreqMax 40
#define OxySaturationMin 0
#define OxySaturationMax 100
#define TemperaturaMin 25
#define TemperaturaMax 45



#include <qmedicalboxwidget.h>
#include <QMessageBox>
#include <serieswidget.h>
#include <studyfinished.h>
#include <include.h>

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
    int8_t wifi_status;
    bool ConsentState;
    SlidingStackedWidget * studyForm;

public slots:
    void patientLoaded(int);
    void loadStudy(int);
    void startStudy();
    void newStudy(bool = false);
    void changePatient();
    void changeProtocol();
    bool isCapturing();
    void isnewStudy(bool = false);
    void Wifi_status(int8_t);
    void Slot_Consent(bool);

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

    uint8_t CSDProtocol_Validation(QJsonArray *);
    uint8_t PulmonaryProtocol_Validation(QJsonArray *);
    QString Protocol_GetValue(QJsonArray *,QString);
    bool Protocol_FindString(QJsonArray *,QString,QString);
    bool PulmonaryProtocol_Checked(QJsonArray *,QString);
    bool validatePatienteAge();

    QString id_ci;

    QPushButton *start;
    QPushButton *updateProtocols;


    SeriesWidget *_seriesWidget;

    studyInfoWidget * studyInfo;
    studyProtocols * _studyDesc;
    studyProtocolsForm *_studyProtocolsFrom;
    dialogPatient * patientWidget;
    QWidget * _clinicdata;
    clinicDataWidget * _clinicdatawidget;
    QVkLineEdit * _reason;

    StudyFinished *pStudyFinished;

    operators ope;

    int studyId;
    int _patient_id;
    QString _patient_Document;
    int last_serie;
    bool started;


};

#endif // STUDY_H
