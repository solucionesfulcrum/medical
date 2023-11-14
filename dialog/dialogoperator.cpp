#include "dialogoperator.h"


//userBox::userBox(int id, QString uid, QString s,int enable,int admin, int superadmin, QWidget *widget) : QWidget(widget){
userBox::userBox(int id, QString uid, QString s,int enable, int admin,int superadmin, int privilege, QWidget *widget) : QWidget(widget){
    QWidget * w = new QWidget();
    w->setObjectName("listBox");
    setContentsMargins(20,0,20,0);

    QPushButton * selectButton = new QPushButton(s);
    selectButton->setObjectName("selectButton");
    selectButton->setFixedHeight(80);

//------------------------------------------------------
//  CR: 10/01/23
    QCheckBox *enableCheckbox = new QCheckBox();
    enableCheckbox->setObjectName("enableCheckbox");
    enableCheckbox->setFixedHeight(80);
    if(enable==1)   enableCheckbox->setChecked(true);
    else            enableCheckbox->setChecked(false);

    QCheckBox *adminCheckbox = new QCheckBox();
    adminCheckbox->setObjectName("enableCheckbox");
    adminCheckbox->setFixedHeight(80);
    if(admin==1)    adminCheckbox->setChecked(true);
    else            adminCheckbox->setChecked(false);

//------------------------------------------------------

    QPushButton * editButton = new QPushButton(QIcon(":/icon/res/img/userEdit.png"),"");
    editButton->setIconSize(QSize(50,50));
    editButton->setFixedSize(60,60);

    connect(editButton,SIGNAL(clicked()),this,SLOT(toEdit()));    
    connect(selectButton,SIGNAL(clicked()),this,SLOT(toSelect()));


    QGridLayout *l = new QGridLayout(w);
    l->addWidget(selectButton   ,0,0,1,4,Qt::AlignLeft);    

    if (privilege==2){
        if(superadmin!=1){
            connect(adminCheckbox,SIGNAL(stateChanged(int)),this,SLOT(toEnableAdmin(int)));
            connect(enableCheckbox,SIGNAL(stateChanged(int)),this,SLOT(toEnable(int)));
            l->addWidget(adminCheckbox ,0,4,1,1,Qt::AlignRight);
            l->addWidget(enableCheckbox ,0,5,1,1,Qt::AlignRight);
        }

        l->addWidget(editButton     ,0,6,1,1,Qt::AlignRight);
    }

    else if(privilege==1){
        if(superadmin!=1 && admin!=1){
            connect(enableCheckbox,SIGNAL(stateChanged(int)),this,SLOT(toEnable(int)));
            l->addWidget(enableCheckbox ,0,5,1,1,Qt::AlignRight);
            l->addWidget(editButton     ,0,6,1,1,Qt::AlignRight);
        }

        else if(admin==1 && superadmin!=1){
            l->addWidget(editButton     ,0,6,1,1,Qt::AlignRight);
        }
    }

    l->setMargin(0);
    l->setSpacing(3);
    userID = uid;
    name = s;
    _id= id;

    QHBoxLayout * m = new QHBoxLayout(this);
    m->addWidget(w);
    m->setSpacing(0);
    m->setMargin(0);
}

userBox::~userBox(){
}

void userBox::toEdit(){
    emit edit(_id);
}
void userBox::toSelect(){
    emit select(_id);
}

//-----------------------------------
//CR: 10/01/23
void userBox::toEnable(int i){
    emit enable(_id,i);
}
//CR: 16/02/23
void userBox::toEnableAdmin(int i){
    emit enableAdmin(_id,i);
}
//-----------------------------------

dialogOperator::dialogOperator(QWidget * parent) : QWidget(parent)
{
    setSearch();
    setForm();
    _widgets = new SlidingStackedWidget(this);
    _widgets->addWidget(search);
    _widgets->addWidget(add);
    QVBoxLayout * _main     = new QVBoxLayout(this);

    _main->addWidget(_widgets);    
    _main->setMargin(0);
    _main->setSpacing(0);
    refresh("");
    toEdit = false;
}

dialogOperator::~dialogOperator()
{
}

void dialogOperator::refresh(QString s){

    int privilege=0;
    if(operators::isSuperAdmin())   privilege++;
    if(operators::isAdmin())        privilege++;

    foreach(userBox *l, results){
        resultLayout->removeWidget(l);
        delete l;
    }
    results.clear();

    if(p.listeID(s).length()==0){
        resultLayout->addWidget(labelEQ);
        labelEQ->show();
        resultLayout->setAlignment( Qt::AlignHCenter|Qt::AlignTop );        
        labelEQ->setStyleSheet("QLabel { padding : 30px 0; font-size:16pt;}");
    }
    else {
        resultLayout->removeWidget(labelEQ);
        labelEQ->hide();

        foreach(int i, p.listeID(s)){
            p.loadData(i);
        //--------------------------------------------------------------------------------
        //  CR: 10/01/23           
            userBox *b = new userBox(i, p.getValue("id").toString(),p.getValue("name").toString(),p.getValue("enable").toInt(),p.getValue("admin").toInt(),p.getValue("superadmin").toInt(),privilege);

        //--------------------------------------------------------------------------------
            //connect(b,SIGNAL(select(int)),this,SLOT(selectUser(int)));
            connect(b,SIGNAL(edit(int)),this,SLOT(editUser(int)));
        //--------------------------------------------------------------------------------
        //  CR: 10/01/23
            connect(b,SIGNAL(enable(int,int)),this,SLOT(enableUser(int,int)));
        //  CR: 16/02/23
            connect(b,SIGNAL(enableAdmin(int,int)),this,SLOT(enableUserAdmin(int,int)));
        //--------------------------------------------------------------------------------
            resultLayout->addWidget(b);
            results.append(b);
        }
        resultLayout->setAlignment( Qt::AlignTop );
        resultLayout->setSpacing(3);
        resultLayout->setMargin(0);
    }
}

void dialogOperator::setSearch(){
    search = new QWidget;
    searchValue = new QVkLineEdit(true);

//----------------------------------------------------------------------------
//  CR: 09/01/23
    searchValue->setPlaceholderText(tr("Busca el operador por nombre"));
//----------------------------------------------------------------------------
    connect(searchValue,SIGNAL(textChanged(QString)),this,SLOT(refresh(QString)));

    addButton = new QPushButton(QIcon(":/icon/res/img/useradd.png"),"");
    addButton->setObjectName("greenButton");
    addButton->setIconSize(QSize(30,30));
    addButton->setFixedSize(50,50);
    connect(addButton,SIGNAL(clicked()),this,SLOT(newUser()));

    QWidget *_header = new QWidget;
    _header->setObjectName("patientSearchHeader");
    QHBoxLayout * hl = new QHBoxLayout(_header);
    hl->addWidget(searchValue,5);
    hl->addWidget(addButton);
    hl->setMargin(0);
    hl->setSpacing(3);

    QWidget *labels             = new QWidget();
    QGridLayout *layout_titles  = new QGridLayout(this);
    QLabel *label_user          = new QLabel(tr("Usuario"));
    QLabel *label_privileges    = new QLabel(tr("Admin"));
    QLabel *label_enable        = new QLabel(tr("Habilitar\n usuario"));
    QLabel *label_edituser      = new QLabel(tr("Editar\n usuario"));

    layout_titles->addWidget(label_user,0,0,1,4);   
    layout_titles->addWidget(label_privileges,0,4,1,1,Qt::AlignRight);
    layout_titles->addWidget(label_enable,0,5,1,1,Qt::AlignRight);
    layout_titles->addWidget(label_edituser,0,6,1,1,Qt::AlignRight);
    labels->setLayout(layout_titles);


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
    resultLayout->setSpacing(3);
    resultLayout->setSizeConstraint(QLayout::SetNoConstraint);
    sl->addWidget(_header);
    sl->addSpacing(20);

    sl->addWidget(labels);
    sl->addWidget(area);
    sl->setMargin(20);
    sl->setSpacing(3);

}

void dialogOperator::setForm(){
    add = new QWidget;
    add->setObjectName("patientForm");
    add->setContentsMargins(20,20,20,20);

    QFormLayout * a = new QFormLayout(add);
    addUserName = new QVkLineEdit;
    addUserPass = new QVkLineEdit;
    isAdmin = new QCheckBox;    
    addUserPass->setEchoMode(QLineEdit::Password);

    QList<QLabel *>formLabel;
    formLabel << new QLabel(tr("Nombre"))
              << new QLabel(tr("Contraseña"))
              << new QLabel(tr("¿Admin?"));
    foreach(QLabel *l, formLabel)
        l->setFixedWidth(220);
    a->addRow(formLabel.at(0),addUserName);
    a->addRow(formLabel.at(1),addUserPass);
    //a->addRow(formLabel.at(2),isAdmin);

    QWidget * buttons = new QWidget();
    QHBoxLayout *b = new QHBoxLayout(buttons);
    QPushButton * save = new QPushButton(QIcon(":/icon/res/img/save.png"),tr("Guardar"));
    connect(save,SIGNAL(clicked()),this,SLOT(saveUser()));
    save->setIconSize(QSize(32,32));
    save->setObjectName("blueButton");
//  CR:
    QPushButton * cancel = new QPushButton(QIcon(":/icon/res/img/cancel.png"),tr("Cancelar"));
    connect(cancel,SIGNAL(clicked()),this,SLOT(goSearch()));
    cancel->setIconSize(QSize(32,32));
    cancel->setObjectName("redButton");
    b->setMargin(0);
    b->setSpacing(1);
    b->addWidget(save);
    b->addWidget(cancel);

    a->addRow("",buttons);
}

void dialogOperator::loadForm(int id){
    p.loadData(id);
    addUserName->setText(p.getValue("name").toString());
    addUserPass->setText(p.getValue("pass").toString());
    if(p.getValue("admin").toInt() == 1)
        isAdmin->setChecked(true);
    else isAdmin->setChecked(false);
    toEdit = true;
    IdToEdit = id;
}

void dialogOperator::newUser(){
    goForm();
    addUserName->setText("");
    addUserPass->setText("");
    isAdmin->setChecked(false);
    toEdit = false;
}

void dialogOperator::goForm(){
    _widgets->slideInIdx(1,SlidingStackedWidget::BOTTOM2TOP);
}

void dialogOperator::goSearch(){
   _widgets->slideInIdx(0,SlidingStackedWidget::TOP2BOTTOM);
}

void dialogOperator::editUser(int a){
    goForm();
    loadForm(a);
}


void dialogOperator::selectUser(int a){
    emit selected(a);
    close();

}

void dialogOperator::saveUser(){
//  To save
    QString n = addUserName->text();
    QString l = addUserPass->text();

    if(n == "" && l == ""){
        QMessageBox::information(this,tr("Campos vacios"), tr("¡Por favor, ingrese todo los datos!"));
        return;
    }

    QHash<QString,QString> d;
    d.insert("name",n);
    d.insert("pass",l);
    if (isAdmin->isChecked())
    {
        d.insert("admin","1");
        d.insert("enable","1");
    }
    else
        d.insert("admin","0");

    if(toEdit)
        p.update(d,IdToEdit);
    else
        p.insert(d);
    refresh("");
    goSearch();

}
//---------------------------------------------------
//  CR: 11/01/23
void dialogOperator::enableUser(int id,int state)
{
    p.loadData(id);
    QHash<QString,QString> d;

    if(state)
        d.insert("enable","1");
    else
        d.insert("enable","0");

    p.update(d,id);
}

//  CR: 16/02/23
void dialogOperator::enableUserAdmin(int id,int state)
{
    p.loadData(id);
    QHash<QString,QString> d;

    if(state){
        d.insert("admin","1");
    }

    else{
        d.insert("admin","0");
    }

    p.update(d,id);
}

//---------------------------------------------------



