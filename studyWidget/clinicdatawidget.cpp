#include "clinicdatawidget.h"

clinicDataWidget::clinicDataWidget(QWidget *parent) : QWidget(parent)
{
    //Reason
    QLabel * reasonIcon = new QLabel(tr("Razón: "));
    _reason = new QVkLineEdit;
    _reason->setFixedHeight(50);
    urgente = new QCheckBox(tr("¿Urgente?"));
    urgente->setFixedWidth(130);


    QWidget * reasonWidget = new QWidget;
    reasonWidget->setFixedHeight(50);
    reasonWidget->setObjectName("reasonWidget");
    reasonWidget->setContentsMargins(1,1,1,1);
    QHBoxLayout *reasonLayout = new QHBoxLayout(reasonWidget);
    reasonLayout->addWidget(reasonIcon,0);
    reasonLayout->addWidget(_reason,10);
    reasonLayout->addSpacing(10);
    reasonLayout->addWidget(urgente,0,Qt::AlignCenter);
    reasonLayout->setSpacing(0);
    reasonLayout->setMargin(0);

    protocolname = new titlelabel();
    dataWidget = new QWidget();
    dataWidget->setObjectName("_clinicdata");
    dataLayout = new QVBoxLayout(dataWidget);
    dataLayout->setAlignment(Qt::AlignTop);
    dataLayout->setSpacing(15);
    dataLayout->setMargin(0);

    area = new QScrollArea();
    area->setWidget(dataWidget);
    area->setWidgetResizable(true);
    area->setObjectName("areaTransBg");
    area->viewport()->setAttribute(Qt::WA_AcceptTouchEvents);
    QScroller::grabGesture(area->viewport(),QScroller::LeftMouseButtonGesture);

    QVBoxLayout * h = new QVBoxLayout(this);
    h->addWidget(reasonWidget);
    h->addSpacing(15);
    h->addWidget(protocolname,0,Qt::AlignCenter);
    h->addWidget(area);
    h->addSpacing(15);
    h->setMargin(0);
}

void clinicDataWidget::reset(){
    _reason->setText("");
    urgente->setChecked(false);
}


void clinicDataWidget::setProtocols(int id){
    protocol.loadData(id);
    protocolname->setText(protocol.getValue("name").toString());
    createDataForm();
}

void clinicDataWidget::createDataForm(){
    reset();

    foreach(clinicInput *w, datas){
        dataLayout->removeWidget(w);
        delete w;
    }
    datas.clear();
    QByteArray json = protocol.getJson().toStdString().c_str();

    QJsonDocument doc = QJsonDocument::fromJson(json);
    QJsonArray listeInputs = doc.array();
    foreach(QJsonValue js, listeInputs){
        QJsonObject obj = js.toObject();
        clinicInput *dt = new clinicInput(obj);
        dataLayout->addWidget(dt);
        datas.append(dt);
    }
}

QByteArray clinicDataWidget::getJson(){
    QJsonArray jsonArray;
    foreach(clinicInput *cl, datas)
        jsonArray.append( cl->getJsonObject());
    QJsonDocument json(jsonArray);
    return json.toJson();
}


QString clinicDataWidget::getReason(){
    return _reason->text();
}

bool clinicDataWidget::getUrgent(){
    return urgente->isChecked();
}