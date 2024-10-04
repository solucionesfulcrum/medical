#include "visor.h"

visorItem::visorItem(int i, QString n, QString f,  QWidget *parent) : QWidget(parent){


    name = new QLabel(n);
    name->setObjectName("VisorListItem");
    name->setProperty("checked", "false");
    _filename = f;
    id = i;

    QHBoxLayout * layout = new QHBoxLayout(this);
    layout->addWidget(name,5);
    layout->setMargin(0);
    layout->setSpacing(0);
}

void visorItem::play(){

}

QString visorItem::filename(){
    return _filename;
}

void visorItem::isPlaying(bool b){

    if(b)
        name->setProperty("checked", "true");
    else name->setProperty("checked", "false");
    name->style()->unpolish(name);
    name->style()->polish(name);

}

void visorItem::mouseReleaseEvent(QMouseEvent  * event){

    if(event->button() == Qt::LeftButton ){
        emit setCurrent(id);
    }
}






visor::visor(QMedicalBoxWidget *parent) : QMedicalBoxWidget(parent)
{

    QFile file("help.json");
    if (!file.open(QIODevice::ReadOnly)){
        return;
    }

    QByteArray json = file.readAll();
    QJsonDocument itemDoc = QJsonDocument::fromJson(json);
    foreach(QJsonValue obj, itemDoc.array())
        protocols.append(obj.toObject());



    videoPlayerWidget();
    listBox = new QWidget();
    listBox->setObjectName("historial");
    listBoxLayout = new QVBoxLayout(listBox);
    listBoxLayout->setSpacing(0);
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

    mainLayout->setDirection(QBoxLayout::LeftToRight);
    mainLayout->addWidget(area,2);
    mainLayout->addWidget(_videoPlayerWidget,5);
    mainLayout->setSpacing(5);
    mainLayout->setMargin(20);

    int id = 0;
    foreach(QJsonObject prot, protocols){
        QLabel * title = new QLabel(prot.value("protocol").toString());
        title->setObjectName("VisorListTitle");
        title->setAlignment(Qt::AlignCenter);
        listBoxLayout->addWidget(title);
        foreach(QJsonValue s, prot.value("sweeps").toArray()){
            QJsonObject sweep = s.toObject();
            visorItem * l = new visorItem(id, sweep.value("name").toString(),sweep.value("file").toString());
            connect(l, SIGNAL(setCurrent(int)),this,SLOT(setCurrentVideo(int)));
            visorItems.append(l);
            listBoxLayout->addWidget(l);
             id++;
        }
    }
    if(id>0){
        visorItems.at(0)->isPlaying(true);
        player->setMedia((QUrl::fromLocalFile(visorItems.at(0)->filename())));
    }

}

visor::~visor()
{

}

void visor::setCurrentVideo(int id){
    foreach(visorItem *vi, visorItems)
        vi->isPlaying(false);
    visorItems.at(id)->isPlaying(true);
    qDebug() << visorItems.at(id)->filename();
    player->setMedia(QMediaContent(QUrl::fromLocalFile(visorItems.at(id)->filename())));
    player->play();
}

void visor::videoPlayerWidget(){


    connect(&poller,SIGNAL(timeout()),this,SLOT(refresh()));

    videoSlider = new QSlider;
    videoSlider->setOrientation(Qt::Horizontal);
    videoSlider->setRange(0,1000);
    videoSlider->setObjectName("videoSlider");
    connect(videoSlider,SIGNAL(sliderReleased()),this,SLOT(videoSliderReleased()));

    player = new QMediaPlayer;
   // playlist = new QMediaPlaylist();
   // playlist->setPlaybackMode(QMediaPlaylist::CurrentItemOnce);

    videoWidget = new QVideoWidget;
    player->setVideoOutput(videoWidget);
    //player->setPlaylist(playlist);
    connect(player,SIGNAL(stateChanged(QMediaPlayer::State)),this, SLOT(playerStateChanged(QMediaPlayer::State)));

    playPause = new QPushButton(QIcon(":/icon/res/img/visor/play.png"),"");
    playPause->setFixedSize(64,64);
    playPause->setObjectName("visorPlayButton");
    connect(playPause,SIGNAL(clicked()),this, SLOT(togglePlay()));

    _videoPlayerWidget = new QWidget;
    _videoPlayerWidget->setStyleSheet("background:#000000;");
    QGridLayout * vblVW = new QGridLayout(_videoPlayerWidget);
    vblVW->addWidget(videoWidget,0,0,1,2);
    vblVW->setRowStretch(0,10);
    vblVW->addWidget(videoSlider,1,1);
    vblVW->addWidget(playPause,1,0,Qt::AlignCenter);
    vblVW->setColumnStretch(1,10);
    vblVW->setRowStretch(1,1);

}


void visor::togglePlay(){
    if(player->state() == QMediaPlayer::StoppedState){
        player->play();
        return;
    }
    if(player->state() == QMediaPlayer::PlayingState){
        player->pause();
        return;
    }
    if(player->state() == QMediaPlayer::PausedState){
        player->play();
        return;
    }
}

void visor::refresh(){
    if (!videoSlider->isSliderDown()){
        if(player->duration() > 0){
            int now = player->position()*1000 / player->duration();
            videoSlider->setValue(now);
        }
        else {
            videoSlider->setValue(0);
        }
    }
}

void visor::playerStateChanged(QMediaPlayer::State state){
    if(state == QMediaPlayer::StoppedState){
        playPause->setIcon(QIcon(":/icon/res/img/visor/play.png"));
        poller.stop();
        videoSlider->setValue(0);
    }

    if(state == QMediaPlayer::PlayingState){
       playPause->setIcon(QIcon(":/icon/res/img/visor/pause.png"));
       poller.start(10);
    }
    if(state == QMediaPlayer::PausedState){
        playPause->setIcon(QIcon(":/icon/res/img/visor/play.png"));
        poller.stop();
    }
}

void visor::videoSliderReleased(){
    int p = videoSlider->value();
    player->setPosition(p*player->duration()/1000);
}

void visor::slidePositionUpdate(qint64 n){
    if(player->duration() > 0){
        int now = n*1000 / player->duration();
        videoSlider->setValue(now);
    }

}

