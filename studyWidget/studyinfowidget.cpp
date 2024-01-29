#include "studyinfowidget.h"

studyInfoWidget::studyInfoWidget(QWidget * parent) : QWidget(parent)
{
//----------------------------
//  CR: 01/02/20
//  setFixedWidth(380);
    setFixedWidth(310);
//----------------------------

    studyInfo = new QWidget();
    studyInfo->setObjectName("studyInfoWidget");
    QHBoxLayout * l = new QHBoxLayout(this);
    l->addWidget(studyInfo);

//----------------------------
//  CR: 01/02/20
//  l->setSpacing(0);
//  l->setMargin(0);
//----------------------------

    studyInfoPatientName = new QLabel(studyInfo);
    studyInfoPatientName->setFixedWidth(230);
    studyInfoPatientName->setFixedHeight(60);

    studyInfoProtocols = new QLabel(studyInfo);
    studyInfoProtocols->setFixedWidth(230);
    studyInfoProtocols->setFixedHeight(60);

    studyInfoDateTime = new QLabel(studyInfo);
    studyInfoDateTime->setAlignment(Qt::AlignCenter);
    studyInfoDateTime->setObjectName("studyInfoWidgetDatetime");
    studyInfoReason = new QTextEdit("",studyInfo);
    studyInfoReason->setEnabled(false);

    studyInfoReason->setFixedWidth(230);    
    studyInfoReason->setFixedHeight(120);

    QLabel * studyInfoPatientLabel = new QLabel(studyInfo);
    studyInfoPatientLabel->setAlignment(Qt::AlignCenter);

    studyInfoPatientLabel->setPixmap(QPixmap(":/icon/res/img/study_info/patient.png"));
    studyInfoPatientLabel->setObjectName("icons");
    //studyInfoPatientLabel->setFixedSize(80,80);
    studyInfoPatientLabel->setFixedSize(70,70);
//  CR: 01/02/21
    studyInfoPatientLabel->move(15,40);
    studyInfoPatientLabel->raise();

    studyInfoPatientName->move(80,50);


    QLabel * studyInfoReasonLabel = new QLabel(studyInfo);
    studyInfoReasonLabel->setAlignment(Qt::AlignCenter);
    studyInfoReasonLabel->setPixmap(QPixmap(":/icon/res/img/study_info/reason.png"));
    studyInfoReasonLabel->move(15,150);
    studyInfoReasonLabel->setObjectName("icons");
    //studyInfoReasonLabel->setFixedSize(80,80);
    studyInfoReasonLabel->setFixedSize(70,70);
    studyInfoReasonLabel->raise();

    studyInfoProtocols->move(80,160);

    QLabel * studyInfoProtocolsLabel = new QLabel(studyInfo);
    studyInfoProtocolsLabel->setAlignment(Qt::AlignCenter);
    studyInfoProtocolsLabel->setPixmap(QPixmap(":/icon/res/img/study_info/protocol.png"));
    studyInfoProtocolsLabel->move(15,250);
    studyInfoProtocolsLabel->setObjectName("icons");
    //studyInfoProtocolsLabel->setFixedSize(80,80);
    studyInfoProtocolsLabel->setFixedSize(70,70);
    studyInfoProtocolsLabel->raise();

    studyInfoReason->move(70,260);

    studyInfoDateTime->move(40,450);
    studyInfoDateTime->setFixedSize(300,120);

}

studyInfoWidget::~studyInfoWidget()
{

}

void studyInfoWidget::setStudyInfoPatient(QString n, QString l){
    studyInfoPatientName->setText(n+"<br />"+l);
    //studyInfoPatientLastName->setText(l);
}

void studyInfoWidget::setStudyInfoPatientName(QString s){
    studyInfoPatientName->setText(s);
}

void studyInfoWidget::setStudyInfoPatientLastName(QString s){
}

void studyInfoWidget::setStudyInfoProtocols(QString s){
    studyInfoProtocols->setText(s);
}

void studyInfoWidget::setStudyInfoReason(QString s){
    studyInfoReason->setText(s);
}

void studyInfoWidget::setStudyInfoDateTime(QString s){
    studyInfoDateTime->setText(s);
}
