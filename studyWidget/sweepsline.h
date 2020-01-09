#ifndef SWEEPSLINE_H
#define SWEEPSLINE_H

#include <QtWidgets>
#include <entitites/studies.h>
#include <entitites/series.h>
#include <entitites/studydesc.h>
#include <widgets/titlelabel.h>


class sweepItem : public QWidget
{
    Q_OBJECT
public:
    sweepItem(int id, QWidget * parent = 0);
    ~sweepItem();
    void setActual(bool);
    void setDone(bool);
    void isfirst();
    void islast();
    int idSerie();
    QString name();
    int idSweep();
protected:
    void paintEvent(QPaintEvent *);
private:

    bool first, last;
    int id_serie;
    series _series;
    bool actual;
    bool done;

};

class sweepsLine : public QWidget
{
    Q_OBJECT
public:
    sweepsLine(QWidget *parent = 0);
    ~sweepsLine();
    void setStudy(int id);
    int actual();
    int actualId();
    void setActual(int);
    int sweepsSize();
    bool isLast();
    void next();
private:
    void clear();
    QHBoxLayout * slinelayout;
    QList<sweepItem*> sweeps;
    titlelabel * title;
    series _series;
    studies _studies;
    int
    numberOfSeries
    ,_actualId
    ,_actual
    ,studyID;
};

#endif // SWEEPSLINE_H
