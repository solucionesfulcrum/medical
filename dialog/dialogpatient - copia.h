#ifndef DIALOGPATIENT_H
#define DIALOGPATIENT_H

#include <QtWidgets>
#include <form/qvklineedit.h>
#include <form/touchcombobox.h>
#include <entitites/patient.h>

class patientBox : public QWidget
{
Q_OBJECT
public:
    patientBox(int, QString, QString, QWidget* = 0);
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
signals:
    void selected(int );
public slots:
    void goForm();
    void goSearch();
    void refresh(QString);
    void editPatient(int);
    void selectPatient(int);
    void savePatient();
    void newPatient();
private:
    void setForm();
    void loadForm(int);
    void setSearch();

    QStackedWidget * _widgets;
    QWidget
    * search
    ,*add
    ,*edit
    ;
    //Remplacer label par widget mio
    QList<patientBox *> results;
     QVBoxLayout * resultLayout;
    QVkLineEdit *searchValue;
    QPushButton * addButton;
    QScrollArea * area;

    patient p;
    bool toEdit;
    int IdToEdit;

    QVkLineEdit
    * addPatientName,
    * addPatientLastName,
    * addPatientID;
    QDateEdit * addPatientBirthdate;
    TouchComboBox * addPatientSex, *addPatientSize;
};

#endif // DIALOGPATIENT_H
