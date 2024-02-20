#include "dialogpatient.h"


patientBox::patientBox(int id, QString uid, QString s, QWidget *widget) : QWidget(widget){

    QWidget * w = new QWidget();
    w->setObjectName("listBox");
    setContentsMargins(20,0,20,0);

    QPushButton * editButton = new QPushButton(QIcon(":/icon/res/img/form/edit.png"),"");
    editButton->setIconSize(QSize(50,50));
    editButton->setFixedSize(60,60);

    QPushButton * selectButton = new QPushButton(s);
    selectButton->setObjectName("selectButton");
    selectButton->setFixedHeight(80);

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
    _widgets = new SlidingStackedWidget(this);
    _widgets->addWidget(search);
    _widgets->addWidget(add);
    QVBoxLayout * _main = new QVBoxLayout(this);
    _main->addWidget(_widgets);
    _main->setMargin(0);
    _main->setSpacing(0);
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

    if(p.listeID(s).length()==0){
        resultLayout->addWidget(labelEQ);
        labelEQ->show();
        resultLayout->setAlignment( Qt::AlignHCenter|Qt::AlignTop );
        //labelEQ->setStyleSheet("QLabel { color : red; font-weight: bold; }");
        labelEQ->setStyleSheet("QLabel { padding : 30px 0; font-size:16pt;}");


    }
    else {
        resultLayout->removeWidget(labelEQ);
        labelEQ->hide();

        foreach(int i, p.listeID(s)){
            p.loadData(i);
            patientBox *b = new patientBox(i, p.getValue("id").toString(),p.getValue("name").toString()+" "+p.getValue("last_name").toString());
            connect(b,SIGNAL(select(int)),this,SLOT(selectPatient(int)));
            connect(b,SIGNAL(edit(int)),this,SLOT(editPatient(int)));
            resultLayout->addWidget(b);
            results.append(b);
        }

        resultLayout->setAlignment( Qt::AlignTop );
        resultLayout->setSpacing(0);
        resultLayout->setMargin(0);
    }

    //qDebug() << p.listeID(s).length();

}



void dialogPatient::setSearch(){
    search = new QWidget;
    searchValue = new QVkLineEdit(true);
    searchValue->setPlaceholderText(tr("Busca el paciente por nombre o DNI"));
    searchValue->setFixedHeight(50);
    connect(searchValue,SIGNAL(textChanged(QString)),this,SLOT(refresh(QString)));
    addButton = new QPushButton(QIcon(":/icon/res/img/form/adduser.png"),"");
    addButton->setObjectName("redButton");
    addButton->setIconSize(QSize(30,30));
    addButton->setFixedSize(50,50);
    connect(addButton,SIGNAL(clicked()),this,SLOT(newPatient()));

    QLabel * icon = new QLabel;
    icon->setPixmap(QPixmap(":/icon/res/img/search.png"));
    icon->setFixedSize(50,50);

    QWidget *_header = new QWidget;
    _header->setObjectName("patientSearchHeader");
    QHBoxLayout * hl = new QHBoxLayout(_header);
    hl->addSpacing(10);
    hl->addWidget(icon);
    hl->addWidget(searchValue,5);
    hl->addSpacing(20);
    hl->addWidget(addButton);
    hl->setMargin(0);
    hl->setSpacing(0);

    QVBoxLayout * sl = new QVBoxLayout(search);
    area = new QScrollArea;
    area->setWidgetResizable(true);
    area->viewport()->setAttribute(Qt::WA_AcceptTouchEvents);
    QScroller::grabGesture(area->viewport(),QScroller::LeftMouseButtonGesture);
    QWidget *resW = new QWidget;
    resW->setObjectName("transparent");
    area->setWidget(resW);
    area->setObjectName("areaFindPatient");
    resultLayout = new QVBoxLayout(resW);
    resultLayout->setSpacing(1);
    resultLayout->setSizeConstraint(QLayout::SetNoConstraint);

    sl->addWidget(_header);
    sl->addSpacing(30);
    sl->addWidget(area);
    sl->setMargin(0);
    sl->setSpacing(1);

}

void dialogPatient::setForm(){
    add = new QWidget;

    QVBoxLayout *vl = new QVBoxLayout(add);
    QWidget *formwid = new QWidget;
    formwid->setObjectName("patientForm");
    formwid->setFixedWidth(500);
    formwid->setContentsMargins(50,20,50,20);
    vl->addWidget(formwid,0,Qt::AlignCenter);

    QGridLayout * a = new QGridLayout(formwid);
    a->setSpacing(10);
    a->setMargin(0);
    a->setRowStretch(0,0);
    a->setRowStretch(1,0);
    a->setAlignment(Qt::AlignTop);
    addPatientName = new QVkLineEdit;
    addPatientLastName = new QVkLineEdit;
    addPatientID = new QVkLineEdit;
    addPatientBirthdate = new datebox;
    addPatientSex = new TouchComboBox;
    addPatientSize = new TouchComboBox;
    addPatientPhone = new QVkLineEdit;
    addPatientCellphone = new QVkLineEdit;
    addPatientEmail = new QVkLineEdit;
    addPatientHeight = new QVkLineEdit;
    addPatientWeight = new QVkLineEdit;

    QStringList s;
    //s << tr("Masculino") << tr("Femenino") << tr("Otro");// JB17022020
    s << tr("Masculino") << tr("Femenino");

    QStringList sz;
    sz << "S" << "M" << "L";

    QList<QLabel *>formLabel;
    formLabel << new QLabel(tr("DNI(*)"))
              << new QLabel(tr("Nombre(*)"))
              << new QLabel(tr("Apellidos(*)"))
              << new QLabel(tr("Fecha de nacimiento(*)"))
              << new QLabel(tr("Sexo(*)"))
              << new QLabel(tr("Tamaño(*)"))
              << new QLabel(tr("Talla(*) (cm)"))
              << new QLabel(tr("Peso(*) (Kg)"))
              << new QLabel(tr("Telefono"))
              << new QLabel(tr("Celular"))
              << new QLabel(tr("Correo electrónico"));

    addPatientID->setPlaceholderText(tr("DNI del paciente"));
    addPatientName->setPlaceholderText(tr("Nombre"));
    addPatientLastName->setPlaceholderText(tr("Apellidos"));
    addPatientSex->setPlaceholderText(tr("Sexo"));
    addPatientSize->setPlaceholderText(tr("Tamaño"));
    addPatientPhone->setPlaceholderText(tr("Telefono"));
    addPatientCellphone->setPlaceholderText(tr("Celular"));
    addPatientEmail->setPlaceholderText(tr("Correo electrónico"));
    addPatientHeight->setPlaceholderText(tr("Talla (cm)"));
    addPatientWeight->setPlaceholderText(tr("Peso (Kg)"));

    //################### Nueva Funcionalidad #######################
    checkId = new QPushButton(QIcon(":/icon/res/img/form/save.png"),tr("Revisar DNI"));

    connect(checkId, SIGNAL(clicked()), this, SLOT(handleId()));
    checkId->setIconSize(QSize(32, 32));
    checkId->setObjectName("greenButton");
    //###############################################################

    foreach(QLabel *l, formLabel)
//------------------------------------------
//  CR: 19/05/23
        l->setFixedWidth(250);
//------------------------------------------
    addPatientSex->setItems(s);
    addPatientSize->setItems(sz);

    QWidget * buttons = new QWidget();
    buttons->setFixedSize(300,80);
    buttons->setContentsMargins(0,30,0,0);

    QHBoxLayout *b = new QHBoxLayout(buttons);

    //###################### Nueva Funcionalidad #####################
//    QPushButton * save = new QPushButton(QIcon(":/icon/res/img/form/save.png"),tr("Guardar"));
    save = new QPushButton(QIcon(":/icon/res/img/form/save.png"),tr("Guardar"));
    //################################################################

    connect(save,SIGNAL(clicked()),this,SLOT(savePatient()));
    save->setIconSize(QSize(32,32));
    save->setObjectName("greenButton");
    QPushButton * cancel = new QPushButton(QIcon(":/icon/res/img/form/cancel.png"),tr("Anular"));
    connect(cancel,SIGNAL(clicked()),this,SLOT(goSearch()));
    cancel->setIconSize(QSize(32,32));
    cancel->setObjectName("redButton");
    b->setMargin(0);
    b->setSpacing(5);
    b->addWidget(save);
    b->addWidget(cancel);

    int line = 0;
    title = new titlelabel();
    a->addWidget(title,0,0,1,2,Qt::AlignCenter);
    a->setRowStretch(0,3);

    line++;
    a->addWidget(formLabel.at(0),line,0);
    line++;
    a->addWidget(addPatientID,line,0);

    //####################### Nueva Funcionalidad ########################
    a->addWidget(checkId, line, 1);
    //####################################################################

    line++;
    a->addWidget(formLabel.at(1),line,0);
    a->addWidget(formLabel.at(2),line,1);

    line++;
    a->addWidget(addPatientName,line,0);
    a->addWidget(addPatientLastName,line,1);

    line++;
    a->addWidget(formLabel.at(3),line,0);

    line++;
    a->addWidget(addPatientBirthdate,line,0,1,2);

    line++;
    a->addWidget(formLabel.at(4),line,0);
    a->addWidget(formLabel.at(5),line,1);

    line++;
    a->addWidget(addPatientSex,line,0);
    a->addWidget(addPatientSize,line,1);

    line++;
    a->addWidget(formLabel.at(6),line,0);
    a->addWidget(formLabel.at(7),line,1);

    line++;
    a->addWidget(addPatientHeight,line,0);
    a->addWidget(addPatientWeight,line,1);

    line++;
    a->addWidget(formLabel.at(8),line,0);
    a->addWidget(formLabel.at(9),line,1);

    line++;
    a->addWidget(addPatientPhone,line,0);
    a->addWidget(addPatientCellphone,line,1);

    line++;
    a->addWidget(formLabel.at(10),line,0);

    line++;
    a->addWidget(addPatientEmail,line,0,1,2);

    line++;
    a->addWidget(buttons,line,0,1,2,Qt::AlignCenter);
    a->setRowStretch(line,3);
}

void dialogPatient::loadForm(int id){
    p.loadData(id);
    title->setText(tr("Editar paciente"));
    addPatientName->setText(p.getValue("name").toString());
    addPatientLastName->setText(p.getValue("last_name").toString());
    addPatientID->setText(p.getValue("idp").toString());
    if(p.getValue("birthday").toString() != "")
        addPatientBirthdate->setDate(QDate::fromString(p.getValue("birthday").toString(),"yyyyMMdd"));
    addPatientSex->setText(p.getValue("sex").toString());
    addPatientSize->setText(p.getValue("size").toString());

    addPatientPhone->setText(p.getValue("phone").toString());
    addPatientCellphone->setText(p.getValue("cellphone").toString());
    addPatientEmail->setText(p.getValue("email").toString());
    addPatientHeight->setText(p.getValue("height").toString());
    addPatientWeight->setText(p.getValue("weight").toString());

    toEdit = true;
    IdToEdit = id;
}

void dialogPatient::newPatient(){
    formType = 1;

    //############### Nueva Funcionalidad ##################
    addPatientName->setEnabled(false);
    addPatientLastName->setEnabled(false);
    addPatientSex->setEnabled(false);
    addPatientSize->setEnabled(false);
    addPatientBirthdate->setEnabled(false);

    addPatientPhone->setEnabled(false);
    addPatientCellphone->setEnabled(false);
    addPatientEmail->setEnabled(false);
    addPatientHeight->setEnabled(false);
    addPatientWeight->setEnabled(false);


    checkId->setVisible(true);
    checkId->setEnabled(true);

    save->setEnabled(false);
    //######################################################

    goForm();
    title->setText(tr("Agregar paciente"));
    addPatientName->setText("");
    addPatientLastName->setText("");
    addPatientID->setText("");
    addPatientBirthdate->setDate(QDate::currentDate());
    addPatientSex->setText("");
    addPatientSize->setText("");

    addPatientPhone->setText("");
    addPatientCellphone->setText("");
    addPatientEmail->setText("");
    addPatientHeight->setText("");
    addPatientWeight->setText("");
    toEdit = false;
}

void dialogPatient::goForm(){
    _widgets->slideInIdx(1,SlidingStackedWidget::BOTTOM2TOP);
    QMessageBox::information(this,tr("Atención"),tr("Por favor ingrese los datos según se muestran en el DNI del paciente."),QMessageBox::Ok);
    //_widgets->setCurrentIndex(1);
}

void dialogPatient::goSearch(){
    _widgets->slideInIdx(0,SlidingStackedWidget::TOP2BOTTOM);
    //_widgets->setCurrentIndex(0);
}

void dialogPatient::editPatient(int a){
    formType = 2;
    //############### Nueva Funcionalidad ##################
    addPatientName->setEnabled(true);
    addPatientLastName->setEnabled(true);
    addPatientSex->setEnabled(true);
    addPatientSize->setEnabled(true);
    addPatientBirthdate->setEnabled(true);

    addPatientPhone->setEnabled(true);
    addPatientCellphone->setEnabled(true);
    addPatientEmail->setEnabled(true);
    addPatientHeight->setEnabled(true);
    addPatientWeight->setEnabled(true);

    checkId->setVisible(false);
    checkId->setEnabled(false);

    save->setEnabled(true);
    //######################################################

    goForm();
    loadForm(a);
}


void dialogPatient::selectPatient(int a){
    emit selected(a);
    //close();
}

void dialogPatient::savePatient(){
    //To save
    QString n = addPatientName->text().simplified();
    QString l = addPatientLastName->text().simplified();
    QString i = addPatientID->text();
    QString b = addPatientBirthdate->date().toString("yyyyMMdd");
    QString s = addPatientSex->text();
    QString sz = addPatientSize->text();

    QString phone = addPatientPhone->text();
    QString cellphone = addPatientCellphone->text();
    QString email = addPatientEmail->text();
    QString height = addPatientHeight->text();
    QString weight = addPatientWeight->text();


//------------------------------------------------------------------------
//  int idSize = i.size();    //Christiam
    if(i == "" || n == "" || l == ""   || b == "" || s == ""  || sz == "" || height == "" || weight == ""  ){
        QMessageBox::information(this,tr("Campos vacios"), tr("Falta ingresar datos del paciente"));
        return;
    }

/*  Christiam comento esto
    QRegularExpression regex("^[0-9a-zA-Z]+$");
    //AVISO (jobenas): se ha agregado verificacion para que el tamaño del DNI
    //sea exactamente de 8 digitos.
    if(!regex.match(i).hasMatch() || idSize != 8)
    {
        QMessageBox::information(this,tr("DNI incorecto"), tr("¡El DNI del paciente es incorrecto!"));
        return;
    }*/

//------------------------------------------------------------------------
//  Christiam
    if(i.length()!=8){
        QMessageBox::information(this,tr("DNI incorecto"), tr("Incorrecta cantidad de digitos"));
        return;
    }

    for(uint8_t x=0;x<8;x++){
        if( (i.at(x)<'0') || (i.at(x)>'9') ){
            QMessageBox::information(this,tr("DNI incorecto"), tr("El DNI tiene valores no numericos"));
            return;
        }
    }

    if(i.toInt() <= 0 || i.toInt() >= 99999999 ){
        QMessageBox::information(this,tr("DNI incorecto"), tr("El DNI del paciente es incorrecto"));
        return;
    }
//------------------------------------------------------------------------

    if(!toEdit && p.exist(i)){
        QMessageBox::information(this,tr("DNI existente"), tr("¡El DNI del paciente ya existe!"));
        return;
    }

    if(toEdit && p.existOther(i,p.getValue("id").toInt())){
        QMessageBox::information(this,tr("DNI existente"), tr("¡El DNI del paciente ya existe!"));
        return;
    }

    QRegExp rx("^[A-Za-zÑñ\\s]+$");//Valida alfabetico
    QRegExpValidator v(rx, 0);
    int pos = 0;
    if(v.validate(n,pos) == QValidator::Invalid){
        QMessageBox::information(this,tr("Nombre incorrecto"), tr("¡El nombre tiene caracteres especiales o números!"));
        return;
    }
    if(v.validate(l,pos) == QValidator::Invalid){
        QMessageBox::information(this,tr("Apellido incorrecto"), tr("¡El apellido tiene caracteres especiales o números!"));
        return;
    }

    if(b > QDate::currentDate().toString("yyyyMMdd")){
        QMessageBox::information(this,tr("Fecha de nacimiento incorecto"), tr("¡La fecha de nacimiento debe ser antes que hoy!"));
        return;
    }


    if(height.toInt() < 80 || height.toInt() > 220){
        QMessageBox::information(this,tr("Talla incorrecta"), tr("¡La talla ingresada no es valida!\nDebe ser un valor entero entre 80 cm y 220 cm."));
        return;
    }

    if(weight.toDouble() < 5 || weight.toDouble() > 200){
        QMessageBox::information(this,tr("Peso incorrecto"), tr("¡El peso ingresado no es valido!\nDebe ser un valor entre 5 kg y 200 kg."));
        return;
    }

    rx.setPattern("\\d{9}");
    v.setRegExp(rx);
    if(phone != "" && v.validate(phone,pos) == QValidator::Invalid){
        QMessageBox::information(this,tr("Telefono incorrecto"), tr("¡El telefono ingresado no es valido!\nDebe ser un valor numerico de 9 digitos"));
        return;
    }
    if(cellphone != "" && v.validate(cellphone,pos) == QValidator::Invalid){
        QMessageBox::information(this,tr("Celular incorrecto"), tr("¡El telefono celular ingresado no es valido!\nDebe ser un valor numerico de 9 digitos"));
        return;
    }

    QMessageBox::StandardButton res = QMessageBox::question(this,tr("Atención"), tr("¿Esta seguro que la información ingresada concuerda con el DNI del paciente?"),
                          QMessageBox::Save|QMessageBox::Cancel,QMessageBox::Save);

    if (res == QMessageBox::Cancel) return;

    QHash<QString,QString> d;
    d.insert("name",n);
    d.insert("last_name",l);
    d.insert("idp",i);
    d.insert("birthday",b);
    d.insert("sex",s);
    d.insert("size",sz);
    d.insert("phone",phone);
    d.insert("cellphone",cellphone);
    d.insert("email",email);
    d.insert("height",height);
    d.insert("weight",weight);
    if(toEdit){
        p.update(d,IdToEdit);
        refresh("");
    }
    else{
        if (p.insert(d)){
            searchValue->setText(i);
        }
        else {
            QMessageBox::information(this,tr("Error de creación"), tr("¡Hubo un problema para guardar el paciente, intenté nuevamente!"));
            return;
        }

    }

    goSearch();

}

void dialogPatient::handleId()
{
    int textSize = addPatientID->text().size();
    QString idText = addPatientID->text();
    QRegularExpression regex("^[0-9a-zA-Z]+$");
    if(!regex.match(idText).hasMatch() || textSize != 8)
    {
        QMessageBox::information(this,tr("DNI incorecto"), tr("¡El DNI del paciente es incorrecto!"));
        return;
    }
    qDebug() << "ID inserted is " + addPatientID->text();

    QList<int> res = p.listeID(idText);
    if(res.size() == 1)
    {
        qDebug() << "Patient exists in db";

        QMessageBox::information(this,tr("Paciente Existe"), tr("El paciente ya se encuentra registrado"));

        int id = res[0];

        p.loadData(id);
        title->setText(tr("Editar paciente"));
        addPatientName->setText(p.getValue("name").toString());
        addPatientLastName->setText(p.getValue("last_name").toString());
        addPatientID->setText(p.getValue("idp").toString());
        if(p.getValue("birthday").toString() != "")
            addPatientBirthdate->setDate(QDate::fromString(p.getValue("birthday").toString(),"yyyyMMdd"));
        addPatientSex->setText(p.getValue("sex").toString());
        addPatientSize->setText(p.getValue("size").toString());
        addPatientPhone->setText(p.getValue("phone").toString());
        addPatientCellphone->setText(p.getValue("cellphone").toString());
        addPatientEmail->setText(p.getValue("email").toString());
        addPatientHeight->setText(p.getValue("height").toString());
        addPatientWeight->setText(p.getValue("weight").toString());
//        toEdit = true;
//        IdToEdit = id;
    }
    else if(res.size() == 0)
    {
        qDebug() << "Patient does not exist in db";

        QDate now = QDate();
        now = now.currentDate();

        addPatientName->setEnabled(true);
        addPatientLastName->setEnabled(true);
        addPatientSex->setEnabled(true);
        addPatientSize->setEnabled(true);
        addPatientBirthdate->setEnabled(true);
        addPatientPhone->setEnabled(true);
        addPatientCellphone->setEnabled(true);
        addPatientEmail->setEnabled(true);
        addPatientHeight->setEnabled(true);
        addPatientWeight->setEnabled(true);

        addPatientName->setText("");
        addPatientLastName->setText("");
        addPatientBirthdate->setDate(now);
        addPatientPhone->setText("");
        addPatientCellphone->setText("");
        addPatientEmail->setText("");
        addPatientHeight->setText("");
        addPatientWeight->setText("");

        save->setEnabled(true);
    }
}


