//---------------------------------------------------------------------------------------------
//  Christiam
#include "checkboxline.h"

checkboxLine::checkboxLine(QStringList items,QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *v = new QVBoxLayout(this);
    QHBoxLayout *h1 = new QHBoxLayout;
    QHBoxLayout *h2 = new QHBoxLayout;

//  CR: 04/01/23
//  Class is modified in order to always ask for cardiac frequency
//  QLabel *labelChk = new QLabel(items.at(0));
    QLabel *labelLine = new QLabel(items.at(1));

    v->setMargin(0);
    v->setSpacing(10);

    myChk = new QCheckBox;
    myLine = new QVkLineEdit;

//  CR: 04/01/23
    myLine->setEnabled(true);

//  CR: 04/01/23
//  h1->addWidget(myChk);
//  h1->addWidget(labelChk);

//  h1->setAlignment(Qt::AlignLeft);
//  h1->addSpacing(600);

    h2->addWidget(labelLine);
    h2->addWidget(myLine);
    //h2->addSpacing(600);

    v->addLayout(h1);
    v->addLayout(h2);
    setLayout(v);

    connect(myChk,SIGNAL(stateChanged(int)),this,SLOT(enableLineEdit(int)));
}

void checkboxLine::enableLineEdit(int)
{
    if(myChk->checkState()==Qt::Checked)
        myLine->setEnabled(true);
    else
        myLine->setEnabled(false);
 }
//---------------------------------------------------------------------------------------------
