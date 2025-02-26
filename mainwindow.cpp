#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    isclosing = false;
    setSizes();

//  Init Pointer Global
    captureProcess::_captureProcess = new captureProcess();
    accesor::mw = (QMainWindow*) this;
    myffplay::player = new myffplay;
    QVirtualKeyboard::vk = new QVirtualKeyboard();
    QVirtualKeyboard::vk->setFixedWidth(mainwidth);

    //QVirtualKeyboard::vk->setVisibleKeyboard(false);
    //version = "1.4.2 (29/05/2019)";
    //version = "1.4.3 (08/11/2019)";
    //version = "2.0.1 (20/02/2020)";
    //version = "2.0.2 (17/07/2020)";
    //version = "2.0.5 (13/08/2020)";
    //version = "2.0.6 (11/10/2020)";
    //version = "2.0.7 (14/10/2020)";
    //version = "2.0.8 (20/10/2020)";
    //version = "2.0.9 (25/10/2020)";
    //version = "2.1.0 (03/11/2020)";
    //version = "2.2.0 (11/11/2020)";
    //version = "2.2.2 (21/01/2021)";
    //version = "2.2.3 (02/02/2021)";
    //version = "2.2.4 (21/02/2021)";
    //version = "2.2.5 (25/02/2021)";
    //version = "2.2.6 (01/03/2021)";
    //version = "2.2.7 (15/03/2021)";
    //version = "2.2.8 (22/05/2021)";
    //version = "2.2.9 (07/06/2021)";
    //version = "2.3.0 (24/06/2021)";
    //version = "2.3.1 (07/07/2021)";
    //version = "2.3.2 (13/07/2021)";
    //version = "2.3.3 (05/08/2021)";
    //version = "2.3.4 (25/08/2021)";
    //version = "2.3.5 (03/09/2021)";
    //version = "2.3.7 (22/09/2021)";
    //version = "2.3.8 (27/09/2021)";
    version = "2.3.9 (24/05/2022)";

    // setFixedSize(mainwidth,mainheight);
    QGraphicsColorizeEffect* effect = new QGraphicsColorizeEffect;
    setGraphicsEffect(effect);
    accesor::stopEffect();

    //To discomment
    //setWindowFlags(Qt::Window | Qt::FramelessWindowHint );
    showFullScreen();
    setObjectName("mw");

//  Create the process of checking studies
    _studycheck = new studyCheck;

//  Create the process for alert server that studies had been created
    pStudyFinished = new StudyFinished;

//  Create mainWindow
    mainwindow = new QWidget;

    //Set Header
    header();

//  Queue: Contains the list of studies send it to the server
    setQueueWidget();

    //Set Main Widget
    setMainWidget();

    //SetMenu
    menu();

    //Set Footer
    footer();

    QGridLayout * l = new QGridLayout(mainwindow);
    l->addWidget(_header,0,0,1,3);
    l->addWidget(_menu,1,0);
    l->addWidget(_main,1,1);

//--------------------------------------------------
//  CR: 20/01/21
    l->addWidget(_queue,1,2,Qt::AlignRight);
//--------------------------------------------------

    l->addWidget(_footer,2,0,1,3);
    l->setMargin(0);
    l->setSpacing(0);


//  Create Login Screen
    login = new WLogin;
    connect(login,SIGNAL(logged()),this,SLOT(setMainWindow()));
    connect(login,SIGNAL(canceled()),this,SLOT(closeSystem()));

    mainWidget = new SlidingStackedWidget(this);
    mainWidget->addWidget(login);
    mainWidget->addWidget(mainwindow);
    mainWidget->setAnimation(QEasingCurve::InOutCubic);

    //mainWidget->setFixedSize(1366,768);
    mainWidget->setFixedSize(1920,1080);

    setCentralWidget(mainWidget);
    setQueueGeometry();

    if(isQueue)
        _queue->setGeometry(showQueueGeometry);
    else  _queue->setGeometry(hideQueueGeometry);
    move ( 0, 0 );

}

void MainWindow::mousePressEvent(QMouseEvent * event){
    if(event->button() == Qt::LeftButton ){
        QVirtualKeyboard::vk->finish();
    }
}

void MainWindow::setSizes(){
    headerheight = 60;
    footerheight = 60;
    mainwidth = 1280;
    mainheight = 800;
    queueWidth = 380;
}

MainWindow::~MainWindow()
{
    _studycheck->stop();
    _studycheck->deleteLater();
}


void MainWindow::setLogin(){
    mainWidget->slideInIdx(0,SlidingStackedWidget::LEFT2RIGHT);
    //Stop Checking studies
    _studycheck->stop();
    if(isQueue)
        toggleQueue();
}

void MainWindow::setMainWindow(){
    mainWidget->slideInIdx(1,SlidingStackedWidget::RIGHT2LEFT);
    login->init();
    _configuration->refreshOpe();
    if (operators::isAdmin())
        menuAddOp->show(); 
    else
        menuAddOp->hide();
    operators op;
    user->setText(op.opName());

//  Start Checking studies
    _studycheck->start();

//  Start review of studies registered on server
    pStudyFinished->start();


//  Start Check bandwith
    cb->start();

    setmenuUS();

    _queue->show();;
    _queue->raise();
}

void MainWindow::header(){
    _header = new QWidget;
    _header->setObjectName("header");
    _header->setFixedHeight(headerheight);
    _header->setContentsMargins(0,0,0,0);

    QLabel * logo = new QLabel;
    QPixmap p  = QPixmap::fromImage(QImage(":/icon/res/img/header/operator.png"));
    logo->setPixmap(p);
    logo->setAlignment(Qt::AlignCenter);
    logo->setFixedWidth(32);

    user = new QLabel;
    user->setObjectName("user");

    menuClose = new QToolButton();
    menuClose->setText(tr("Cerrar"));
    menuClose->setIcon(QIcon(":/icon/res/img/header/logout.png"));
    menuClose->setObjectName("menuClose");
    connect(menuClose,SIGNAL(clicked()),this,SLOT(logout()));

    QSize menuButtonSize(60,60);
    QSize menuIconSize(50,50);

    QLabel *versionLabel = new QLabel(version);
    versionLabel->setObjectName("versionLabel");
    menuButtons << menuClose;

    QHBoxLayout * layout = new QHBoxLayout(_header);
    layout->setMargin(0);
    layout->setSpacing(0);
    layout->setAlignment(Qt::AlignLeft);
    layout->addSpacing(30);
    layout->addWidget(logo);
    layout->addSpacing(5);
    layout->addWidget(user);

    layout->addStretch(50);
    layout->addWidget(versionLabel,0, Qt::AlignBottom);
    layout->addSpacing(60);
    foreach (QToolButton * bt, menuButtons){
        bt->setFixedSize(menuButtonSize);
        bt->setIconSize(menuIconSize);
        layout->addWidget(bt,0,Qt::AlignRight);
    }
}

void MainWindow::menu(){
    _menu = new QWidget;
    _menu->setObjectName("menu");
    _menu->setFixedWidth(180);

//-----------------------------------------------------------------------------------
//  Christiam
    int size = 24;

    menuUS = new QToolButton();
    menuUS->setStyleSheet("QToolButton{font-size: "+QString::number(size)+"px;}");
    menuUS->setText(tr("Ecografía"));
    menuUS->setCheckable(true);
    connect(menuUS,SIGNAL(clicked()),this,SLOT(setmenuUS()));

    menuConfig = new QToolButton();
    menuConfig->setStyleSheet("QToolButton{font-size: "+QString::number(size)+"px;}");
    menuConfig->setText(tr("Ajustes"));
    menuConfig->setCheckable(true);
    connect(menuConfig,SIGNAL(clicked()),this,SLOT(setmenuConfig()));

    menuHist = new QToolButton();
    menuHist->setStyleSheet("QToolButton{font-size: "+QString::number(size)+"px;}");
    menuHist->setText(tr("Historial"));
    menuHist->setCheckable(true);
    connect(menuHist,SIGNAL(clicked()),this,SLOT(setmenuHist()));

    menuAddOp = new QToolButton();
    menuAddOp->setStyleSheet("QToolButton{font-size: "+QString::number(size)+"px;}");
    menuAddOp->setText(tr("Operadores"));
    menuAddOp->setCheckable(true);
    connect(menuAddOp,SIGNAL(clicked()),this,SLOT(setmenuOperador()));

    menuHelp = new QToolButton();
    menuHelp->setStyleSheet("QToolButton{font-size: "+QString::number(size)+"px;}");
    menuHelp->setText(tr("Tutoriales"));
    menuHelp->setCheckable(true);
    connect(menuHelp,SIGNAL(clicked()),this,SLOT(setmenuHelp()));
//-----------------------------------------------------------------------------------

    toolButtons << menuUS
                << menuHist
                << menuConfig
                << menuAddOp
                << menuHelp ;

    //QSize toolButtonSize(180,95);
    //QSize toolIconSize(64,64);
    //QSize toolButtonSize(180,95);
    QSize toolButtonSize(140,95);
    QSize toolIconSize(70,70);

    QVBoxLayout * layout = new QVBoxLayout(_menu);
    layout->setMargin(0);
    layout->setSpacing(10);
    layout->setAlignment(Qt::AlignCenter | Qt::AlignTop);
    layout->addSpacing(10);
    foreach (QToolButton * bt, toolButtons){
        bt->setFixedSize(toolButtonSize);
        bt->setIconSize(toolIconSize);
        bt->setToolButtonStyle( Qt::ToolButtonTextUnderIcon );
        layout->addWidget(bt,0,Qt::AlignLeft);
    }
}


void MainWindow::setMenuDisabled(bool b){
   if(b)
    _menu->hide();
   else _menu->show();
}

void MainWindow::footer(){

    battery *bt = new battery();
    diskSpace *ds = new diskSpace();
    cb = new checkBandwith;
//--------------------------------------------------------------------------------------
//  CR: 21/01/20
//  If Wifi connection is ok, it updates a variable of _study.
    connect(cb,&checkBandwith::Wifi_status,_study,&study::Wifi_status);
//--------------------------------------------------------------------------------------
    captureProcessWidget *cpw = new captureProcessWidget();

    _footer = new QWidget();
    _footer->setFixedHeight(footerheight);
    _footer->setObjectName("footer");
    QHBoxLayout *layout = new QHBoxLayout(_footer);
    layout->setMargin(0);
    layout->setSpacing(0);

//  Christiam: Los widget wifi y eth no se agregan al layout
    QPushButton * wifi = new QPushButton(QIcon(":/icon/res/img/footer/wifi.png"),"");
    connect(wifi,&QPushButton::clicked, this, &MainWindow::wifiConfig);
    QPushButton * eth = new QPushButton(QIcon(":/icon/res/img/footer/net.png"),"");
    connect(eth,&QPushButton::clicked, this, &MainWindow::ethernetConfig);

    wifi->setFixedSize(50,50);
    wifi->setIconSize(QSize(50,50));
    eth->setFixedSize(50,50);
    eth->setIconSize(QSize(50,50));

    menueQueue = new QToolButton();
    menueQueue->setText(tr("Ver cola"));
    menueQueue->setIcon(QIcon(":/icon/res/img/footer/queue.png"));
    menueQueue->setObjectName("QueueLookButton");
    menueQueue->setCheckable(true);
    menueQueue->setChecked(false);

//  CR: 20/01/21
//  Comment this line to disable toggle of queue widget.
//    connect(menueQueue,SIGNAL(clicked()),this,SLOT(toggleQueue()));

    QList<QToolButton*> footerButton;
    footerButton << menueQueue;
    QSize footerButtonSize(60,60);
    QSize footerIconSize(35,35);
    layout->addSpacing(10);
    layout->addWidget(bt);
    layout->addSpacing(30);
    layout->addWidget(ds);
    layout->addSpacing(20);
    layout->addWidget(cpw);
    layout->addSpacing(20);
    layout->addWidget(cb);
    //layout->addWidget(wifi);
    layout->addSpacing(5);
    //layout->addWidget(eth);


    layout->addStretch(10);

    QLabel *d =  new QLabel("");
    d->setFixedSize(282,50);
    d->setAlignment(Qt::AlignCenter);
    layout->addSpacing(3);
    layout->addWidget(_queue->infoWidget());
    foreach (QToolButton * bt, footerButton){
        bt->setFixedSize(footerButtonSize);
        bt->setIconSize(footerIconSize);
        layout->addWidget(bt,Qt::AlignRight);
        layout->addSpacing(1);
    }



}

void MainWindow::uncheckMenu(){
    menuUS->setChecked(false);
    menuConfig->setChecked(false);
    menuHist->setChecked(false);
    menuClose->setChecked(false);
    menuAddOp->setChecked(false);
    menuHelp->setChecked(false);

    menuUS->setIcon(QIcon(":/icon/res/img/menu/iconMenu_01.png"));
    menuConfig->setIcon(QIcon(":/icon/res/img/menu/iconMenu_03.png"));
    menuHist->setIcon(QIcon(":/icon/res/img/menu/iconMenu_02.png"));
    menuAddOp->setIcon(QIcon(":/icon/res/img/menu/iconMenu_04.png"));
    menuHelp->setIcon(QIcon(":/icon/res/img/menu/iconMenu_05.png"));
}

void MainWindow::loadStudy(int id){
    setmenuUS();
    _study->loadStudy(id);
}


void MainWindow::setmenuUS(){
    setmenu(0);
    uncheckMenu();
    menuUS->setChecked(true);
    menuUS->setIcon(QIcon(":/icon/res/img/menu/iconMenuBlue_01.png"));
}

void MainWindow::setmenuHelp(){
    setmenu(1);
    uncheckMenu();
    menuHelp->setChecked(true);
    menuHelp->setIcon(QIcon(":/icon/res/img/menu/iconMenuBlue_05.png"));
}


void MainWindow::setmenuConfig(){
    _configuration->updateCams();

    setmenu(2);
    uncheckMenu();
    menuConfig->setChecked(true);
    menuConfig->setIcon(QIcon(":/icon/res/img/menu/iconMenuBlue_03.png"));
}

void MainWindow::setmenuHist(){
    if (_main->currentIndex() != 3)
        _historical->reset();
    setmenu(3);
    uncheckMenu();
    menuHist->setChecked(true);
    menuHist->setIcon(QIcon(":/icon/res/img/menu/iconMenuBlue_02.png"));
}

void MainWindow::setmenuOperador(){
    setmenu(4);
    uncheckMenu();
    menuAddOp->setChecked(true);
    menuAddOp->setIcon(QIcon(":/icon/res/img/menu/iconMenuBlue_04.png"));
}



void MainWindow::setmenu(int i){
    showFullScreen();
    _main->setCurrentIndex(i);
}

void MainWindow::setMainWidget(){
    _main = new QStackedWidget;
    _operatores = new dialogOperator;
    _study = new study;
    _study->setQueueWidget(_queue);
    connect(_study,SIGNAL(studyStarted(bool)),this,SLOT(setMenuDisabled(bool)));

    _visor = new visor;
    _configuration = new configuration;
    _historical = new historical();
    _historical->setQueueWidget(_queue);
    connect(_historical,SIGNAL(loadStudyId(int)),this,SLOT(loadStudy(int)));

    _main->addWidget(_study);           // Index 0
    _main->addWidget(_visor);           // Index 1
    _main->addWidget(_configuration);
    _main->addWidget(_historical);
    _main->addWidget(_operatores);
}

void MainWindow::setQueueWidget(){
    _queue = new QueueWidget(this);
    connect(_queue,SIGNAL(isThreadFinished()),this,SLOT(queueFinished()));
    _queue->start();

    showQueueAnimation = new QPropertyAnimation(_queue, "geometry");
    connect(showQueueAnimation,SIGNAL(finished()),this,SLOT(update()));
    showQueueAnimation->setDuration(200);

    setQueueGeometry();
    isQueue =false;
}

void MainWindow::logout(){

//----------------------------------------------------
//  CR: 20/01/21
    if(_queue->_series.listeIDtoQueue().count()!=0){
        if(QMessageBox::question(this,tr("Advertencia"),tr("Existen estudios pendientes de envio, se recomienda no cerrar la aplicación y/o revisar la conexión a internet. ¿Desea continuar?"), QMessageBox::Yes, QMessageBox::No) == QMessageBox::No) return;
    }
//----------------------------------------------------

    if(!isCapturing())
        setLogin();   
}

bool MainWindow::isCapturing(){
    if(_study->isCapturing()){
        QMessageBox::warning(this,tr("Adquisición en proceso"),tr("Un barrido esta en proceso de adqusición, por favor terminé la adqusición actual, antes de realizar otra acción."));
        return true;
    }
    else return false;
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);
    setQueueGeometry();
}

void MainWindow::setQueueGeometry(){
    int xpos =  geometry().width()-queueWidth;
    int h = geometry().height()-footerheight-headerheight;
    int topup = headerheight;

    hideQueueGeometry = QRect(geometry().width(), topup, 0, h);
    showQueueGeometry = QRect(xpos,topup, queueWidth, h);

}

void MainWindow::toggleQueue(){
    if(isQueue)
        _queue->setGeometry(showQueueGeometry);
    else _queue->setGeometry(hideQueueGeometry);


    showQueueAnimation->stop();
    _queue->raise();


    if(isQueue){
        //Hide
        showQueueAnimation->setStartValue(_queue->geometry());
        showQueueAnimation->setEndValue(hideQueueGeometry);
        isQueue = false;
        menueQueue->setChecked(false);
    }
    else {
        //Show
        showQueueAnimation->setStartValue(_queue->geometry());
        showQueueAnimation->setEndValue(showQueueGeometry);
        isQueue = true;
        menueQueue->setChecked(true);
    }
    showQueueAnimation->start();

}


void MainWindow::ethernetConfig(){
    HWND   w = (HWND)winId();
    HINSTANCE h =  ShellExecute(
                w,
                L"open",
                L"shell:::{7007ACC7-3202-11D1-AAD2-00805FC1270E}",
                NULL,
                NULL,
                SW_SHOWNORMAL);

}
void MainWindow::queueFinished(){
    if(isclosing){
        isclosing = false;
        stopDiag->close();
        delete stopDiag;
        close();

    }
}


void MainWindow::wifiConfig(){
    HWND   w = (HWND)winId();
    HINSTANCE h =  ShellExecute(
                w,
                L"open",
                L"shell:::{1FA9085F-25A2-489B-85D4-86326EEDCD87}",
                NULL,
                NULL,
                SW_SHOWNORMAL);

}


void MainWindow::closeSystem(){

    if(QMessageBox::question(this,tr("Salir"),tr("¿Está seguro de salir del sistema?"), QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes){
        _queue->stop();
        //qDebug() << "Kill Capture process if running";
        captureProcess::_captureProcess->killProcess();
        //qDebug() << "Checking Studies is running: " <<  _studycheck->isFinished();
        //qDebug() << "Queue is running: " <<  _queue->IsThreadRunning();
        if (_studycheck->isFinished() && !_queue->IsThreadRunning() )
            close();
        else {
            isclosing = true;
            stopDiag = new QDialog;
            stopDiag->setWindowFlags( Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
            stopDiag->setModal(true);
            stopDiag->setObjectName("upProtDialog");
            stopDiag->setFixedSize(400,200);
            QVBoxLayout *hl = new QVBoxLayout(stopDiag);
            hl->addWidget( new QLabel(tr("Cerrando")));
            stopDiag->show();
        }
    }

}

void MainWindow::closeEvent(QCloseEvent *){
    //ExitWindowsEx(EWX_SHUTDOWN,0);
}
