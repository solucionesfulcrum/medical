#include "qmedicalboxwidget.h"



QMedicalBoxWidget::QMedicalBoxWidget(QWidget *parent) : QWidget(parent)
{
    mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setMargin(0);
}

QMedicalBoxWidget::~QMedicalBoxWidget()
{

}

void QMedicalBoxWidget::setQueueWidget(QueueWidget * q){
    _queuewidget = q;
}

void QMedicalBoxWidget::sendToQueue(int id){
    QHash<QString,QString> data;
    data.insert("toqueue","1");
    _series.update(data,id);
    _queuewidget->add(id);
}

