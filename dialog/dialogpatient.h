#ifndef DIALOGPATIENT_H
#define DIALOGPATIENT_H

#include <QtWidgets>
#include <form/qvklineedit.h>
#include <form/touchcombobox.h>
#include <form/datebox.h>
#include <entitites/patient.h>
#include <widgets/slidingstackedwidget.h>

#include <widgets/titlelabel.h>
class patientBox : public QWidget
{
    Q_OBJECT
public:
    patientBox(int, QString, QString, QWidget* = nullptr);
    ~patientBox();
signals:
    void edit(int);
    void select(int);
private slots:
    void toEdit();
    void toSelect();
private:
    QString patientID, name;
    int _id;

};

class dialogPatient : public QWidget
{
    Q_OBJECT
public:
    dialogPatient(QWidget* parent =0);
    ~dialogPatient();
    patient p;

signals:
    void selected(int );
protected:

public slots:
    void goForm();
    void goSearch();
    void refresh(QString);
    void editPatient(int);
    void selectPatient(int);
    void savePatient();
    void newPatient();
    void handleId();
private:
    QHideEvent * hideevent;
    void setForm();
    void loadForm(int);
    void setSearch();
    QGraphicsOpacityEffect *effect;
    SlidingStackedWidget * _widgets;
    QWidget
    * search
    ,*add
    ,*edit
    ;

    titlelabel *title;
    QList<patientBox *> results;
    QVBoxLayout * resultLayout;
    QVkLineEdit *searchValue;
    QPushButton * addButton;
    QScrollArea * area;
    QLabel *labelEQ = new QLabel(tr("No hay ninguna coincidencia"));

    //########## Nueva Funcionalidad ########
    int formType;

    QPushButton * checkId;
    QPushButton * save;
    //#######################################

    bool toEdit;
    int IdToEdit;

    QVkLineEdit
    * addPatientName,
    * addPatientLastName,
    * addPatientID,
    * addPatientPhone,
    * addPatientCellphone,
    * addPatientEmail,
    * addPatientHeight,
    * addPatientWeight;
    datebox * addPatientBirthdate;
    TouchComboBox
    * addPatientSex,
    * addPatientSize;
};

#endif // DIALOGPATIENT_H
