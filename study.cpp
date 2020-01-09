#include "study.h"


study::study( QMedicalBoxWidget *parent) : QMedicalBoxWidget(parent)
{
    _seriesWidget = new SeriesWidget;
    _seriesWidget->hide();

    connect(_seriesWidget,SIGNAL(finished(bool)),this,SLOT(newStudy(bool)));
    connect(_seriesWidget,SIGNAL(finishedStudy(bool)),this,SLOT(isnewStudy(bool)));
    connect(_seriesWidget,SIGNAL(sendToQueue(int)),this,SLOT(sendToQueue(int)));

    QPushButton * newStudyButton = new QPushButton(tr("Nuevo Estudio"));
    connect(newStudyButton,SIGNAL(clicked()),this,SLOT(newStudy()));
    studyInfo = new studyInfoWidget();
    setForm();
    mainLayout->setDirection(QBoxLayout::LeftToRight);
    mainLayout->addWidget(studyForm);
    mainLayout->addWidget(_seriesWidget);
    mainLayout->addWidget(studyInfo);
    mainLayout->setMargin(0);
    started = false;
    newStudy();
}

study::~study()
{
}

void study::setForm(){
    setPatientForm();
    setStudyProtocolsForm();
    setClinicsData();
    studyForm = new SlidingStackedWidget(this);
    QMargins margin(30,30,30,30);
    patientWidget->setContentsMargins(margin);
    _studyProtocolsFrom->setContentsMargins(margin);
    _clinicdata->setContentsMargins(margin);
    studyForm->addWidget(patientWidget);
    studyForm->addWidget(_studyProtocolsFrom);
    studyForm->addWidget(_clinicdata);
}

void study::setPatientForm(){
    patientWidget = new dialogPatient;
    connect(patientWidget,SIGNAL(selected(int)),this,SLOT(patientLoaded(int)));
}

void study::setClinicsData(){
    _clinicdata = new QWidget;
    //_clinicdata->setObjectName("form");
    _clinicdatawidget = new clinicDataWidget;

    start = new QPushButton(QIcon(":/icon/res/img/player_play.png"),tr("Iniciar"));
    connect(start,SIGNAL(clicked()),this,SLOT(startStudy()));
    start->setObjectName("redButton");
    start->setFixedSize(200,60);
    //Buttons
    QPushButton * changeprotocolsbutton = new QPushButton(QIcon(":/icon/res/img/arrowleft.png"),tr("Cambiar")+"\n"+tr("de protocolo"));
    connect(changeprotocolsbutton,SIGNAL(clicked()),this,SLOT(changeProtocol()));
    changeprotocolsbutton->setObjectName("greenButton");
    changeprotocolsbutton->setFixedSize(200,60);

    QWidget * buttonWidget = new QWidget;
    // buttonWidget->setFixedHeight(50);
    QHBoxLayout *buttonsLayout = new QHBoxLayout(buttonWidget);
    buttonsLayout->addWidget(changeprotocolsbutton,1,Qt::AlignRight);
    buttonsLayout->addWidget(start,0,Qt::AlignRight);
    buttonsLayout->setSpacing(1);
    buttonsLayout->setMargin(0);

    QVBoxLayout * vl = new QVBoxLayout(_clinicdata);
    vl->addWidget(_clinicdatawidget);
    vl->addWidget(buttonWidget);

}

void study::setStudyProtocolsForm(){
    _studyProtocolsFrom = new studyProtocolsForm;
    _studyDesc = new studyProtocols();
    connect(_studyDesc,SIGNAL(selected()),this,SLOT(protocolSelected()));
    _studyProtocolsFrom->setObjectName("studyProtocolsFrom");


    //Header
    QWidget * headerWidget = new QWidget;
    headerWidget->setFixedHeight(50);
    headerWidget->setObjectName("reasonWidget");
    titlelabel * headerTitle = new titlelabel(tr("Protocolos"));
    headerTitle->setAlignment(Qt::AlignLeft);
    QPushButton * updateProtocols = new QPushButton(QIcon(":/icon/res/img/reload.png"),"");
    connect(updateProtocols,SIGNAL(clicked()),_studyDesc,SLOT(updateProtocols()));
    updateProtocols->setFixedSize(50,50);
    updateProtocols->setIconSize(QSize(35,35));
    updateProtocols->setObjectName("greenButton");
    QHBoxLayout *headerLayout = new QHBoxLayout(headerWidget);
    headerLayout->addWidget(headerTitle,8,Qt::AlignLeft);
    headerLayout->addSpacing(1);
    headerLayout->addWidget(updateProtocols,0);
    headerLayout->setSpacing(0);
    headerLayout->setMargin(0);



    //Buttons
    QPushButton * changepatient = new QPushButton(QIcon(":/icon/res/img/arrowleft.png"),tr("Cambiar de paciente"));
    connect(changepatient,SIGNAL(clicked()),this,SLOT(changePatient()));
    changepatient->setObjectName("greenButton");
    changepatient->setFixedSize(200,60);

    QWidget * buttonWidget = new QWidget;
    // buttonWidget->setFixedHeight(50);
    QHBoxLayout *buttonsLayout = new QHBoxLayout(buttonWidget);
    buttonsLayout->addWidget(changepatient);
    buttonsLayout->setSpacing(1);
    buttonsLayout->setMargin(0);

    //All
    QVBoxLayout * all = new QVBoxLayout(_studyProtocolsFrom);
    all->addSpacing(20);
    all->addWidget(headerWidget);
    all->addSpacing(10);
    all->addWidget(_studyDesc,10);
    all->addSpacing(20);
    all->addWidget(buttonWidget,0,Qt::AlignCenter);
    all->addSpacing(15);
    all->setSpacing(0);
    all->setMargin(0);

}

void study::startStudy(){
    start->setEnabled(false);
    if (_patient_id > 0){

        if (Falta_trimestre()){
            QMessageBox::information(this,tr("Protocolo Obstétrico"),tr("Falta seleccionar trimestre."),QMessageBox::Ok);
            start->setEnabled(true);
            return;
        }
        if (Falta_FUR_o_FPP()){
            //QMessageBox::information(this,QString::fromUtf8("Protocolo Obstétrico"),QString::fromUtf8("Fechas no válidas.\nVerifique las fechas FUR y FPP."),QMessageBox::Ok);
            QMessageBox::information(this,tr("Protocolo Obstétrico"),tr("Fechas no válidas.\nVerifique las fechas FUR y FPP."),QMessageBox::Ok);
            start->setEnabled(true);
            return;
        }


        MuestraUltimoUltrasonido();
        // Falta verificar si la fecha de ultimo ultrasonido ha sido
        // colocada correctamente (check desactivado y fecha diferente al dia de hoy)
//        if(FaltaUltimoUltrasonido())
//        {
//            QMessageBox::information(this, tr("Falta Información"), tr("Debe indicar la fecha de Último Ultrasonido"));
//            start->setEnabled(true);
//            return;
//        }
        // Validar fechas en base a ecuacion

        //findPatient->setEnabled(false);
        QDateTime now = QDateTime::currentDateTime();

        //Create Study
        QHash<QString,QString> data;
        //QDateTime dt(_studyDate->date(),_studyTime->time());
        data.insert("uid",studies::doUID());
        data.insert("starttime",QString::number(now.toMSecsSinceEpoch()));
        data.insert("finishtime",QString::number(now.toMSecsSinceEpoch()));
        data.insert("reason",_clinicdatawidget->getReason());
        if (_clinicdatawidget->getUrgent())
            data.insert("urgent","1");
        else
            data.insert("urgent","0");
        data.insert("data",QString(_clinicdatawidget->getJson()));
        data.insert("state","-1");
        data.insert("id_protocols",QString::number(_studyDesc->getValue()));
        data.insert("id_patients",QString::number(_patient_id));
        data.insert("id_operators",QString::number(ope.lastOp()));
        _studies.insert(data);
        studyInfo->setStudyInfoDateTime(now.toString("dd/MM/yyyy <br /> hh:mm:ss"));
        studyId = _studies.getLastId();

        //Create Series
        studydesc dsc;
        QList<int> sweeps = dsc.getSweepId(_studyDesc->getValue());
        foreach(int s, sweeps){
            QHash<QString,QString> data;
            data.insert("toqueue","0");
            data.insert("capture","0");
            data.insert("sent","0");
            data.insert("confirmed","0");
            data.insert("image_number","0");
            data.insert("id_sweeps",QString::number(s));
            data.insert("id_studies",QString::number(studyId));
            _series.insert(data);
        }
        studyInfo->setStudyInfoReason(_clinicdatawidget->getReason());

        studyForm->hide();
        _seriesWidget->setStudy(studyId);
        _seriesWidget->show();
        started = true;
        emit studyStarted(true);


    }
    else QMessageBox::information(this,tr("Información requerida"),tr("Por favor, complete toda la información requerida"));
    start->setEnabled(true);
}

void study::loadStudy(int id){
    start->setEnabled(false);
    studyId = id;

    studies _study;
    _study.loadData(id);

    patient _patient;
    _patient.loadData(_study.id_patient());

    studydesc _protocols;
    _protocols.loadData(_study.getValue("id_protocols").toInt());

    //Show information
    studyInfo->setStudyInfoPatient(_patient.getValue("name").toString(),_patient.getValue("last_name").toString());
    _patient_id = _study.id_patient();
    studyInfo->setStudyInfoProtocols(_protocols.getValue("name").toString());
    studyInfo->setStudyInfoDateTime(studies::datetimetoFormat(_study.datetime(),"dd/MM/yyyy <br /> hh:mm:ss"));
    studyInfo->setStudyInfoReason(_study.reason());
    studyForm->hide();
    _seriesWidget->setStudy(studyId);
    _seriesWidget->show();
    started = true;
    emit studyStarted(true);

}

void study::isnewStudy(bool b){
    started = false;
    newStudy(b);
}

void study::newStudy(bool b){
    bool torestart = true;
    if (started){
        torestart = false;
        qDebug()<<"Cancela estudio";
        if (QMessageBox::question(this,tr("¿Finalizar el estudio?"),tr("¿El estudio esta incompleto, esta seguro de finalizarlo?"),QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
            torestart = true;
    }
    if(torestart){
        studyId = -1;
        if(!b){
            _patient_id = -1;
            studyInfo->setStudyInfoPatient("","");
        }
        _clinicdatawidget->reset();
        _studyDesc->load();
        studyInfo->setStudyInfoProtocols("");
        studyInfo->setStudyInfoDateTime("");
        studyInfo->setStudyInfoReason("");
        studyForm->show();
        studyForm->setCurrentIndex(0);
        _seriesWidget->hide();
        started = false;
        if(b)
            studyForm->setCurrentIndex(1);
        emit studyStarted(false);
    }
}



void study::patientLoaded(int s){
    patient p;
    p.loadData(s);
    studyInfo->setStudyInfoPatient(p.getValue("name").toString(),p.getValue("last_name").toString());
    _patient_id = s;
    studyForm->slideInNext();
}

void study::changePatient(){
    studyForm->slideInPrev();
}

void study::changeProtocol(){
    _studyDesc->save(-1);
    studyInfo->setStudyInfoProtocols("");
    studyForm->slideInPrev();
}

bool study::isCapturing(){
    return _seriesWidget->isCapturingVideo();
}

void study::protocolSelected(){
    _clinicdatawidget->setProtocols(_studyDesc->getValue());
    studyInfo->setStudyInfoProtocols(_studyDesc->text());
    studyForm->slideInNext();
}



bool study::Falta_FUR_o_FPP(){
    QString datos,fecha;
    int pos;
    QDateTime now = QDateTime::currentDateTime();

    datos=_clinicdatawidget->getJson();

    pos=datos.indexOf("FUR");
    if (pos<1) return false;
    pos=datos.indexOf("values",pos);
    if (pos<1) return false;
    pos=datos.indexOf("\"",pos+8);
    fecha=datos.mid(pos+1,10);
    qDebug()<<"FUR: "<<fecha;
    if (fecha==now.toString("dd/MM/yyyy")) return true;

    pos=datos.indexOf("FPP");
    if (pos<1) return false;
    pos=datos.indexOf("values",pos);
    if (pos<1) return false;
    pos=datos.indexOf("\"",pos+8);
    fecha=datos.mid(pos+1,10);
    qDebug()<<"FPP: "<<fecha;
    if (fecha==now.toString("dd/MM/yyyy")) return true;

    qDebug()<<"Now: "<<now.toString("dd/MM/yyyy");
    return false;
}


bool study::Falta_trimestre(){


    QString datos,trimestre;
    int pos;

    datos=_clinicdatawidget->getJson();

    pos=datos.indexOf("trimestre");
    if (pos<1) return false;
    pos=datos.indexOf("values",pos);
    if (pos<1) return false;
    pos=datos.indexOf("\"",pos+8);
    trimestre=datos.mid(pos+1,1);
    qDebug()<<"trimestre: "<<trimestre;
    if (trimestre.toInt()<1) return true;

    return false;
}


//Falta Agregar el chequeo del ultimo ultrasonido por aca
//para poder resolver el problema de si la persona se olvido o no
void study::MuestraUltimoUltrasonido()
{
    qDebug() << "Attempting to retrieve calendar and checkbox";
    QObject* p = this->parent();
    QCheckBox* uecb = p->findChild<QCheckBox*>("uecb");
    QCalendarWidget* fuu = p->findChild<QCalendarWidget*>("FUU");
    if(!uecb || !fuu)
    {
        QMessageBox::information(this, tr("Falta Información"), tr("No se encontraron los objetos de ultimo ultrasonido"));
        start->setEnabled(true);
        return;
    }

    if(uecb->isChecked())
    {
        QMessageBox::information(this, tr("Información de Ultrasonido"), tr("Se ha marcado como primer Ultrasonido"));
        start->setEnabled(true);
        return;
    }
    else
    {
        QDate now = QDate();
        now = now.currentDate();
        QDate calDate = fuu->selectedDate();
        QString msgString = "La fecha marcada para último ultrasonido es: " + calDate.toString();
        QMessageBox::information(this, tr("Información de Ultrasonido"), tr(msgString.toStdString().c_str()));
        start->setEnabled(true);
    }
}
