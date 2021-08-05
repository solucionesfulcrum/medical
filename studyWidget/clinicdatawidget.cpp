#include "clinicdatawidget.h"

clinicDataWidget::clinicDataWidget(QWidget *parent) : QWidget(parent)
{
    //Reason
    QLabel * reasonIcon = new QLabel(tr("Motivo: "));
    _reason = new QButtonGroup();
    exam = new QRadioButton("Examen Rutinario / Seguimiento");
    urgent = new QRadioButton("¿Urgente?");
    trainning = new QRadioButton("¿Entrenamiento?");

    _reason->addButton(exam);
    _reason->addButton(urgent);
    _reason->addButton(trainning);
    connect(_reason,SIGNAL(buttonClicked(int)),this,SLOT(confirmReason(int)));

    QWidget * reasonWidget = new QWidget;
    reasonWidget->setObjectName("reasonWidget");
    reasonWidget->setContentsMargins(1,1,1,1);

    QGridLayout *reasonLayout = new QGridLayout(reasonWidget);
    reasonLayout->addWidget(reasonIcon,0,0,1,3);
    reasonLayout->addWidget(exam,1,0,1,2);
    reasonLayout->addWidget(urgent,1,2,1,1);
    reasonLayout->addWidget(trainning,2,0,1,2);

    protocolname = new titlelabel();
    //protocolname->setFixedWidth(600);//JB 20012020 Tamaño de ancho del titulo dentro de estudios.
    historytitle = new titlelabel(); //IF 13072021 Titulo history clínica.
    historytitle->setLineWidth(130); //IF 13072021 Ancho linea titulo history clínica.
    //historytitle->setFixedHeight(10);//IF 13072021 Alto titulo history clínica.
    //historytitle->setFixedWidth(600);//IF 13072021 Ancho titulo history clínica.
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
    //h->addSpacing(0);
    h->addWidget(protocolname,0,Qt::AlignCenter);
    h->addWidget(historytitle,0,Qt::AlignCenter);
    h->addWidget(area);
    h->addSpacing(15);
    h->setMargin(0);
}

void clinicDataWidget::reset(){
    QAbstractButton *b = _reason->checkedButton();
    if (b) {
        b->setAutoExclusive(false);
        b->setChecked(false);
        b->setAutoExclusive(true);
    }
}


void clinicDataWidget::setProtocols(int id){
    protocol.loadData(id);
    protocolname->setText("Protocolo "+protocol.getValue("name").toString());
    historytitle->setText("Historia Clínica");
    createDataForm();
}

void clinicDataWidget::createDataForm(){
    reset();

//-----------------------------------------------------
//  CR: Pensaria que hay que no es necesario esta parte de codigo
    foreach(clinicInput *w, datas){
        dataLayout->removeWidget(w);
        delete w;
    }
//-----------------------------------------------------
    datas.clear();  // Christiam
//-----------------------------------------------------

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
    QString reason = _reason->checkedButton()->text();
    reason.replace("¿","").replace("?","");
    return reason;
}

bool clinicDataWidget::getUrgent(){
    return urgent->isChecked();
}

bool clinicDataWidget::getTrainnning(){
    return trainning->isChecked();
}

void clinicDataWidget::confirmReason(int id){
    QMessageBox::information(this,tr("Motivo"),tr("¿Está seguro del motivo selecionado?"),QMessageBox::Yes);
}

