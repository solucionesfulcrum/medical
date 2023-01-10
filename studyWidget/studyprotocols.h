#ifndef STUDYPROTOCOLS_H
#define STUDYPROTOCOLS_H

#include <QtWidgets>
#include <serieswidget.h>
#include <entitites/operators.h>


class studyProtocols : public QWidget
{
    Q_OBJECT
public:
    studyProtocols(QWidget *parent = 0);
    ~studyProtocols();
    QString text();
    int getValue();
signals:
    void selected();
public slots:
    void updateProtocols();
    void load();
    void loadWithSex(char sex, int age);
    void updated();
    void save(int s);
private slots:

private:
    QLabel * updateStatus;
    QDialog *dg;
    studydesc sd;
    int value;
    QList<int> items;
    QGridLayout *layout;
    studyDescUpdate *dsc;
    QList <TouchButton*> touchButtons;
};

#endif // STUDYPROTOCOLS_H
