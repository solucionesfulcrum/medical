#ifndef DIALOGSITE_H
#define DIALOGSITE_H

#include <QtWidgets>
#include <form/qvklineedit.h>
#include <form/touchcombobox.h>
#include <entitites/operators.h>
#include <entitites/sites.h>
#include <widgets/slidingstackedwidget.h>
#include <dialog/dialogoperator.h>


class userSiteBox : public QWidget
{
Q_OBJECT
public:
    userSiteBox(int,QString, QString,int, QWidget* = 0);
    ~userSiteBox();
signals:
    void eliminate(int);
    void select(int);
    void enable(int,int);      // ID and checkbox state
private slots:
    void toSelect();
    void toEnable(int);
    void toEliminate();

private:
    QString userID, name;
    int _id;



};



class dialogSite: public QWidget
{
    Q_OBJECT
public:
    dialogSite(QWidget *parent = 0);
    ~dialogSite();
signals:
    void selected(int );
public slots:
    void addLocation(void);

    void goFormAddLocation();
    void goFormLocation();
    void saveLocation();
    void enableLocation(int,int);
    void eliminateLocation(int);

    void refresh(QString);

    void selectUser(int);

private:
    void setFormLocation(void);
    void setFormAddLocation(void);
    void loadFormLocation(int);

    SlidingStackedWidget * _widgets;
    QWidget *search,*add,*edit;

    QList<userSiteBox *> results;
    QVBoxLayout * resultLayout;
    QPushButton *addButton;
    QScrollArea * area;
    QLabel *labelEQ = new QLabel(tr("No hay ninguna coincidencia"));

    sites p;

    bool toEdit;
    int IdToEdit;

    QVkLineEdit *addLocationName;
};


#endif // DIALOGOPERATOR_H
