#include "study.h"


study::study( QMedicalBoxWidget *parent) : QMedicalBoxWidget(parent)
{
    _seriesWidget = new SeriesWidget;
    _seriesWidget->hide();

    pStudyFinished = new StudyFinished(this);

    wifi_status = 0;

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
    patientWidget->setObjectName("PacienteWidget");
    connect(patientWidget,SIGNAL(selected(int)),this,SLOT(patientLoaded(int)));
}

void study::setClinicsData(){
    _clinicdata = new QWidget;
    //_clinicdata->setObjectName("form");
    _clinicdatawidget = new clinicDataWidget;

    start = new QPushButton(QIcon(":/icon/res/img/player_play.png"),tr("Iniciar"));
    connect(start,SIGNAL(clicked()),this,SLOT(startStudy()));
    start->setObjectName("redButton");
    start->setFixedSize(220,70);
    start->setStyleSheet({"font-size: 22px; font-weight: bold;"}); //JB-06082020

//  Buttons
    QPushButton * changeprotocolsbutton = new QPushButton(QIcon(":/icon/res/img/arrowleft.png"),tr("Cambiar")+"\n"+tr("de protocolo"));
    connect(changeprotocolsbutton,SIGNAL(clicked()),this,SLOT(changeProtocol()));
    changeprotocolsbutton->setObjectName("greenButton");
    changeprotocolsbutton->setFixedSize(220,70);
    changeprotocolsbutton->setStyleSheet({"font-size: 18px; font-weight: bold;"});//JB-06082020

    QWidget * buttonWidget = new QWidget;
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
    changepatient->setFixedSize(240,60);

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

//      Obstetric protocol survey validation        
        if(_studyDesc->getValue()==1){
            if(_clinicdatawidget->getReason()==""){
                QMessageBox::information(this,tr("Protocolo Obstétrico"),tr("Falta completar el motivo del estudio."),QMessageBox::Ok);
                start->setEnabled(true);
                return;
            }

            if (Falta_trimestre()){
                QMessageBox::information(this,tr("Protocolo Obstétrico"),tr("Falta seleccionar trimestre."),QMessageBox::Ok);
                start->setEnabled(true);
                return;
            }
            if (Falta_FUR_o_FPP()){
                QMessageBox::information(this,tr("Protocolo Obstétrico"),tr("Fechas no válidas.\nVerifique las fechas FUR y FPP."),QMessageBox::Ok);
                start->setEnabled(true);
                return;
            }

        //---------------------------------------------------------------------------------------------
        //  Christiam
            uint8_t e = validateCardiacBeat();

            if(e==3){
                QMessageBox::information(this,tr("Protocolo Obstétrico"),tr("La frecuencia cardiaca colocada no es numero."),QMessageBox::Ok);
                start->setEnabled(true);
                return;
            }
            else if (e==1){
                QMessageBox::information(this,tr("Protocolo Obstétrico"),tr("La frecuencia cardiaca esta fuera de rango [1,300]."),QMessageBox::Ok);
                start->setEnabled(true);
                return;
            }

            //---------------------------------------------------------------------------------------------

            MuestraUltimoUltrasonido();
        }

//      Pulmonary protocol survey validation
        else if(_studyDesc->getValue()==3){
            QByteArray json = _clinicdatawidget->getJson().toStdString().c_str();
            QJsonDocument jdoc = QJsonDocument::fromJson(json);
            QJsonArray jsonList = jdoc.array();

            uint8_t ok;

            ok = PulmonaryProtocol_Validation(&jsonList);
            if(ok==0){
                start->setEnabled(true);
                return;
            }
        }

//      CSD protocol survey validation
        else if(_studyDesc->getValue()==4){
            QByteArray json = _clinicdatawidget->getJson().toStdString().c_str();
            QJsonDocument jdoc = QJsonDocument::fromJson(json);
            QJsonArray jsonList = jdoc.array();

            uint8_t ok;

            ok = CSDProtocol_Validation(&jsonList);
            if(ok==0){
                start->setEnabled(true);
                return;
            }
        }

        QDateTime now = QDateTime::currentDateTime();

//      Create Study
        QHash<QString,QString> data;
        QString doUID = studies::doUID();

        data.insert("uid",doUID);
        data.insert("starttime",QString::number(now.toMSecsSinceEpoch()));
        data.insert("finishtime",QString::number(now.toMSecsSinceEpoch()));
        data.insert("reason",_clinicdatawidget->getReason());
        data.insert("ConsentimientoInformado",id_ci);
        data.insert("study_state",study_created);

        if (_clinicdatawidget->getUrgent())
            data.insert("urgent","1");
        else
            data.insert("urgent","0");

        if (_clinicdatawidget->getTrainnning())
            data.insert("trainning","1");
        else
            data.insert("trainning","0");

        data.insert("data",QString(_clinicdatawidget->getJson()));
        data.insert("state","-1");        
        data.insert("id_protocols",QString::number(_studyDesc->getValue()));
        data.insert("id_patients",QString::number(_patient_id));
        data.insert("id_operators",QString::number(ope.lastOp()));        

        _studies.insert(data);
        studyInfo->setStudyInfoDateTime(now.toString("dd/MM/yyyy <br /> hh:mm:ss"));
        studyId = _studies.getLastId();

//      Create Series
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
    else QMessageBox::information(this,tr("Información requerida"),tr("Por favor, completar toda la información requerida."));
    start->setEnabled(true);
}
uint8_t study::CSDProtocol_Validation(QJsonArray *jarray){

    QString value;

//  Validate reason
    if(_clinicdatawidget->getReason()==""){
        QMessageBox::information(this,tr("Protocolo CSD"),tr("Falta completar el motivo del estudio."),QMessageBox::Ok);
        start->setEnabled(true);
        return 0;
    }

    // Validate Other Selected on Exam Indications
    value = Protocol_GetValue(jarray,"txtIndicacionesOtros");
    if( (value=="") && (Protocol_FindString(jarray,"exam_indicaction", "Otros")==true) ){
        QMessageBox::information(this,tr("Protocolo CSD"),tr("Se seleccionó Otros en Indicaciones de Examen se debe especificar el valor."),QMessageBox::Ok);
        return 0;
    }

    value = Protocol_GetValue(jarray,"txtCirugiaAbdominalAnteriorEspecificar");
    if( (value=="") && (Protocol_FindString(jarray,"medical_history", "Cirugías abdominales anteriores")==true) ){
        QMessageBox::information(this,tr("Protocolo CSD"),tr("Se seleccionó Cirugías abdominales anteriores se debe especificar el valor."),QMessageBox::Ok);
        return 0;
    }
    return 1;
}

uint8_t study::PulmonaryProtocol_Validation(QJsonArray *jarray){

    QString value;
    bool ok;
    int number;
    double decimal;

    //  Validate reason
    if(_clinicdatawidget->getReason() == ""){
        QMessageBox::information(this,tr("Protocolo Pulmonar"),tr("Falta completar el motivo del estudio."),QMessageBox::Ok);
        start->setEnabled(true);
        return 0;
    }

    // Validate date of last ultrasound
    QCheckBox* firstUltrasound = this->parent()->findChild<QCheckBox*>("uecb");
    QDate ultrasounddate = QDate::fromString(Protocol_GetValue(jarray,"ultrasounddate"),"dd/MM/yyyy");
    if (!firstUltrasound->isChecked() && ultrasounddate > QDate::currentDate().addDays(-1)) {
        QMessageBox::information(this,tr("Protocolo Pulmonar"),tr("Falta seleccionar la fecha de último ultrasonido."),QMessageBox::Ok);
        return 0;
    }

    // Validate country of infection
    value = Protocol_GetValue(jarray,"txtLPI");
    if (value == ""){
        QMessageBox::information(this,tr("Protocolo Pulmonar"),tr("Falta completar lugar probable de infección."),QMessageBox::Ok);
        return 0;
    }

    // Validate address
    value = Protocol_GetValue(jarray,"txtDomicilioActual");
    if (value == ""){
        QMessageBox::information(this,tr("Protocolo Pulmonar"),tr("Falta completar domicilio actual."),QMessageBox::Ok);
        return 0;
    }

    // Validate case of infection
    value = Protocol_GetValue(jarray,"selCaseStatus");
    if (value == ""){
        QMessageBox::information(this,tr("Protocolo Pulmonar"),tr("Falta seleccionar clasificación del caso."),QMessageBox::Ok);
        return 0;
    } else if (value == "Confirmado") {
        QString covidTest = Protocol_GetValue(jarray,"selCovidTest");
        if (covidTest == "") {
            QMessageBox::information(this,tr("Protocolo Pulmonar"),tr("Falta seleccionar prueba COVID."),QMessageBox::Ok);
            return 0;
        } else if (covidTest == "Si") { // Si tiene pueba COVID -> sub validación
            // Validate date of covid test
            QDate dateFechaToma = QDate::fromString(Protocol_GetValue(jarray,"dateFechaToma"),"dd/MM/yyyy");
            if(dateFechaToma > QDate::currentDate()) {
                QMessageBox::information(this,tr("Protocolo Pulmonar"),tr("Falta seleccionar fecha de toma de prueba COVID."),QMessageBox::Ok);
                return 0;
            }

            QString testType = Protocol_GetValue(jarray,"selCovidTestType");
            if (testType == "") {
                QMessageBox::information(this,tr("Protocolo Pulmonar"),tr("Falta seleccionar tipo de prueba COVID."),QMessageBox::Ok);
                return 0;
            } else if (testType == "Otros" && Protocol_GetValue(jarray,"txtCovidTestOther") == "") {
                QMessageBox::information(this,tr("Protocolo Pulmonar"),tr("Falta completar otro tipo de prueba COVID."),QMessageBox::Ok);
                return 0;
            }
        }
    }

    // Validate if patient present sympthoms
    value = Protocol_GetValue(jarray,"selSymptom");
    if(value == ""){
        QMessageBox::information(this,tr("Protocolo Pulmonar"),tr("Falta seleccionar si ha presentado síntomas."),QMessageBox::Ok);
        return 0;
    } else if (value == "Si") {
        if(PulmonaryProtocol_Checked(jarray,"cbSymptomKind") == false){
            QMessageBox::information(this,tr("Protocolo Pulmonar"),tr("Falta marcar qué síntomas presentó."),QMessageBox::Ok);
            return 0;
        } else if(Protocol_FindString(jarray,"cbSymptomKind", "Otro síntoma") == true && Protocol_GetValue(jarray,"txtSymtomOther") == "") {
            QMessageBox::information(this,tr("Protocolo Pulmonar"),tr("Falta completar otro síntoma."),QMessageBox::Ok);
            return 0;
        }

        // Validate date when symptoms start
        QDate dateSymptomsStart = QDate::fromString(Protocol_GetValue(jarray,"dateSymtomStart"),"dd/MM/yyyy");
        if(dateSymptomsStart > QDate::currentDate()) {
            QMessageBox::information(this,tr("Protocolo Pulmonar"),tr("Falta seleccionar fecha de inicio de síntomas."),QMessageBox::Ok);
            return 0;
        }
    }

    // Validate if patient has been hospitalized
    value = Protocol_GetValue(jarray,"selHospitalized");
    if(value == ""){
        QMessageBox::information(this,tr("Protocolo Pulmonar"),tr("Falta seleccionar si ha sido hospitalizado."),QMessageBox::Ok);
        return 0;
    } else if (value == "Si") {
        // Validate date of hospitalization
        QDate dateHospitalization = QDate::fromString(Protocol_GetValue(jarray,"dateHospitalization"),"dd/MM/yyyy");
        if(dateHospitalization > QDate::currentDate()) {
            QMessageBox::information(this,tr("Protocolo Pulmonar"),tr("Falta seleccionar fecha de hospitalización."),QMessageBox::Ok);
            return 0;
        }

        if(Protocol_GetValue(jarray,"selMechanicVentilation") == ""){
            QMessageBox::information(this,tr("Protocolo Pulmonar"),tr("Falta seleccionar si ha recibido ventilacion mecánica."),QMessageBox::Ok);
            return 0;
        }
    }

    /* Validete hospitalization date is after symptoms date
    if(DateIniSyptoms>DateIniHospitalization){
        QMessageBox::information(this,tr("Protocolo Pulmonar"),tr("Fecha de sintomas debe ser antes de fecha de hospitalización."),QMessageBox::Ok);
        return 0;
    }
    */



    // Validate cardiac frequency
    number = Protocol_GetValue(jarray,"txtFrecuenciaCardiaca").toInt(&ok,10);
    if (ok == false) {
        QMessageBox::information(this,tr("Protocolo Pulmonar"),tr("Frecuencia cardiaca no válida."),QMessageBox::Ok);
        return 0;
    } else if ((number<CardiacFreqMin)||(number>CardiacFreqMax)) {
        QMessageBox::information(this,tr("Protocolo Pulmonar"),tr("Frecuencia cardiaca debe estar en el rango entre 0 y 300."),QMessageBox::Ok);
        return 0;
    }

    // Validate respiratory frequency
    number = Protocol_GetValue(jarray,"txtFrecuenciaRespiratoria").toInt(&ok,10);
    if (ok == false) {
        QMessageBox::information(this,tr("Protocolo Pulmonar"),tr("Frecuencia respiratoria no válida."),QMessageBox::Ok);
        return 0;
    } else if ((number<RespiratoryFreqMin)||(number>RespiratoryFreqMax)) {
        QMessageBox::information(this,tr("Protocolo Pulmonar"),tr("Frecuencia respiratoria debe estar en el rango entre 0 y 40."),QMessageBox::Ok);
        return 0;
    }

    // Validate oxygen saturation
    number = Protocol_GetValue(jarray,"txtSaturacionOxigeno").toInt(&ok,10);
    if (ok == false) {
        QMessageBox::information(this,tr("Protocolo Pulmonar"),tr("Saturacion de oxigeno no válida."),QMessageBox::Ok);
        return 0;
    } else if ((number<OxySaturationMin)||(number>OxySaturationMax)) {
        QMessageBox::information(this,tr("Protocolo Pulmonar"),tr("Saturacion de oxigeno debe estar en el rango entre 0 y 100."),QMessageBox::Ok);
        return 0;
    }

    // Validate patient's temperature
    decimal = Protocol_GetValue(jarray,"txtTemperatura").toDouble(&ok);
    if (ok == false) {
        QMessageBox::information(this,tr("Protocolo Pulmonar"),tr("Temperatura del paciente no válida."),QMessageBox::Ok);
        return 0;
    } else if ((decimal<TemperaturaMin)||(decimal>TemperaturaMax)) {
        QMessageBox::information(this,tr("Protocolo Pulmonar"),tr("Temperatura del paciente debe estar en el rango entre 25 y 45."),QMessageBox::Ok);
        return 0;
    }

    // Validate comorbidity
    if (PulmonaryProtocol_Checked(jarray,"cbComorbidity") == false) {
        QMessageBox::information(this,tr("Protocolo Pulmonar"),tr("Falta marcar si presenta condiciones de comorbilidad."),QMessageBox::Ok);
        return 0;
    } else if(Protocol_FindString(jarray,"cbComorbidity", "Otra cond. de comorbilidad") == true && Protocol_GetValue(jarray,"txtComorbidityOther") == "") {
        QMessageBox::information(this,tr("Protocolo Pulmonar"),tr("Falta completar otra condiciones de comorbilidad."),QMessageBox::Ok);
        return 0;
    }

    // Validate general aspect
    if (PulmonaryProtocol_Checked(jarray,"cbGeneralAspect") == false) {
        QMessageBox::information(this,tr("Protocolo Pulmonar"),tr("Falta marcar el aspecto general del paciente,"),QMessageBox::Ok);
        return 0;
    }

    // Validate if is vaccinated
    value = Protocol_GetValue(jarray,"selVaccinated");
    if (value == "") {
        QMessageBox::information(this,tr("Protocolo Pulmonar"),tr("Falta seleccionar si se vacunó el paciente."),QMessageBox::Ok);
        return 0;
    } else if (value == "Si") {
        QString vaccineType = Protocol_GetValue(jarray,"selVaccineType");
        if (vaccineType == "") {
            QMessageBox::information(this,tr("Protocolo Pulmonar"),tr("Falta seleccionar el tipo de vacuna."),QMessageBox::Ok);
            return 0;
        } else if (vaccineType == "Otro" && Protocol_GetValue(jarray,"txtVaccineOther") == "") {
            QMessageBox::information(this,tr("Protocolo Pulmonar"),tr("Falta completar otra vacuna."),QMessageBox::Ok);
            return 0;
        }

        if (Protocol_GetValue(jarray,"selVaccineDose") == "") {
            QMessageBox::information(this,tr("Protocolo Pulmonar"),tr("Falta seleccionar la dosis de la vacuna."),QMessageBox::Ok);
            return 0;
        }

        // Validate date of hospitalization
        QDate dateLastDose = QDate::fromString(Protocol_GetValue(jarray,"dateLastDose"),"dd/MM/yyyy");
        if(dateLastDose > QDate::currentDate()) {
            QMessageBox::information(this,tr("Protocolo Pulmonar"),tr("Falta seleccionar fecha de última dosis."),QMessageBox::Ok);
            return 0;
        }
    }

    // Validate if have had COVID
    value = Protocol_GetValue(jarray,"selHadCovid");
    if (value == "") {
        QMessageBox::information(this,tr("Protocolo Pulmonar"),tr("Falta seleccionar si tuvo COVID."),QMessageBox::Ok);
        return 0;
    } else if (value == "Si") {
        QDate dateCovidStart = QDate::fromString(Protocol_GetValue(jarray,"dateCovidStart"),"dd/MM/yyyy");
        if(dateCovidStart > QDate::currentDate()) {
            QMessageBox::information(this,tr("Protocolo Pulmonar"),tr("Falta seleccionar fecha en que tuvo COVID."),QMessageBox::Ok);
            return 0;
        }
    }

    return 1;
}

bool study::PulmonaryProtocol_Checked(QJsonArray *jarray,QString strname){
    //QString value;

    foreach(QJsonValue jasonV,*jarray){
        QJsonObject obj = jasonV.toObject();
        if(obj["name"]==strname){
            QJsonArray j = obj["values"].toArray();
            if(j.count()==0) return false;
            return true;
        }
    }
     return false;
}

bool study::Protocol_FindString(QJsonArray *jarray, QString StrName, QString StrValue)
{
    QString value;

    foreach(QJsonValue jasonV,*jarray){
        QJsonObject obj = jasonV.toObject();
        if(obj["name"]==StrName){
            QJsonArray j = obj["values"].toArray();
            for(uint8_t i=0;i<j.count();i++){
                value = j.at(i).toString();
                if(value==StrValue) return true;
            }
        }
    }
     return false;

}




QString study::Protocol_GetValue(QJsonArray *jarray,QString str){

    QString value;

    foreach(QJsonValue jasonV,*jarray){
        QJsonObject obj = jasonV.toObject();
        if(obj["name"]==str){
            value = obj["values"].toArray().at(0).toString();
        }
    }
    return value.simplified();
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
//---------------------------------------------------
//  CR: 15/03/21
//  Modify study_state in DB
    QHash<QString,QString> data;
    data.insert("study_state",study_finished);        
    _studies.UpdateLastElement(data);

//  Send message to server
    started = false;
    newStudy(b);
//---------------------------------------------------
}

void study::newStudy(bool b){
    bool torestart = true;
    if (started){
        torestart = false;

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


//  Load data when select patient
void study::patientLoaded(int s){
    patient p;
    p.loadData(s);
    _patient_Document = p.getValue("idp").toString();
    studyInfo->setStudyInfoPatient(p.getValue("name").toString(),p.getValue("last_name").toString());
    _patient_id = s;
//----------------------------------------------------------------
//  CR:
    if(p.sex()=="Masculino")    _studyDesc->loadWithSex('M');
    else _studyDesc->loadWithSex('X');
//----------------------------------------------------------------
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

    QFile errfile("HTTPres.txt");
    if (errfile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
    {
        QDateTime datetime;
        QTextStream out(&errfile);
        out  <<  "\n"
             << "DateTime: " + datetime.currentDateTime().toString() +  "\n"
             << "WifiStatus: " + QString::number(wifi_status) + "\n";
        errfile.close();
    }

    if(wifi_status==-1){
    QMessageBox::warning(this,tr("Conexión a internet"),tr("NO HAY CONEXION A INTERNET. Revisar la conexión."));
    }

    if (!validatePatienteAge()) {
        studyForm->slideInPrev();
        return;
    }


    int r = QMessageBox::warning(this,tr("Consentimiento informado"),tr(
                                        "Su respuesta se tomará como una declaración jurada, <br />"
                                        "si fuera positiva implica, que culminó con el llenado <br />"
                                        "del consentimiento informado y que éste ha sido <br />"
                                        " firmado por el paciente tratado."),QMessageBox::Yes|QMessageBox::No);

    if(r==QMessageBox::No){
        QMessageBox::information(this,tr("Consentimiento informado"),tr("Para continuar debe completar la firma del consentimiento informado."),QMessageBox::Ok);
        studyForm->slideInPrev();
        return;
    }

    id_ci = _patient_Document+"_"+QString::number(_studies.getLastElement("id")+1);
    QMessageBox::information(this,tr("Consentimiento informado"),tr("Código: ")+ id_ci,QMessageBox::Ok);

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
    if (fecha==now.toString("dd/MM/yyyy")) return true;

    pos=datos.indexOf("FPP");
    if (pos<1) return false;
    pos=datos.indexOf("values",pos);
    if (pos<1) return false;
    pos=datos.indexOf("\"",pos+8);
    fecha=datos.mid(pos+1,10);
    if (fecha==now.toString("dd/MM/yyyy")) return true;

    return false;
}

//---------------------------------------------------------------------------------------------
//  Christiam
uint8_t study::validateCardiacBeat()
{
    QByteArray json = _clinicdatawidget->getJson().toStdString().c_str();
    QJsonDocument jdoc = QJsonDocument::fromJson(json);
    QJsonArray jsonList = jdoc.array();

    foreach(QJsonValue jasonV,jsonList){
        QJsonObject obj = jasonV.toObject();
        if(obj["type"]=="mix"){
            bool ok;
            int frequency;
            QJsonValue jvalue;
            jvalue = obj["values"].toArray().at(0);
            frequency = jvalue.toString().toInt(&ok,10);
            if(!ok) return 3;
            else {
                if(frequency==-32768)   return 2;
                if( (frequency<1) || (frequency>300) )  return 1;
                else    return 0;
            }
        }
    }
    return 4;
}
//---------------------------------------------------------------------------------------------

bool study::Falta_trimestre(){

    QString datos,trimestre;
    int pos;

    datos=_clinicdatawidget->getJson();
    qDebug()<<datos;

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
        QMessageBox::information(this, tr("Falta Información"), tr("No se encontraron los objetos de último ultrasonido"));
        start->setEnabled(true);
        return;
    }

    if(uecb->isChecked())
    {
        //QMessageBox::information(this, tr("Información de Ultrasonido"), tr("Se ha marcado como primer Ultrasonido")); //JB 24012020 se retiro mensaje porque no aportaba valor cambio Benjamin
        start->setEnabled(true);
        return;
    }
    else
    {
        QDate now = QDate();
        now = now.currentDate();
        QDate calDate = fuu->selectedDate();
        //QString msgString = "La fecha marcada para último ultrasonido es: " + calDate.toString();
        //QMessageBox::information(this, tr("Información de Ultrasonido"), tr(msgString.toStdString().c_str())); //JB 24012020 se retiro mensaje porque no aportaba valor cambio Benjamin
        start->setEnabled(true);
    }
}

void study::Wifi_status(int8_t m){
    wifi_status = m;
}

bool study::validatePatienteAge(){
    patient currentPatient;
    currentPatient.loadData(_patient_id);
    int age = currentPatient.age();
    bool result = true;

    switch(_studyDesc->getValue())
    {
        case 1:
            if (age < 16) {
                QMessageBox::warning(this,tr("Protocolo Obstétrico"),tr("No puede realizar el protocolo obstétrico en menores de 16 años."));
                result = false;
            }
            break;
        case 3:
            if (age < 11 || age > 89 ) {
                QMessageBox::warning(this,tr("Protocolo Pulmonar"),tr("No puede realizar el protocolo pulmonar en menores de 11 años ni mayores de 89."));
                result = false;
            }
            break;
        case 4:
            if (age < 18) {
                QMessageBox::warning(this,tr("Protocolo CSD"),tr("No puede realizar el protocolo CSD en menores de 18 años."));
                result = false;
            }
            break;
        default:
            result = true;
    }

    return result;
}
