#include "wlogin.h"

WLogin::WLogin(QWidget *parent) : QWidget(parent)
{
    setMinimumSize(400,400);
    QVBoxLayout * mainLayout = new QVBoxLayout(this);

//  Background Widget
    QWidget * bg = new QWidget();
    bg->setObjectName("bglogin");
    mainLayout->addWidget(bg);
    mainLayout->setSpacing(0);
    mainLayout->setMargin(0);

    QLabel * logo = new QLabel();
    logo->setPixmap(QPixmap(":/icon/res/img/logow.png"));
    //logo->setFixedSize(400,400);
    logo->setObjectName("block");
    logo->setAlignment(Qt::AlignCenter);

//  29/02/2024 SE MODIFICA ANCHO A 420 PARA AÑADIR NUEVO LOGO MEDICALINT
    _width = 420;
    //_width = 500;
    _y = 340;

    createUserLoginBox();
    createUserList();

    loginSlideWidget = new SlidingStackedWidget(this);
    loginSlideWidget->setAnimation(QEasingCurve::InOutCubic);
    loginSlideWidget->addWidget(logBox);
    loginSlideWidget->addWidget(_listBox);


    QWidget * loginWidget = new QWidget;
    loginWidget->setFixedWidth(_width);
    loginWidget->setFixedHeight(470);
    //loginWidget->setFixedHeight(500);
    loginWidget->setObjectName("loginWidget");
    QVBoxLayout * loginLayout = new QVBoxLayout(loginWidget);
    loginLayout->addWidget(logo,Qt::AlignCenter );
    loginLayout->addSpacing(20);
    loginLayout->addWidget(loginSlideWidget,Qt::AlignCenter);
    loginLayout->setSpacing(0);
    loginLayout->setMargin(35);

    QHBoxLayout * bgLayout = new QHBoxLayout(bg);
    bgLayout->addWidget(loginWidget);
    bgLayout->setSpacing(0);
    bgLayout->setMargin(0);
    createCloseButton();


    //To test
    testPass = "";
    init();


}

WLogin::~WLogin()
{

}

void WLogin::createCloseButton(){
    /*Close Button */
    QPushButton *close = new QPushButton(QIcon(":/icon/res/img/exit.png"),tr(""),this);
    connect(close,SIGNAL(clicked()),this,SLOT(cancel()));
    close->setFixedSize(80,80);
    close->setIconSize(QSize(80,80));
    close->move(0,0);
    close->setObjectName("closeLogin");
}

void WLogin::createUserLoginBox(){
    logBox = new QWidget(this);
    logBox->setObjectName("login");

    error = new QLabel(this);
    error->setAlignment(Qt::AlignCenter);
    error->setObjectName("loginerror");

    createUserBox();
    createUserLogin();

    QPushButton *log = new QPushButton(tr("Ingresar"));
    connect(log,SIGNAL(clicked()),this,SLOT(login()));

    QVBoxLayout *vb = new QVBoxLayout(logBox);
    vb->addWidget(_userBox);
    vb->addWidget(error);
    vb->addSpacing(40);
    vb->addWidget(_loginBox);
    vb->addSpacing(20);
    vb->addWidget(log);
    vb->setSpacing(0);
    vb->setMargin(0);
}


void WLogin::createUserBox(){
    _userBox = new QWidget;
    _user = new QLabel("-");
    _user->setObjectName("block");
    _user->setAlignment(Qt::AlignCenter);
    _user->setObjectName("loginUsername");

    QHBoxLayout * layout = new QHBoxLayout(_userBox);
    layout->addWidget(_user,5);
    layout->setSpacing(1);
    layout->setMargin(0);
}

void WLogin::createUserLogin(){
    QLabel* userpass = new QLabel;
    userpass->setPixmap(QPixmap(":/icon/res/img/logpass.png"));
    userpass->setFixedSize(50,50);
    userpass->setAlignment(Qt::AlignCenter);
    userpass->setObjectName("block");


    _loginBox = new QWidget;
//------------------------------------------------------
//  CR: 09/07/23
    _pass = new QVkLineEdit(true,true);
//------------------------------------------------------
    _pass->setFixedHeight(50);
    _pass->setEchoMode(QLineEdit::Password);
    _pass->setText(testPass);
    _pass->setPlaceholderText(tr("Contraseña"));
    _pass->setObjectName("passLine");

//--------------------------------------------------------------
//  CR: 31/01/23
    _pass->vk->edit->setEchoMode(QLineEdit::Password);
//--------------------------------------------------------------
    _userlistboton = new QPushButton(QIcon(":/icon/res/img/loglist.png"),tr(""));
    _userlistboton->setCheckable(true);
    _userlistboton->setFixedSize(50,50);
    _userlistboton->setIconSize(QSize(30,30));
    _userlistboton->setObjectName("changeOpeButton");
    connect(_userlistboton,SIGNAL(toggled(bool)),this,SLOT(showList(bool)));

    QHBoxLayout *input = new QHBoxLayout(_loginBox);
    input->setMargin(0);
    input->setSpacing(1);
    input->addWidget(userpass);
    input->addWidget(_pass);
    input->addWidget(_userlistboton);

}

void WLogin::createUserList(){
    _listBox = new QWidget;
    _listBox->setMaximumWidth(_width);
    _listWidg = new QWidget;
    _listWidg->setObjectName("_listWidg");

    _areaBox = new QScrollArea();
    _areaBox->setObjectName("loginArea");
    _areaBox->setFixedSize(_width,200);
    _areaBox->setWidgetResizable(true);
    _areaBox->setWidget(_listWidg);
    _areaBox->viewport()->setAttribute(Qt::WA_AcceptTouchEvents);
    QScroller::grabGesture(_areaBox->viewport(),QScroller::LeftMouseButtonGesture);

    QPushButton * closeOpe = new QPushButton(tr("Regresar"));
    closeOpe->setObjectName("redButton");
    closeOpe->setFixedHeight(50);
    closeOpe->setFixedWidth(120);
    connect(closeOpe,SIGNAL(clicked()),this,SLOT(cancelOpe()));


    QVBoxLayout * m = new QVBoxLayout(_listBox);
    m->addWidget(_areaBox);
    m->addSpacing(10);
    m->addWidget(closeOpe,Qt::AlignCenter);
    m->setMargin(0);
    listlayout = new QVBoxLayout(_listWidg);
    listlayout->setMargin(0);
    listlayout->setSpacing(1);
    listlayout->setSizeConstraint(QLayout::SetNoConstraint);
    listlayout->setAlignment( Qt::AlignTop );
    usersButtons = new QButtonGroup;
    connect(usersButtons,SIGNAL(buttonClicked(int)),this,SLOT(chooseOpe(int)));

    refreshList();


}



void WLogin::init(){
    error->hide();
    _pass->setText(testPass);
    idLogin = 0;
    int last = ope.lastOp();
    if(last != -1 ){
        ope.loadData(last);
        if(ope.getValue("name").toString().toUpper() != "")
        {
            idLogin = last;
            _user->setText(ope.getValue("name").toString().toUpper());
        }
    }
}




void WLogin::refreshList(){
    foreach(QAbstractButton *b, usersButtons->buttons()){
        listlayout->removeWidget(b);
        delete b;
    }
    _areaHeight = 0;

    QList<int> o = ope.listeID("");
    for(int i = 0; i<o.size(); i++){
        ope.loadData(o.at(i));

        if(ope.getValue("enable").toInt())
        {
            QPushButton * t = new QPushButton(ope.getValue("name").toString().toUpper());
            t->setFixedHeight(50);
            t->setFixedWidth(_areaBox->width()-35);
            usersButtons->addButton(t,ope.getValue("id").toInt());
            listlayout->addWidget(t);
            _areaHeight += 51;
        }
    }
}

void WLogin::login(){
    if(ope.logIn(idLogin,_pass->text()))
    {
//----------------------------------------------------------
//      CR: 31/01/23
        _pass->vk->edit->setEchoMode(QLineEdit::Normal);
//----------------------------------------------------------
        emit logged();
    }

    else {
        error->setText(tr("¡La contraseña es incorrecta!"));
        error->show();
    }
}

void WLogin::chooseOpe(int i){
    if(i != -1){
        operators ope;
        ope.loadData(i);
        idLogin = i;
        _user->setText(ope.getValue("name").toString().toUpper());
    }
    showList(false);
    _userlistboton->setChecked(false);
}


void WLogin::showList(bool b){
    refreshList();
    if(!b)
        loginSlideWidget->slideInIdx(0,SlidingStackedWidget::LEFT2RIGHT);
    else
        loginSlideWidget->slideInIdx(1,SlidingStackedWidget::RIGHT2LEFT);
    error->hide();
}

void WLogin::cancelOpe(){
    chooseOpe(-1);
}

void WLogin::cancel(){
    emit canceled();
}

