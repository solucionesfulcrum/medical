#include "dialogsite.h"

userSiteBox::userSiteBox(int id, QString uid, QString s,int enable, QWidget *widget) : QWidget(widget)
{
    QWidget * w = new QWidget();
    w->setObjectName("listBox");
    setContentsMargins(20,0,20,0);

    QPushButton * selectButton = new QPushButton(s);
    selectButton->setObjectName("selectButton");
    selectButton->setFixedHeight(80);

    QCheckBox *enableCheckbox = new QCheckBox();
    enableCheckbox->setObjectName("enableCheckbox");
    enableCheckbox->setFixedHeight(80);
    if(enable==1)   enableCheckbox->setChecked(true);
    else            enableCheckbox->setChecked(false);

    QPushButton * eliminateButton = new QPushButton(tr("Eliminar"));
    eliminateButton->setObjectName("redButton");
    eliminateButton->setFixedSize(180,50);

    connect(eliminateButton,SIGNAL(clicked()),this,SLOT(toEliminate()));
    connect(selectButton,SIGNAL(clicked()),this,SLOT(toSelect()));
    connect(enableCheckbox,SIGNAL(stateChanged(int)),this,SLOT(toEnable(int)));

    QGridLayout *l = new QGridLayout(w);
    l->addWidget(selectButton   ,0,0,1,4,Qt::AlignLeft);
    l->addWidget(enableCheckbox ,0,5,1,1,Qt::AlignRight);
    l->addWidget(eliminateButton,0,6,1,1,Qt::AlignRight);

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

userSiteBox::~userSiteBox()
{
}

void userSiteBox::toEliminate()
{
    emit eliminate(_id);
}

void userSiteBox::toSelect()
{
    emit select(_id);
}

void userSiteBox::toEnable(int i){
    emit enable(_id,i);
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

dialogSite::dialogSite(QWidget * parent) : QWidget(parent)
{
    setFormLocation();
    setFormAddLocation();
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

dialogSite::~dialogSite()
{
}


void dialogSite::refresh(QString s)
{
    foreach(userSiteBox *l, results){
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
            userSiteBox *b = new userSiteBox(i, p.getValue("id").toString(),p.getValue("name").toString(),p.getValue("enable").toInt());

        //--------------------------------------------------------------------------------
            //connect(b,SIGNAL(select(int)),this,SLOT(selectUser(int)));

            connect(b,SIGNAL(enable(int,int)),this,SLOT(enableLocation(int,int)));
            connect(b,SIGNAL(eliminate(int)),this,SLOT(eliminateLocation(int)));
            resultLayout->addWidget(b);
            results.append(b);
        }
        resultLayout->setAlignment( Qt::AlignTop );
        resultLayout->setSpacing(3);
        resultLayout->setMargin(0);
    }
}

void dialogSite::setFormLocation()
{
    search = new QWidget;

    addButton = new QPushButton(tr("Agregar locación"));
    addButton->setObjectName("greenButton");
    addButton->setFixedSize(200,50);
    connect(addButton,SIGNAL(clicked()),this,SLOT(addLocation()));


    QWidget *labels             = new QWidget();
    QGridLayout *layout_titles  = new QGridLayout(this);
    QLabel *label_location      = new QLabel(tr("Locación"));
    QLabel *label_enable        = new QLabel(tr("Habilitar"));
    QLabel *label_eliminate     = new QLabel(tr("Eliminar"));

    layout_titles->addWidget(label_location,0,0,1,4);
    layout_titles->addWidget(label_enable,0,4,1,1,Qt::AlignRight);
    layout_titles->addWidget(label_eliminate,0,5,1,1,Qt::AlignCenter);
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

    sl->addWidget(addButton);
    sl->addSpacing(20);

    sl->addWidget(labels);
    sl->addWidget(area);
    sl->setMargin(20);
    sl->setSpacing(3);
}

void dialogSite::setFormAddLocation()
{
    add = new QWidget;
    add->setObjectName("patientForm");
    add->setContentsMargins(20,20,20,20);

    QFormLayout * a = new QFormLayout(add);
    addLocationName = new QVkLineEdit;

    QList<QLabel *>formLabel;
    formLabel << new QLabel(tr("Locación"));

    foreach(QLabel *l, formLabel)
        l->setFixedWidth(220);

    a->addRow(formLabel.at(0),addLocationName);

    QWidget *buttons = new QWidget();
    QHBoxLayout *b = new QHBoxLayout(buttons);
    QPushButton * save = new QPushButton(QIcon(":/icon/res/img/save.png"),tr("Guardar"));
    connect(save,SIGNAL(clicked()),this,SLOT(saveLocation()));
    save->setIconSize(QSize(32,32));
    save->setObjectName("blueButton");

    QPushButton * cancel = new QPushButton(QIcon(":/icon/res/img/cancel.png"),tr("Cancelar"));
    connect(cancel,SIGNAL(clicked()),this,SLOT(goFormLocation()));
    cancel->setIconSize(QSize(32,32));
    cancel->setObjectName("redButton");

    b->setMargin(0);
    b->setSpacing(1);
    b->addWidget(save);
    b->addWidget(cancel);

    a->addRow("",buttons);
}

void dialogSite::loadFormLocation(int id){
    IdToEdit = id;

    /*
    p.loadData(id);
    addLocationName->setText(p.getValue("name").toString());
    if(p.getValue("enable").toInt() == 1)
        isAdmin->setChecked(true);
    else isAdmin->setChecked(false);
    toEdit = true;
    IdToEdit = id;
    */
}

void dialogSite::addLocation(void)
{
    goFormAddLocation();
    addLocationName->setText("");
    //toEdit = false;
}

void dialogSite::goFormAddLocation(){
    _widgets->slideInIdx(1,SlidingStackedWidget::BOTTOM2TOP);
}

void dialogSite::goFormLocation(){
   _widgets->slideInIdx(0,SlidingStackedWidget::TOP2BOTTOM);
}

void dialogSite::selectUser(int a)
{
    emit selected(a);
    close();
}

void dialogSite::eliminateLocation(int id)
{
    if(QMessageBox::question(this,tr("Eliminar puestos de salud"),
    tr("¿Está seguro de eliminar este puesto de salud?"),
    QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
    {
        p.eliminateSite(id);
        refresh("");
    }
}


void dialogSite::saveLocation(void)
{
    QString n = addLocationName->text();

    if(n=="")
    {
        QMessageBox::information(this,tr("Campo vacio"), tr("Completar el nombre de la locación"));
        return;
    }

    QHash<QString,QString> d;
    d.insert("name",n);
    d.insert("enable","0");
    p.insert(d);
    refresh("");
    goFormLocation();

}

void dialogSite::enableLocation(int id,int state)
{
    p.loadData(id);
    QHash<QString,QString> d;

    if(state)
        d.insert("enable","1");
    else
        d.insert("enable","0");

    p.update(d,id);        
}


//---------------------------------------------------


