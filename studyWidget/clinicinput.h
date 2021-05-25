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


class clinicInput : public QWidget
{
    Q_OBJECT
public:
    explicit clinicInput(QJsonObject, QWidget *parent = nullptr);
    QJsonObject getJsonObject();
    QDate calcFPP(QDate fur);
signals:

private slots:
    void calendarSelectedTestNoStr();
    void covidTestSelected(const QString &text);
    void otherTypeSelected(const QString &text);
    void firstUltrasoundCheck();

private :
    QStringList type;
    QWidget * std_input;
    QJsonObject obj;
    QStringList item;
    QStringList defaults;
};

#endif // CLINICINPUT_H
