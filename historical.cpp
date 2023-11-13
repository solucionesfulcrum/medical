#include "historical.h"

historical::historical(QMedicalBoxWidget *parent) : QMedicalBoxWidget(parent)
{
    setSearchBox();
    listBox = new QWidget();
    listBox->setObjectName("historial");
    listBoxLayout = new QVBoxLayout(listBox);
    listBoxLayout->setSpacing(1);
    listBoxLayout->setMargin(0);
    listBoxLayout->setSizeConstraint(QLayout::SetNoConstraint);
    listBoxLayout->setAlignment( Qt::AlignTop );
    area = new QScrollArea();
    area->setObjectName("historialarea");
    area->viewport()->setAttribute(Qt::WA_AcceptTouchEvents);
    QScroller::grabGesture(area->viewport(),QScroller::LeftMouseButtonGesture);
    //area->setFixedSize(this->width(),300);
    area->setWidgetResizable(true);
    area->setWidget(listBox);
    area->viewport()->setAttribute(Qt::WA_AcceptTouchEvents);

    m_timeout = false;
    connect(&m_timer,SIGNAL(timeout()),this,SLOT(timeout()));

    mainLayout->setDirection(QBoxLayout::LeftToRight);
    mainLayout->addWidget(searchbox);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(area,3);
    mainLayout->setSpacing(0);
    mainLayout->setMargin(20);

}

historical::~historical()
{
}



void historical::setSearchBox(){

    searchbox = new QWidget();
    searchbox->setFixedWidth(300);
    searchbox->setObjectName("historialSearch");
    searchbox->setContentsMargins(1,1,1,1);
    QVBoxLayout * layout = new QVBoxLayout(searchbox);

    layout->setSpacing(0);
    layout->setMargin(0);
    layout->setAlignment(Qt::AlignTop);

    titlelabel * title = new titlelabel(tr("Historial"));
    title->setAlignment(Qt::AlignLeft);
    title->setLineWidth(65);


    name = new QVkLineEdit;
//------------------------------------------
//  CR: 19/05/23
    state = new TouchComboBox;
//------------------------------------------
    date = new TouchComboBox;
    protocols = new TouchComboBox;
    status = new TouchComboBox;
    from = new datebox;
    to = new datebox;
    from->setEnabled(false);
    to->setEnabled(false);

//-------------------------------------------------------------------------------------------------------
//  CR: 19/05/23
//  CR: 26/07/23
    stateItem << tr("Incompleto") << tr("En transmisión") << tr("Enviado") << tr("Asignado") << tr("Diagnosticado") << tr("Descargado") << tr("Desactivado") << tr("Adenda") << tr("Todos");

    state->setItems(stateItem);
    state->setText(stateItem.at(8));
//-------------------------------------------------------------------------------------------------------

    dateItem << tr("Hoy") << tr("3 días") << tr("7 días") << tr("30 días") << tr("Personalizado");
    date->setItems(dateItem);
    date->setText(dateItem.at(3));
    connect(date,SIGNAL(textChanged(QString)),this,SLOT(setDate(QString)));
    studydesc sd;

    QStringList prot;
    prot << "Todos";
    protocolsHash.insert(0,tr("Todos"));

    foreach(int i, sd.get()){
        sd.loadData(i);
        prot.append(sd.getValue("name").toString());
        protocolsHash.insert(i,sd.getValue("name").toString());
    }

    protocols->setItems(prot);
    protocols->setText(prot.at(0));
    searchbutton = new QPushButton(QIcon(":/icon/res/img/search_white.png"),tr("Buscar"));
    connect(searchbutton,SIGNAL(clicked()),this,SLOT(load()));
    searchbutton->setObjectName("greenButton");
    searchbutton->setIconSize(QSize(32,32));

    QLabel *patientLabel = new QLabel(tr("Paciente"));
    QLabel *stateLabel = new QLabel(tr("Estado"));
    QLabel *dateLabel = new QLabel(tr("Fecha predeterminada"));
    QLabel *protLabel = new QLabel(tr("Protocolo"));
    QLabel *dateLabelb = new QLabel(tr("Fecha (De - Hasta)"));

    layout->addWidget(title);
    layout->addSpacing(15);
    layout->addWidget(patientLabel);
    layout->addWidget(name);
//---------------------------------------------
//  CR: 15/05/23
    layout->addSpacing(15);
    layout->addWidget(stateLabel);
    layout->addWidget(state);
//---------------------------------------------
    layout->addSpacing(15);
    layout->addWidget(dateLabel);
    layout->addWidget(date);
    layout->addSpacing(15);
    layout->addWidget(dateLabelb);
    layout->addWidget(from);
    layout->addWidget(to);
    layout->addSpacing(15);
    layout->addWidget(protLabel);
    layout->addWidget(protocols);
    layout->addSpacing(15);
    layout->addWidget(searchbutton,1,Qt::AlignCenter);
    setDate(dateItem.at(3));

}


void historical::reset(){
    foreach(QHistWidget *b, studiesWidget){
        listBoxLayout->removeWidget(b);
        delete b;
    }
    studiesWidget.clear();
}

//-------------------------------------------------------
//  CR: 04/06/23
void historical::ResetOptions(void)
{
    state->setText(stateItem.at(8));
    date->setText(dateItem.at(3));
    protocols->setText("Todos");
    name->setText("");

}
//-------------------------------------------------------
void historical::timeout(void)
{
    m_timeout = true;
}

void historical::load(){

    searchbutton->setEnabled(false);
    m_timeout = false;
    m_timer.start(1000);
    do {
        QCoreApplication::processEvents();
    } while (m_timeout==false);



//  Christiam: Valido si es nombre,dni,apellido o nombre apellido o apellido nombre
    QString _cad1,_cad2,temp;
    int i;
    uint8_t palabras=0;

    temp = name->text();
    i = temp.indexOf(" ");
    if(i<0){
        _cad1 = temp;
    }
    else {
        palabras+=1;
        _cad1 = temp.left(i);
        _cad2 = temp.mid(i);
        _cad2.remove(" ");
    }

    int _protocols = protocolsHash.key(protocols->text());

    QDateTime fromDate(from->date());
    fromDate.setTime(QTime(0,0,0));

    QDateTime toDate(to->date());
    toDate.setTime(QTime(23,59,59));

    uint timestampTo = toDate.toTime_t();
    uint timestampFrom = fromDate.toTime_t();

    foreach(QHistWidget *b, studiesWidget){
        listBoxLayout->removeWidget(b);
        delete b;
    }
    studiesWidget.clear();

    QString query;

    if (palabras==0) {
        query += "SELECT s.id ";
        query += "FROM studies as s, patients as p , protocols as pl ";
        query += "WHERE s.starttime > "+QString::number(timestampFrom)+" ";
        query += "AND s.starttime < "+QString::number(timestampTo)+" ";
        query += "AND (";
        query += "p.name LIKE \"%"+_cad1+"%\" ";
        query += "OR p.last_name LIKE \"%"+_cad1+"%\"";
        query += "OR p.idp LIKE \"%"+_cad1+"%\"";
        query += ") ";
        if (_protocols != 0)
            query += "AND pl.id = "+QString::number(_protocols)+" ";
        query += "AND pl.id = s.id_protocols ";
        query += "AND p.id = s.id_patients ";

        if(state->text()=="Incompleto")
        {
            query += "AND s.state = -1 ";
        }
        else if(state->text()=="En transmisión")
        {
            query += "AND s.state = 0 ";
        }        
        else if(state->text()=="Enviado")
        {
            query += "AND s.state = 1 ";
        }
        else if(state->text()=="Asignado")
        {
            query += "AND s.state = 2 ";
        }
        else if(state->text()=="Diagnosticado")
        {
            query += "AND s.state = 3 ";
        }
        else if(state->text()=="Descargado")
        {
            query += "AND s.state = 4 ";
        }
        else if(state->text()=="Desactivado")
        {
            query += "AND s.state = 7 ";
        }
        else if(state->text()=="Adenda")
        {
            query += "AND s.state = 8 ";
        }

        query += "ORDER BY starttime desc";
    }
    else {
        query += "SELECT s.id ";
        query += "FROM studies as s, patients as p , protocols as pl ";
        query += "WHERE s.starttime > "+QString::number(timestampFrom)+" ";
        query += "AND s.starttime < "+QString::number(timestampTo)+" ";
        query += "AND (";
        query += "((p.name LIKE \"%"+_cad1+"%\" ";
        query += "AND p.last_name LIKE \"%"+_cad2+"%\")";

        query += "OR (p.name LIKE \"%"+_cad2+"%\"";
        query += "AND p.last_name LIKE \"%"+_cad1+"%\"))";
        //query += "OR p.idp LIKE \"%"+_cad1+"%\"";
        query += ") ";
        if (_protocols != 0)
            query += "AND pl.id = "+QString::number(_protocols)+" ";
        query += "AND pl.id = s.id_protocols ";
        query += "AND p.id = s.id_patients ";

        if(state->text()=="Incompleto")
        {
            query += "AND s.state = -1 ";
        }
        else if(state->text()=="En transmisión")
        {
            query += "AND s.state = 0 ";
        }
        else if(state->text()=="Enviado")
        {
            query += "AND s.state = 1 ";
        }
        else if(state->text()=="Asignado")
        {
            query += "AND s.state = 2 ";
        }
        else if(state->text()=="Diagnosticado")
        {
            query += "AND s.state = 3 ";
        }
        else if(state->text()=="Descargado")
        {
            query += "AND s.state = 4 ";
        }
        else if(state->text()=="Desactivado")
        {
            query += "AND s.state = 7 ";
        }
        else if(state->text()=="Adenda")
        {
            query += "AND s.state = 8 ";
        }


        query += "ORDER BY starttime desc";
    }

    QList<int> o = _studies.listeID(query);
    for(int i = 0; i<o.size(); i++){
        _studies.loadData(o.at(i));
        qDebug()<<"CR:"<<o.at(i);
        QHistWidget * t = new QHistWidget(o.at(i));
        connect(t,SIGNAL(loadStudyId(int)),this,SLOT(loadStudy(int)));
        connect(t,SIGNAL(deletedStudy(QHistWidget*)),this,SLOT(deletedStudy(QHistWidget*)));
        studiesWidget.append(t);
        listBoxLayout->addWidget(t);
    }

    searchbutton->setEnabled(true);
}

void historical::deletedStudy(QHistWidget * qhw){

    for (int x=0; x<studiesWidget.length(); x++){
        if(studiesWidget.at(x) == qhw){
            studiesWidget.removeAt(x);
        }
    }
    listBoxLayout->removeWidget(qhw);
    delete qhw;
    listBox->update();
    listBoxLayout->update();
}

void historical::loadStudy(int id){
    emit loadStudyId(id);
}

void historical::setDate(QString _date){
    from->setEnabled(false);
    to->setEnabled(false);
    QDate now = QDate::currentDate();
    QDate dt;

    bool changeDate = true;

    if(_date == dateItem.at(0))
        dt = now;
    if(_date == dateItem.at(1))
        dt = now.addDays(-3);
    if(_date == dateItem.at(2))
        dt = now.addDays(-7);
    if(_date == dateItem.at(3))
        dt = now.addMonths(-1);
    if(_date == dateItem.at(4)){
        from->setEnabled(true);
        to->setEnabled(true);
        changeDate = false;
    }
    if (changeDate){
        to->setDate(now);
        from->setDate(dt);
    }


}
