#include "calform.h"

calform::calform(QWidget *parent) : QWidget(parent)
{
    QCalendarWidget* cal = new QCalendarWidget;
    cal->setMinimumDate(QDate(1900, 1, 1));
    cal->setMaximumDate(QDate(3000, 1, 1));
}


void calform::setEnabled(bool val)
{
    cal->setEnabled(val);
}
