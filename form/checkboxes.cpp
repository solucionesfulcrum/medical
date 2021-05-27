#include "checkboxes.h"

checkboxes::checkboxes(QStringList items, QStringList deflt, QWidget *parent) : QWidget(parent)
{
    QGridLayout * v = new QGridLayout(this);
    v->setMargin(0);
    v->setSpacing(10);
    int x = 0;
    foreach(QString l, items){
        QCheckBox * check = new QCheckBox(l);
        v->addWidget(check,x/2,x%2);
        if(deflt.contains(l))
            check->setChecked(true);
        checklist.append(check);
        x++;
    }
}

QString checkboxes::text(){
    QString res= "";
    foreach(QCheckBox * b, checklist){
        if(b->isChecked())
            res += b->text()+",";
    }
    return res;
}

QStringList checkboxes::getChecked(){
    QStringList res;
    foreach(QCheckBox * b, checklist){
        if(b->isChecked())
            res.append(b->text());
    }
    return res;
}

QCheckBox* checkboxes::getItem(int i) {
    return checklist.at(i);
}

radiobuttons::radiobuttons(QStringList items, QStringList deflt, QWidget *parent) : QWidget(parent)
{

    QGridLayout * v = new QGridLayout(this);
    v->setMargin(0);
    v->setSpacing(10);
    int x = 0;
    foreach(QString l, items){
        QRadioButton * check = new QRadioButton(l,this);
        v->addWidget(check,x/2,x%2);
        if(deflt.at(0) == l)
            check->setChecked(true);
        radioList.append(check);
        x++;
    }
}

QString radiobuttons::text(){
    QString res= "";
    foreach(QRadioButton * b, radioList){
        if(b->isChecked())
            res += b->text();
    }
    return res;
}



