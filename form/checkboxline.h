//---------------------------------------------------------------------------------------------
//  Christiam

#ifndef CHECKBOXLINE_H
#define CHECKBOXLINE_H

#include <QWidget>
#include "qvklineedit.h"

class checkboxLine : public QWidget
{
    Q_OBJECT
public:
    explicit checkboxLine(QStringList,QWidget *parent = nullptr);

public:
    QCheckBox   *myChk;
    QVkLineEdit *myLine;

private slots:
    void enableLineEdit(int);
};

#endif // CHECKBOXLINE_H
//---------------------------------------------------------------------------------------------
