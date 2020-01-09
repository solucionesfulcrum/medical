#ifndef DATEBOX_H
#define DATEBOX_H

#include <form/touchcombobox.h>
#include <QtWidgets>

class datebox : public QWidget
{
    Q_OBJECT
public:
    datebox(QWidget * parent = 0);
    ~datebox();
    void setDate(QDate);
    QDate date();
private slots:
    void updateDay(QString);

private:
    QStringList monthname, dayList;
    TouchComboBox
    * day,
    * month,
    * year;
};

#endif // DATEBOX_H
