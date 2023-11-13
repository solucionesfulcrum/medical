#include "clinicdatawidget.h"

clinicDataWidget::clinicDataWidget(QWidget *parent) : QWidget(parent)
{
    //Reason
    QLabel * reasonIcon = new QLabel(tr("Motivo: "));
    QFont reasonFont = reasonIcon->font();
    reasonFont.setPointSize(10);

    _reason     = new QButtonGroup();
    exam        = new QRadioButton(tr("Examen Rutinario / Seguimiento"));
    urgent      = new QRadioButton(tr("¿Urgente?"));
    trainning   = new QRadioButton(tr("¿Entrenamiento?"));
    campaign    = new QRadioButton(tr("Campaña"));

    _reason->addButton(exam);
    _reason->addButton(urgent);
    _reason->addButton(trainning);
    _reason->addButton(campaign);

    _reason->setId(campaign,1);
    _reason->setId(urgent,2);
    _reason->setId(exam,3);
    _reason->setId(trainning,4);

    _reason->setExclusive(false);

    connect(_reason,SIGNAL(buttonClicked(int)),this,SLOT(confirmReason(int)));

    QWidget * reasonWidget = new QWidget;
    reasonWidget->setObjectName("reasonWidget");
    reasonWidget->setContentsMargins(1,1,1,1);

    QGridLayout *reasonLayout = new QGridLayout(reasonWidget);
    reasonLayout->addWidget(reasonIcon,0,0,1,3);
    reasonLayout->addWidget(exam,1,0,1,2);
    reasonLayout->addWidget(urgent,1,2,1,1);
    reasonLayout->addWidget(trainning,2,0,1,2);
    reasonLayout->addWidget(campaign,2,2,1,2);

    protocolname = new titlelabel();

    historytitle = new titlelabel();
    historytitle->setLineWidth(130);

    dataWidget = new QWidget();
    dataWidget->setObjectName("_clinicdata");
    dataLayout = new QVBoxLayout(dataWidget);
    dataLayout->setAlignment(Qt::AlignTop);
    dataLayout->setSpacing(15);
    dataLayout->setMargin(0);
    dataLayout->addWidget(reasonWidget);

    area = new QScrollArea();
    area->setWidget(dataWidget);
    area->setWidgetResizable(true);
    area->setObjectName("areaTransBg");
    area->viewport()->setAttribute(Qt::WA_AcceptTouchEvents);
    QScroller::grabGesture(area->viewport(),QScroller::LeftMouseButtonGesture);

    QVBoxLayout * h = new QVBoxLayout(this);
    //h->addWidget(reasonWidget);
    //h->addSpacing(0);
    h->addWidget(protocolname,0,Qt::AlignCenter);
    h->addWidget(historytitle,0,Qt::AlignCenter);
    h->addWidget(area);
    h->addSpacing(15);
    h->setMargin(0);
}

bool clinicDataWidget::getCampaign(void)
{
    return campaign->isChecked();
}


void clinicDataWidget::reset(){

    exam->setChecked(false);
    trainning->setChecked(false);
    urgent->setChecked(false);
    campaign->setChecked(false);



    /*
    QAbstractButton *b = _reason->checkedButton();
    if (b) {
        _reason->setExclusive(false);
        b->setChecked(false);
        _reason->setExclusive(true);
    }*/
}

void clinicDataWidget::setProtocols(int id){
    protocol.loadData(id);
    protocolname->setText(tr("Protocolo ")+protocol.getValue("name").toString());
    historytitle->setText(tr("Historia Clínica"));
    createDataForm();

}



void clinicDataWidget::createDataForm(){

    reset();

    foreach(clinicInput *w, datas)
    {
        dataLayout->removeWidget(w);
        delete w;
    }

    datas.clear();

    QByteArray json = protocol.getJson().toStdString().c_str();

    QJsonDocument doc = QJsonDocument::fromJson(json);
    QJsonArray listeInputs = doc.array();
    foreach(QJsonValue js, listeInputs){
        QJsonObject obj = js.toObject();
        clinicInput *dt = new clinicInput(obj,_reason,campaign);
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
    QString reason = "";
    int hasReason = _reason->checkedId();
    if (hasReason != -1) {
        reason = _reason->checkedButton()->text();
        reason.replace("¿","").replace("?","");
    }

    return reason;
}

bool clinicDataWidget::getUrgent(){
    return urgent->isChecked();
}

bool clinicDataWidget::getTrainnning(){
    return trainning->isChecked();
}

void clinicDataWidget::confirmReason(int buttonId){

    if(buttonId==4)
    {
        exam->setChecked(false);
        urgent->setChecked(false);
        campaign->setChecked(false);
    }
    else if(buttonId==3)
    {
        trainning->setChecked(false);
        urgent->setChecked(false);
        campaign->setChecked(false);
    }
    else if(buttonId==2)
    {
        exam->setChecked(false);
        trainning->setChecked(false);
    }
    else if(buttonId==1)
    {
        exam->setChecked(false);
        trainning->setChecked(false);
    }


    QMessageBox::information(this,tr("Motivo"),tr("¿Está seguro del motivo o motivos selecionados?"),QMessageBox::Yes);
}

