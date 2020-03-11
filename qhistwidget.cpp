#include "qhistwidget.h"



sweepInfo::sweepInfo(int id, QWidget *parent) : QWidget(parent){
    _series.loadData(id);

    _toqueue = new QLabel("");
    _capture = new QLabel("");
    _sent = new QLabel("");
    _confirmed = new QLabel("");

    _toqueue->setPixmap(QPixmap(":/icon/res/img/series/queue.png"));
    _capture->setPixmap(QPixmap(":/icon/res/img/series/capture.png"));
    _sent->setPixmap(QPixmap(":/icon/res/img/series/sent.png"));
    _confirmed->setPixmap(QPixmap(":/icon/res/img/series/confirmed.png"));

    lookVid = new QPushButton(QIcon(":/icon/res/img/player_play.png"),"");
    lookVid->setObjectName("greenButton");
    lookVid->setFixedSize(64,64);
    connect(lookVid, &QPushButton::clicked,this,&sweepInfo::view);
    QList<QLabel*> l;
    l << _toqueue << _capture << _sent << _confirmed;
    foreach(QLabel *lab, l)
    {
        lab->setFixedSize(30,30);
        lab->setObjectName("sweepsTags");
        lab->setAlignment(Qt::AlignCenter);
    }
    QLabel * title  = new QLabel(_studydesc.getSweepName(_series.getValue("id_sweeps").toInt()));
    QHBoxLayout *stepLayout = new QHBoxLayout;
    stepLayout->addWidget(_capture,0);
    stepLayout->addWidget(_toqueue,0);
    stepLayout->addWidget(_sent,0);
    stepLayout->addWidget(_confirmed,0);
    stepLayout->setAlignment(Qt::AlignLeft);
    stepLayout->setMargin(0);
    stepLayout->setSpacing(2);

    QVBoxLayout * tlayout = new QVBoxLayout();
    tlayout->addWidget(title);
    tlayout->addLayout(stepLayout);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addLayout(tlayout,5);
    layout->addWidget(lookVid,1,Qt::AlignCenter);
    layout->setMargin(0);
    layout->setSpacing(2);



    lookVid->setEnabled(false);
    refresh();

}

void sweepInfo::refresh(){
    if(_series.getValue("toqueue").toInt() == 1)
        _toqueue->setProperty("done",true);
    else _toqueue->setProperty("done",false);

    _toqueue->style()->unpolish(_toqueue);
    _toqueue->style()->polish(_toqueue);

    if(_series.getValue("capture").toInt() == 1)
        _capture->setProperty("done",true);
    else _capture->setProperty("done",false);

    _capture->style()->unpolish(_capture);
    _capture->style()->polish(_capture);

    if(_series.getValue("sent").toInt() == 1)
        _sent->setProperty("done",true);
    else _sent->setProperty("done",false);

    _sent->style()->unpolish(_sent);
    _sent->style()->polish(_sent);

    if(_series.getValue("confirmed").toInt() == 1)
        _confirmed->setProperty("done",true);
    else _confirmed->setProperty("done",false);

    _confirmed->style()->unpolish(_confirmed);
    _confirmed->style()->polish(_confirmed);

    qDebug() << _series.haveVideo()  << _series.getVideoFile();;
    if (_series.haveVideo())
        lookVid->setEnabled(true);

}

void sweepInfo::view(){
    config conf;

    QString f = _series.getVideoFile();
    QString p;
    if (f.right(3) == "yuv")
        p = "ffplay  "+myffplay::basicOption+" -f rawvideo -pix_fmt [PIXELCONF] -video_size [SIZE] -framerate [FPS] "+f;
    else p = "ffplay  "+myffplay::basicOption+" "+f;

    p = p.replace("[FPS]",conf.getValue("fps").toString());
    p = p.replace("[SIZE]",conf.getValue("SIZE").toString());
    p = p.replace("[PIXELCONF]",conf.getValue("PIXELCONF").toString());
    qDebug() << p;
    myffplay::player->start(p);
}

studyInfo::studyInfo(int id, QDialog *parent) : QDialog(parent){
    setWindowFlags( Qt::Tool |  Qt::FramelessWindowHint );
    setModal(true);
    setFixedSize(1200,600);

    accesor::startEffect();

    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)));

    studyID = id;
    filename = "studies/"+QString::number(studyID)+"/informe.pdf";
    report.setFileName(filename);
    _studies.loadData(id);
    _patient.loadData(_studies.id_patient());
    _studydesc.loadData(_studies.getValue("id_protocols").toInt());
    _operators.loadData(_studies.getValue("id_operators").toInt());

    createStudyBox();
    createSweepsBox();
    QWidget *_studyInfo = new QWidget;
    _studyInfo->setObjectName("StudyHist");


    QHBoxLayout * vl = new QHBoxLayout(_studyInfo);
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(_studyInfo);
    mainLayout->setSpacing(0);
    mainLayout->setMargin(0);

    vl->addWidget(studyBox);
    vl->addWidget(area,1);
    vl->setMargin(1);
    vl->setSpacing(10);
}

void studyInfo::closeEvent(QCloseEvent *event){
    accesor::stopEffect();
}


void studyInfo::createStudyBox(){
    studyBox = new QWidget;
    studyBox->setObjectName("studyBox");

    QLabel * patientname = new QLabel(_patient.name()+" "+_patient.lastName());
    QLabel * patientid = new QLabel(_patient.getValue("idp").toString());
    QLabel * protocol = new QLabel(_studydesc.getValue("name").toString());
    QLabel * opName = new QLabel(_operators.getValue("name").toString());
    QLabel * reason = new QLabel(_studies.getValue("reason").toString());
    QLabel * startDate = new QLabel(studies::datetimetoFormat(_studies.datetime()));
    QLabel * stopDate = new QLabel(studies::datetimetoFormat(_studies.getValue("finishtime").toString()));
    QLabel * statut = new QLabel(_studies.getState());
    titlelabel * title = new titlelabel("Estudio");
    title->setAlignment(Qt::AlignLeft);

    QPushButton * closeButton = new QPushButton(tr("Cerrar"));
    connect(closeButton,SIGNAL(clicked()),this,SLOT(close()));
    closeButton->setObjectName("redButton");
    closeButton->setFixedSize(200,60);

    dlButton = new QPushButton(QIcon(":/icon/res/img/checklist.png"),tr("Abrir el informe"));
    dlButton->setObjectName("greenButton");
    dlButton->setFixedSize(200,60);
    dlButton->setIconSize(QSize(32,32));
    connect(dlButton,SIGNAL(clicked()),this,SLOT(download()));
    if(_studies.getValue("report_link").toString() == "" )
        dlButton->setEnabled(false);
    int line = 0;
    QGridLayout * layout = new QGridLayout(studyBox);
    layout->setSpacing(5);
    layout->setAlignment(Qt::AlignTop);
    layout->addWidget(title,line,0,Qt::AlignCenter);
    layout->addWidget(dlButton,line,1,Qt::AlignCenter);
    line++;
    layout->addWidget(new QLabel(tr("DNI del Paciente:")),line,0);
    layout->addWidget(patientid,line,1);
    line++;
    layout->addWidget(new QLabel(tr("Paciente:")),line,0);
    layout->addWidget(patientname,line,1);
    line++;
    layout->addWidget(new QLabel(tr("Protocolos:")),line,0);
    layout->addWidget(protocol,line,1);
    line++;
    layout->addWidget(new QLabel(tr("Operador:")),line,0);
    layout->addWidget(opName,line,1);
    line++;
    layout->addWidget(new QLabel(tr("Razón:")),line,0);
    layout->addWidget(reason,line,1);
    line++;
    layout->addWidget(new QLabel(tr("Inicio:")),line,0);
    layout->addWidget(startDate,line,1);
    line++;
    layout->addWidget(new QLabel(tr("Fin:")),line,0);
    layout->addWidget(stopDate,line,1);
    line++;
    layout->addWidget(new QLabel(tr("Estado:")),line,0);
    layout->addWidget(statut,line,1);
    line++;
    layout->setColumnStretch(1,8);
    layout->addWidget(closeButton,line,0,1,2,Qt::AlignLeft);


}


void studyInfo::createSweepsBox(){
    sweepsBox = new QWidget;
    QWidget *listBox = new QWidget;
    listBox->setObjectName(tr("historial"));//JB18022020
    area = new QScrollArea();
    QVBoxLayout *lb = new QVBoxLayout(listBox);
    lb->setSpacing(15);
    lb->setAlignment(Qt::AlignTop);
    area->viewport()->setAttribute(Qt::WA_AcceptTouchEvents);
    QScroller::grabGesture(area->viewport(),QScroller::LeftMouseButtonGesture);
    area->setObjectName("studyinfoarea");
    area->setWidgetResizable(true);
    area->setWidget(listBox);
    area->viewport()->setAttribute(Qt::WA_AcceptTouchEvents);
    foreach(int i, _series.listeIDFromStudy(studyID))
        lb->addWidget(new sweepInfo(i));
}

void studyInfo::openFile(){

    QDesktopServices::openUrl(QUrl("file:///"+QDir::currentPath()+"/"+filename));
}

void studyInfo::download(){
    if(!report.exists()){
        dlButton->setText(tr("Recuperando..."));
        dlButton->setEnabled(false);
        //qDebug() << "Download: " <<   _studies.getValue("report_link").toString();
        pReply = manager->get(QNetworkRequest(QUrl(_studies.getValue("report_link").toString())));
        connect(pReply,SIGNAL(downloadProgress(qint64,qint64)),this,SLOT(dl(qint64,qint64)));
    }
    else openFile();
}

void studyInfo::replyFinished(QNetworkReply* pReply){
    QByteArray res = pReply->readAll();
    if(pReply->error() == QNetworkReply::NoError){
        QDir d;
        d.mkpath("studies/"+QString::number(studyID)+"/");
        if (report.open(QIODevice::WriteOnly)){
            report.write(res);
            report.close();

            QHash<QString, QString> data;
            data.insert("new_report","2");
            _studies.update(data,_studies.getValue("id").toInt());
            this->openFile();
        }
    }
    dlButton->setText(tr("Informe"));
    dlButton->setEnabled(true);
}

void studyInfo::dl(qint64  ,qint64 ){
    // qDebug() << "Download: " << a << n;
}



QHistWidget::QHistWidget(int id, QWidget *parent) : QWidget(parent)
{
    sid = id;
    _studies.loadData(id);
    _patient.loadData(_studies.id_patient());
    _studydesc.loadData(_studies.getValue("id_protocols").toInt());
    setFixedHeight(100);

    QVBoxLayout * tl = new QVBoxLayout(this);
    QWidget *studieWidget = new QWidget();
    setContentsMargins(20,10,20,10);
    studieWidget->setObjectName("studieWidget");
    tl->addWidget(studieWidget);
    tl->setMargin(0);
    tl->setSpacing(0);

    //Creation of label
    QLabel * date = new QLabel(studies::datetimetoFormat(_studies.datetime(),"dd/MM/yyyy"));
    QLabel * time = new QLabel(studies::datetimetoFormat(_studies.datetime(),"HH:mm"));
    QLabel * studydesc = new QLabel(_studydesc.getValue("name").toString());
    QLabel * patientName = new QLabel(_patient.name()+" "+_patient.lastName()+ " / "+_patient.id());
    statut = new QLabel();



    QMenu * studyMenu = new QMenu();
    studyMenu->setWindowFlags(studyMenu->windowFlags() | Qt::NoDropShadowWindowHint);
    QAction * showStudy = new QAction(tr("Ver información"));
    connect(showStudy,SIGNAL(triggered()),this, SLOT(openInfo()));
    studyMenu->addAction(showStudy);
    QAction * restartStudy = new QAction(tr("Continuar el estudio"));
    connect(restartStudy,SIGNAL(triggered()),this, SLOT(loadStudy()));
    studyMenu->addAction(restartStudy);
    QAction * deleteStudy = new QAction(tr("Borrar el estudio"));
    connect(deleteStudy,SIGNAL(triggered()),this, SLOT(deleteStudy()));
    studyMenu->addAction(deleteStudy);
     studyMenu->setObjectName("studyMenu");
     QLabel * statutLabel = new QLabel(_studies.getState());


    if (_studies.getValue("state").toInt() > -1){
        restartStudy->setDisabled(true);
        deleteStudy->setDisabled(true);
    }

    QPushButton * studyMenuButton = new QPushButton(tr("Opciones"));
    studyMenuButton->setMenu(studyMenu);

    //Set Styles
    date->setObjectName("datetime");
    time->setObjectName("datetime");
    patientName->setObjectName("patient");
    statutLabel->setObjectName("statut");

    QString objectname = "statutPending";

    switch (_studies.getValue("state").toInt()){
    case -1: objectname = "statutPending"; break;
    case 0: objectname = "statutFinished";  statut->setPixmap(QPixmap(":/icon/res/img/Check.png")); break;
    case 1: objectname = "statutConfirmed"; statut->setPixmap(QPixmap(":/icon/res/img/Check.png")); break;
    case 2: objectname = "statutAsigned";  statut->setPixmap(QPixmap(":/icon/res/img/link.png")); break;
    case 3: objectname = "statutDiagnostic";  statut->setPixmap(QPixmap(":/icon/res/img/reported.png")); break;

    }
    statut->setObjectName(objectname);


    studyMenuButton->setObjectName("studieWidgetButton");

    //Set Width
    studyMenu->setFixedWidth(180);
    date->setFixedWidth(100);
    time->setFixedWidth(80);

    statut->setFixedSize(40,40);
    statutLabel->setFixedSize(80,40);
    statutLabel->setAlignment(Qt::AlignCenter);


    /*getInfo = new QPushButton("Retomar");
    if(_studies.getValue("new_report").toInt() == 1)
        getInfo->setIcon(QIcon(":/icon/res/img/historial/info.png"));
    getInfo->setFixedSize(50,50);
    getInfo->setIconSize(QSize(60,60));
    if (_studies.getValue("state").toInt() > -1)
        getInfo->setDisabled(true);*/


    QGridLayout * gd = new QGridLayout(studieWidget);
    gd->addWidget(statut,0,0,Qt::AlignHCenter | Qt::AlignBottom);
    gd->addWidget(statutLabel,1,0,Qt::AlignHCenter | Qt::AlignTop);
    gd->addWidget(patientName,0,1,1,3);

    gd->addWidget(date,1,1,Qt::AlignTop);
    gd->addWidget(time,1,2,Qt::AlignTop);
    gd->addWidget(studydesc,1,3,Qt::AlignTop);

    gd->addWidget(studyMenuButton,0,4,2,1,Qt::AlignRight | Qt::AlignVCenter);

    gd->setContentsMargins(5,2,5,2);
    gd->setSpacing(1);
    gd->setColumnStretch(3,5);
}

void QHistWidget::mouseReleaseEvent(QMouseEvent * event){
}

QHistWidget::~QHistWidget()
{
}

void QHistWidget::deleteStudy(){
    if(QMessageBox::question(this,tr("¿Seguro?"),tr("¿Esta seguro de borrar el estudio?"),QMessageBox::Yes,QMessageBox::No))
    {
        if(studies::deleteStudy(sid))
            emit deletedStudy(this);
        else{
            QMessageBox::warning(this,tr("No se pudo borrar el estudio"),tr("El estudio no se ha podido borrar, por favor reinicie el sistema para poder borrarlo"));
        }
    }
}

void QHistWidget::loadStudy(){
    emit loadStudyId(sid);
}


void QHistWidget::openInfo()
{
    studyInfo *dg = new studyInfo(sid);
    dg->show();
}




