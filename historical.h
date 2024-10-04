#ifndef HISTORICAL_H
#define HISTORICAL_H

#include <qhistwidget.h>
#include "qmedicalboxwidget.h"
#include <form/touchcombobox.h>
#include <form/qvklineedit.h>
#include <entitites/studydesc.h>
#include <entitites/studies.h>
#include <widgets/slidingstackedwidget.h>
#include <widgets/titlelabel.h>

class historical : public QMedicalBoxWidget
{
    Q_OBJECT
public:
    explicit historical(QMedicalBoxWidget *parent = 0);
    ~historical();

public slots:
    void load();
    void reset();
    void setDate(QString);

private slots:
    void loadStudy(int);
    void deletedStudy(QHistWidget*);
signals:
    void loadStudyId(int);

private:
    void setSearchBox();
    QScrollArea * area;
    QWidget *searchbox;
    QWidget *listBox;
    QVBoxLayout *listBoxLayout;
    TouchComboBox *date, * protocols, *status;
    QVkLineEdit * name;
    datebox * from, *to;
    QPushButton * searchbutton;
    studies _studies;
    QList<QHistWidget*> studiesWidget;
    QHash<int,QString> protocolsHash;
    QStringList dateItem;



};

#endif // HISTORICAL_H
