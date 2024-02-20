#include "myffplay.h"

myffplay* myffplay::player = nullptr;
//QString myffplay::basicOption = "-x 1024 -y 768 -autoexit"; //JB 30012020 se agrego linea inferior
QString myffplay::basicOption = "-x 1024 -y 768 -autoexit -fs";

myffplay::myffplay() : QObject()
{
    videoplayer = new QProcess;
    connect(videoplayer,SIGNAL(started()),this,SLOT(videoStarted()));
    connect(videoplayer,SIGNAL(finished(int)),this,SLOT(videoFinished(int)));
    connect(videoplayer,SIGNAL(bytesWritten(qint64)),this,SLOT(videoHaveByteWritted(qint64)));


//    videoHideBar = new QWidget();
//    videoHideBar->setObjectName("videoToolBar");
//    videoHideBar->setFixedSize(1024,40);
//    videoHideBar->setWindowFlags( Qt::Tool |  Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

//    videoHideBar->hide();


//    videoToolBar = new QWidget();
//    videoToolBar->setObjectName("videoToolBar");
//    videoToolBar->setFixedSize(130,50);
//    videoToolBar->setWindowFlags( Qt::Tool |  Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
//    videoToolBar->hide();
//    setToolBar();
    isplaying = false;
}



myffplay::~myffplay()
{

}

void myffplay::start(QString ffplay){
//  setToolBar();
    videoplayer->start(ffplay);
}

//---------------------------------------------
//  Christiam
bool myffplay::waitForFinished()
{
    return videoplayer->waitForFinished();
}
//---------------------------------------------


void myffplay::setToolBar(){
    QHBoxLayout * layout = new QHBoxLayout(videoToolBar);
    QSize bsize(130,50);
    QSize iconSize(30,30);

    button_quit = new QToolButton;
    button_quit->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    button_quit->setIcon(QIcon(":/icon/res/img/capture_button/close.png"));
    button_quit->setText(tr("Cerrar"));
    button_quit->setIconSize(iconSize);
    button_quit->setFixedSize(bsize);
    connect(button_quit,SIGNAL(clicked()),this,SLOT(videoClose()));

    button_pause = new QToolButton;
    button_pause->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    button_pause->setIcon(QIcon(":/icon/res/img/capture_button/play.png"));
    button_pause->setText(tr("Pausa"));
    button_pause->setIconSize(iconSize);
    button_pause->setFixedSize(bsize);
    connect(button_pause,SIGNAL(clicked()),this,SLOT(videoPlayPause()));

    button_nextFrame = new QToolButton;
    button_nextFrame->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    button_nextFrame->setIcon(QIcon(":/icon/res/img/capture_button/play.png"));
    button_nextFrame->setText(tr("Siguiente"));
    button_nextFrame->setIconSize(iconSize);
    button_nextFrame->setFixedSize(bsize);
    connect(button_nextFrame,SIGNAL(clicked()),this,SLOT(videoNextFrame()));

    button_backward = new QToolButton;
    button_backward->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    button_backward->setIcon(QIcon(":/icon/res/img/capture_button/play.png"));
    button_backward->setText(tr("-10s"));
    button_backward->setIconSize(iconSize);
    button_backward->setFixedSize(bsize);
    connect(button_backward,SIGNAL(clicked()),this,SLOT(videoNextFrame()));

    button_forward = new QToolButton;
    button_forward->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    button_forward->setIcon(QIcon(":/icon/res/img/capture_button/play.png"));
    button_forward->setText(tr("+10s"));
    button_forward->setIconSize(iconSize);
    button_forward->setFixedSize(bsize);
    connect(button_forward,SIGNAL(clicked()),this,SLOT(videoForward()));

    layout->addWidget(button_backward);
    layout->addWidget(button_pause);
    layout->addWidget(button_nextFrame);
    layout->addWidget(button_forward);
    layout->addWidget(button_quit);
    layout->setSpacing(5);
    layout->setMargin(0);

}

void myffplay::setPlayButton(){
    if(isplaying){
        button_pause->setIcon(QIcon(":/icon/res/img/capture_button/play.png"));
        button_pause->setText(tr("Pausa"));
    }
    else {
        button_pause->setIcon(QIcon(":/icon/res/img/capture_button/pause.png"));
        button_pause->setText(tr("Reproducir"));
    }
}

void myffplay::videoClose(){
    /*videoToolBar->hide();
    videoHideBar->hide();*/
    videoplayer->close();
}

void myffplay::videoPlayPause(){

}

void myffplay::videoNextFrame(){

}

void myffplay::videoForward(){

}

void myffplay::videoBackward(){

}

void myffplay::videoStarted(){
    isplaying = true;
    /*videoToolBar->move(accesor::mw->x(),accesor::mw->y()+accesor::mw->height()-50);
    videoHideBar->move(accesor::mw->x(),accesor::mw->y());
    videoToolBar->show();
    videoHideBar->show();*/

}

void myffplay::videoFinished(int x){
    qDebug() << x;
    isplaying =false;
    //videoToolBar->hide();
    //videoHideBar->hide();
}

void myffplay::closeEvent(QCloseEvent * e){

}

void myffplay::videoHaveByteWritted(qint64 i){
}

