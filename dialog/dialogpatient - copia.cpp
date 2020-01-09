#include "dialogpatient.h"


patientBox::patientBox(int id, QString uid, QString s, QWidget *widget) : QWidget(widget){

    QWidget * w = new QWidget();
    w->setObjectName("listBox");

    QPushButton * editButton = new QPushButton(QIcon(":/icon/res/img/userEdit.png"),"");

    QPushButton * selectButton = new QPushButton(s);
    selectButton->setObjectName("selectButton");
    connect(editButton,SIGNAL(clicked()),this,SLOT(toEdit()));
    connect(selectButton,SIGNAL(clicked()),this,SLOT(toSelect()));

    QHBoxLayout *l = new QHBoxLayout(w);
    l->addWidget(selectButton,8);
    l->addWidget(editButton);
    l->setMargin(0);
    l->setSpacing(1);
    patientID = uid;
    name = s;
    _id= id;

    QHBoxLayout * m = new QHBoxLayout(this);
    m->addWidget(w);
    m->setSpacing(0);
    m->setMargin(0);
}

patientBox::~patientBox(){

}

void patientBox::toEdit(){
    emit edit(_id);
}
void patientBox::toSelect(){
    emit select(_id);
}

dialogPatient::dialogPatient(QWidget * parent) : QWidget(parent)
{

    setSearch();
    setForm();
    setObjectName("dialog");

    _widgets = new QStackedWidget;
    _widgets->addWidget(search);
    _widgets->addWidget(add);
    _widgets->setContentsMargins(5,20,5,0);

    /*QLabel * title = new QLabel("Pacientes");
    title->setObjectName("title");
    QPushButton * closeButton = new QPushButton(QIcon(":/icon/res/img/close.png"),"");
    closeButton->setObjectName("redButton");
    closeButton->setFixedSize(50,50);
    connect(closeButton,SIGNAL(clicked()),this,SLOT(close()));


    QWidget *_header = new QWidget;
    QHBoxLayout * hl = new QHBoxLayout(_header);
    hl->addWidget(title,10);
    hl->addWidget(closeButton);
    hl->setMargin(0);
    hl->setSpacing(1);
    */

    QVBoxLayout * _main = new QVBoxLayout(this);
    //_main->addWidget(_header);
    _main->addWidget(_widgets);
    _main->setMargin(0);
    _main->setSpacing(1);

    refresh("");
    toEdit = false;
}

dialogPatient::~dialogPatient()
{
}

void dialogPatient::refresh(QString s){
    foreach(patientBox *l, results){
        resultLayout->removeWidget(l);
        delete l;
    }
    results.clear();
    foreach(int i, p.listeID(s)){
        p.loadData(i);
        patientBox *b = new patientBox(i, p.getValue("id").toString(),p.getValue("name").toString()+" "+p.getValue("last_name").toString());
        connect(b,SIGNAL(select(int)),this,SLOT(selectPatient(int)));
        connect(b,SIGNAL(edit(int)),this,SLOT(editPatient(int)));
        resultLayout->addWidget(b);
        results.append(b);
    }
    resultLayout->setAlignment( Qt::AlignTop );
    resultLayout->setSpacing(1);
    resultLayout->setMargin(0);
}

void dialogPatient::setSearch(){
    search = new QWidget;
    searchValue = new QVkLineEdit(true);
    searchValue->setPlaceholderText("Busca el paciente por nombre o ID");
    connect(searchValue,SIGNAL(textChanged(QString)),this,SLOT(refresh(QString)));
    addButton = new QPushButton(QIcon(":/icon/res/img/useradd.png"),"Nuevo");
    addButton->setObjectName("greenButton");
    connect(addButton,SIGNAL(clicked()),this,SLOT(newPatient()));

    QWidget *_header = new QWidget;
    QHBoxLayout * hl = new QHBoxLayout(_header);
    hl->addWidget(searchValue,5);
    hl->addWidget(addButton);
    hl->setMargin(0);
    hl->setSpacing(1);

    QVBoxLayout * sl = new QVBoxLayout(search);
    area = new QScrollArea;
    area->setFixedWidth(1024);

    area->setWidgetResizable(true);
    QWidget *resW = new QWidget;
    resW->setObjectName("transparent");
    area->setWidget(resW);
    resultLayout = new QVBoxLayout(resW);
    resultLayout->setSpacing(1);
    resultLayout->setSizeConstraint(QLayout::SetNoConstraint);
    sl->addWidget(_header);
    sl->addWidget(area);
    //sl->addWidget(searchValue->virtualKeyboard());
    sl->setMargin(0);
    sl->setSpacing(3);

}

void dialogPatient::setForm(){
    add = new QWidget;

    QFormLayout * a = new QFormLayout(add);
    addPatientName = new QVkLineEdit;
    addPatientLastName = new QVkLineEdit;
    addPatientID = new QVkLineEdit;
    addPatientBirthdate = new QDateEdit;
    addPatientBirthdate->setCalendarPopup(true);
    addPatientSex = new TouchComboBox;
    QStringList s;
    s << "Masculino" << "Femenino" << "Otro";
    addPatientSize = new TouchComboBox;
    QStringList sz;
    sz << "S" << "M" << "L";

    QList<QLabel *>formLabel;
    formLabel << new QLabel("ID")
              << new QLabel("Nombre")
              << new QLabel("Apellidos")
              << new QLabel("Fecha de nacimiento")
              << new QLabel("Sexo")
              << new QLabel("Tamaño");
    foreach(QLabel *l, formLabel)
        l->setFixedWidth(220);
    addPatientSex->setItems(s);
    addPatientSize->setItems(sz);
    a->addRow(formLabel.at(0),addPatientID);
    a->addRow(formLabel.at(1),addPatientName);
    a->addRow(formLabel.at(2),addPatientLastName);
    a->addRow(formLabel.at(3),addPatientBirthdate);
    a->addRow(formLabel.at(4),addPatientSex);
    a->addRow(formLabel.at(5),addPatientSize);

    QWidget * buttons = new QWidget();
    QHBoxLayout *b = new QHBoxLayout(buttons);
    QPushButton * save = new QPushButton("Guardar");
    connect(save,SIGNAL(clicked()),this,SLOT(savePatient()));
    QPushButton * cancel = new QPushButton("Anular");
    connect(cancel,SIGNAL(clicked()),this,SLOT(goSearch()));

    b->addWidget(save);
    b->addWidget(cancel);
    a->addRow("",buttons);
}

void dialogPatient::loadForm(int id){
    p.loadData(id);
    addPatientName->setText(p.getValue("name").toString());
    addPatientLastName->setText(p.getValue("last_name").toString());
    addPatientID->setText(p.getValue("idp").toString());
    addPatientBirthdate->setDate(QDate::fromString(p.getValue("birthday").toString(),"yyyyMMdd"));
    addPatientSex->setText(p.getValue("sex").toString());
    addPatientSize->setText(p.getValue("size").toString());
    toEdit = true;
    IdToEdit = id;
}

void dialogPatient::newPatient(){
    goForm();
    addPatientName->setText("");
    addPatientLastName->setText("");
    addPatientID->setText("");
    addPatientBirthdate->setDate(QDate::currentDate());
    addPatientSex->setText("");
    addPatientSize->setText("");
    toEdit = false;
}

void dialogPatient::goForm(){
    _widgets->setCurrentIndex(1);
}

void dialogPatient::goSearch(){
    _widgets->setCurrentIndex(0);
}

void dialogPatient::editPatient(int a){
    goForm();
    loadForm(a);
}


void dialogPatient::selectPatient(int a){
    emit selected(a);
    //close();
}

void dialogPatient::savePatient(){
    //To save
    QString n = addPatientName->text();
    QString l = addPatientLastName->text();
    QString i = addPatientID->text();
    QString b = addPatientBirthdate->date().toString("yyyyMMdd");
    QString s = addPatientSex->text();
    QString sz = addPatientSize->text();

    if(i == "" ){
        QMessageBox::information(this,"Campo vacio", "Por favor, informe todo los campos!");
        return;
    }

    if(p.exist(i)){
        QMessageBox::information(this,"ID existente", "El ID del paciente ya existe!");
        return;
    }

    QHash<QString,QString> d;
    d.insert("name",n);
    d.insert("last_name",l);
    d.insert("idp",i);
    d.insert("birthday",b);
    d.insert("sex",s);
    d.insert("size",sz);
    if(toEdit)
        p.update(d,IdToEdit);
    else
        p.insert(d);
    refresh("");
    goSearch();

}


