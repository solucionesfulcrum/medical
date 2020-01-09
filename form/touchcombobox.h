#ifndef TOUCHCOMBOBOX_H
#define TOUCHCOMBOBOX_H

#include <include.h>
#include "accesor.h"


class TouchButton : public QLabel
{
    Q_OBJECT
public:
    TouchButton(const QString & text, QWidget * parent = 0, Qt::WindowFlags f = 0);

    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void setChecked(bool);
    void setId(int);
    int getId();
signals:
    void sendValue(QString);
    void sendId(int);

private:

    int id;
    QTime t;

};



class TouchComboBox : public QLineEdit
{
    Q_OBJECT
public:
    explicit TouchComboBox(QWidget *parent = 0);
    ~TouchComboBox();
    void mousePressEvent(QMouseEvent *);
    void setItems(QStringList);
    void createPopUpWidget();
signals:
    void clicked();

public slots:
    void save(QString);
    void cancel();

private:
    QPropertyAnimation * animation;
    void refresh();
    QStringList items;
    QString current;
    QDialog * pop;
    QList <TouchButton*> touchButtons;
    QScrollArea* area;


};

#endif // TOUCHCOMBOBOX_H
