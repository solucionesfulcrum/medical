#ifndef DIALOGOPERATOR_H
#define DIALOGOPERATOR_H

#include <QtWidgets>
#include <form/qvklineedit.h>
#include <form/touchcombobox.h>
#include <entitites/operators.h>
#include <widgets/slidingstackedwidget.h>


class userBox : public QWidget
{
Q_OBJECT
public:    
    userBox(int, QString, QString,int,int,int,int, QWidget* = 0);
    ~userBox();
signals:
    void edit(int);
    void select(int);
    void enable(int,int);      // ID and checkbox state
    void enableAdmin(int,int);
private slots:
    void toEdit();
    void toSelect();
    void toEnable(int);
    void toEnableAdmin(int);

private:
    QString userID, name;
    int _id;

};

class dialogOperator : public QWidget
{
    Q_OBJECT
public:
    dialogOperator(QWidget *parent = 0);
    ~dialogOperator();
signals:
    void selected(int );
public slots:
    void goForm();
    void goSearch();
    void refresh(QString);
    void editUser(int);
    void selectUser(int);
    void saveUser();
    void newUser();
    void enableUser(int,int);
    void enableUserAdmin(int,int);
private:
    void setForm();
    void loadForm(int);
    void setSearch();

    SlidingStackedWidget * _widgets;
    QWidget
    * search
    ,*add
    ,*edit
    ;
    //Remplacer label par widget mio
    QList<userBox *> results;
     QVBoxLayout * resultLayout;
    QVkLineEdit *searchValue;
    QPushButton *addButton;
    QScrollArea * area;
    QLabel *labelEQ = new QLabel(tr("No hay ninguna coincidencia"));

    operators p;
    bool toEdit;
    int IdToEdit;

    QVkLineEdit
    * addUserName,
    * addUserPass;
    QCheckBox * isAdmin;
};


#endif // DIALOGOPERATOR_H
