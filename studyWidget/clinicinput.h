#ifndef CLINICINPUT_H
#define CLINICINPUT_H

#include <QtWidgets>
#include <form/qvklineedit.h>
#include <form/datebox.h>
#include <form/checkboxes.h>
#include <form/touchcombobox.h>
#include <form/calform.h>
#include <form/checkboxline.h>
#include <dialog/dialogpatient.h>
#include <entitites/sites.h>


class clinicInput : public QWidget
{
    Q_OBJECT
public:
    explicit clinicInput(QJsonObject,QButtonGroup *g=nullptr, QRadioButton *w=nullptr,QWidget *parent = nullptr);
    QJsonObject getJsonObject();
    QDate calcFPP(QDate fur);
    void setItemDefaults();

signals:

private slots:
    void calendarSelectedTestNoStr();
    void caseStatusSelected(const QString &text);
    void covidTestSelected(const QString &text);
    void otherTypeSelected(const QString &text);
    void haveSymtomSelected(const QString &text);
    void hospitalizedSelected(const QString &text);
    void vaccinatedSelected(const QString &text);
    void otherVaccineTypeSelected(const QString &text);
    void hadCovidSelected(const QString &text);
    void specificChecked(int);
    void otherChecked(int);
    void otherSymtomChecked(int);
    void otherComorbidityChecked(int);
    void nonComorbidityChecked(int);
    void firstUltrasoundCheck();

    void selectButtonCampaign(bool);
    void buttonChange(int);


private :
    QStringList type;
    QWidget * std_input;
    QJsonObject obj;
    QStringList item;
    QStringList defaults;

    void toggleCheckBox(QObject *parent, const QString actionText, const QString selectedText, const QString selectToggle);
    void toggleInput(QObject *parent, const QString actionText, const QString selectedText, const QString selectToggle);
    void toggleCalendar(QObject *parent, const QString actionText, const QString selectedText, const QString selectToggle);
    void toggleSelect(QObject *parent, const QString actionText, const QString selectedText, const QString selectToggle);
};

#endif // CLINICINPUT_H
