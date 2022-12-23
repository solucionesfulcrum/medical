#include "serieswidget.h"


SeriesWidget::SeriesWidget( QWidget *parent) : QWidget(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    isCapturing = false;
    poller = new QTimer;
    connect(poller,SIGNAL(timeout()),this,SLOT(refreshCaptureTime()));

    /* Recover capture process */
    _captureProcess = captureProcess::_captureProcess;
    connect(_captureProcess,SIGNAL(finished(int)),this,SLOT(processFinished(int)));
    connect(_captureProcess,SIGNAL(started()),this,SLOT(processStarted()));
    connect(_captureProcess,SIGNAL(readyReadStandardOutput()),this,SLOT(processData()));
    connect(_captureProcess,SIGNAL(readyReadStandardError()),this,SLOT(processData()));

    //Create and add Signal to finish study button
    finishStudyButton = new QPushButton(QIcon(":/icon/res/img/capture_button/close.png"),tr("Cerrar")+"\n"+tr("estudio"));
    finishStudyButton->setObjectName("greenButton");
    finishStudyButton->setFixedSize(220,70);
    finishStudyButton->setIconSize(QSize(42,42));
    finishStudyButton->setStyleSheet({"font-size: 18px; font-weight: bold;"}); //JB-06082020
    connect(finishStudyButton,&QPushButton::clicked,this,&SeriesWidget::finishStudy);

    //Create slidinwidget for study capture
    cpt = new SlidingStackedWidget(this);
    createToolBox();
    createListBox();
    createCaptureBox();
    cpt->addWidget(_captureButton);
    cpt->addWidget(toolBox);
    connect(cpt,SIGNAL(animationFinished()),this,SLOT(cptSlideFinished()));

    //Set Windows Capture Widget
    QVBoxLayout * hl = new QVBoxLayout(this);
    //hl->addSpacing(30);
    hl->addWidget(listBox);
    hl->addWidget(cpt,5,Qt::AlignCenter);
    hl->addWidget(finishStudyButton,0,Qt::AlignCenter | Qt::AlignTop);
    hl->addSpacing(80);
    hl->setSpacing(0);
    hl->setMargin(0);
}

SeriesWidget::~SeriesWidget()
{
    if(isCapturing)
        _captureProcess->write("q");
    _captureProcess->killProcess();
}


void SeriesWidget::closeEvent(QCloseEvent *){
}

void SeriesWidget::createListBox(){
    listBox = new QWidget;
    _sweepsline = new sweepsLine();
    QVBoxLayout * lsb = new QVBoxLayout(listBox);
    lsb->setSpacing(0);
    lsb->setMargin(0);
    lsb->setSizeConstraint(QLayout::SetNoConstraint);
    lsb->setAlignment( Qt::AlignTop );
    lsb->addWidget(_sweepsline);

}

void SeriesWidget::initCaptureButton(){
    _captureButton->setTime();
    _captureButton->setTimeMs(0);
    _captureButton->setBlock(false);
    _captureButton->setInfo(tr("Toca para grabar"));
}

void SeriesWidget::setStudy(int i){
    idStudy = i;
    _studies.loadData(i);
    setToolsEnabled(false);
    _sweepsline->setStudy(i);
    cpt->setCurrentIndex(0);
    initCaptureButton();
}


void SeriesWidget::createCaptureBox(){
    _captureButton = new captureButton();
    _captureButton->setInfo(tr("Toca para grabar"));
    connect(_captureButton,SIGNAL(clicked()),this,SLOT(startRecord()));
}

void SeriesWidget::createToolBox()
{
    toolBox = new QWidget;
    resultLabel = new QLabel();
    resultLabel->setAlignment(Qt::AlignCenter);
    resultLabel->setObjectName("toolbox");
    resultLabel->setFixedHeight(60);

    QSize is(42,42);
    int bw = 190;

    sendButton = new sendbutton();    
    sendButton->setStyleSheet({"font-size: 21px; font-weight: bold"});//JB-06082020
    connect(sendButton,SIGNAL(clicked()),this,SLOT(send()));

    QWidget * lb = new QWidget();
    lb->setObjectName("toolbox");
    lb->setFixedHeight(80);

    lookButton = new QToolButton();
    lookButton->setIcon(QIcon(":/icon/res/img/capture_button/play.png"));
    lookButton->setIconSize(is);
    lookButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    lookButton->setText(tr("Ver la")+"\n"+tr("adquisición"));
    lookButton->setObjectName("greenButton");
    lookButton->setFixedSize(380,60);
    lookButton->setStyleSheet({"font-size: 18px; font-weight: bold"});//JB-06082020
    lookButton->setFixedWidth(bw);
    connect(lookButton,SIGNAL(clicked()),this,SLOT(look()));

    restartButton = new QToolButton();
    restartButton->setIcon(QIcon(":/icon/res/img/capture_button/undo.png"));
    restartButton->setIconSize(is);
    restartButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    restartButton->setText(tr("Rehacer")+"\n"+tr("la captura"));
    restartButton->setObjectName("greenButton");
    restartButton->setFixedSize(400,60);
    restartButton->setStyleSheet({"font-size: 18px; font-weight: bold"});//JB-06082020
    restartButton->setFixedWidth(bw);
    connect(restartButton,SIGNAL(clicked()),this,SLOT(restartSerie()));

    QHBoxLayout * llb = new QHBoxLayout(lb);
    llb->addWidget(restartButton);
    llb->addWidget(lookButton);

    QVBoxLayout * toolLayout = new QVBoxLayout(toolBox);
    toolLayout->addWidget(sendButton);
    toolLayout->addWidget(lb);
    toolLayout->setSpacing(5);
    toolLayout->setMargin(0);

    setToolsEnabled(true);
}


bool SeriesWidget::isCapturingVideo(){
    return isCapturing;
}

void SeriesWidget::setActual(int n){
    _sweepsline->setActual(n);
}

void SeriesWidget::setToolsEnabled(bool b){
    sendButton->setEnabled(b);
    lookButton->setEnabled(b);
}

// Start record process
void SeriesWidget::startRecord(){
    if(isCapturing)
        stopRecord();
    else{
        ffmpegErr = "";
        int id_serie = _sweepsline->actualId();
        f = "studies/"+QString::number(idStudy)+"/"+QString::number(id_serie);
        QDir studiesFolder(".");

        //Create Folder of studies (if not exist)
        studiesFolder.mkpath(f);
        _captureButton->setInfo(tr("Iniciando..."));

        //Delete Sweeps if exist
        if (QFile::exists(f+"/"+uncompressedvideoname))
            QFile::remove(f+"/"+uncompressedvideoname);
        _captureProcess->setFolder(f);

        if(_captureProcess->startRecord()){
            finishStudyButton->setDisabled(true);
            _captureButton->setBlock(true);
            isCapturing = true;
            setToolsEnabled(false);
        }
    }
}

void SeriesWidget::processStarted(){
    //QSound::play("sound/start.wav");
    QHash<QString,QString> data;
    int id_serie = _sweepsline->actualId();
    _series.loadData(id_serie);
    int intent = _series.getValue("intent").toInt()+1;
    data.insert("uid",studies::doUID());
    data.insert("starttime",studies::getCurrentDateTime());
    data.insert("intent",QString::number(intent));
    _series.update(data,id_serie);
    _time.restart();
    _captureButton->setRecord(true);
    poller->start(100);
}

void SeriesWidget::stopRecord(){
    if (isCapturing){
        // QSound::play("sound/stop.wav");
        _captureProcess->stopRecord();
        _captureButton->setBlock(true);
        _captureButton->setInfo(tr("Guardando"));
        poller->stop();
        t.setHMS(0,0,0);
        t = t.addMSecs(_time.elapsed());
    }

}

void SeriesWidget::processFinished(int i){
    //Kill Process in case exist
    _captureProcess->killProcess();
    isCapturing = false;
    _captureButton->setRecord(false);
    poller->stop();
    finishStudyButton->setDisabled(false);
    qDebug() << i;

    //i is the exit code of ffmpeg, if 0 exit with success
    if(i == 0){
        /*Check if file exist and is valid*/
        int isValid = validateVideo(f+"/"+uncompressedvideoname);
        if( isValid != VALID_VIDEO ){
            _captureButton->setInfo(tr("Problema con la grabación"));
            _captureButton->setBlock(false);
        }
        else {
            //All is good validate and send
            _captureButton->setInfo(tr("Grabación finalizada"));
            //Update series
            QHash<QString,QString> data;
            data.insert("uid",studies::doUID());
            data.insert("finishtime",studies::getCurrentDateTime());
            data.insert("capture","1");
            _series.update(data,_sweepsline->actualId());
            cpt->slideInNext();
        }
    }
    else {
        //QSound::play("sound/err.wav");
        _captureButton->setInfo(tr("Error de captura"));
        _captureButton->setBlock(false);
        QMessageBox::warning(this,tr("Error de captura"),
                             tr("No se pudo capturar el barrido<br />"
                             "<ul>"
                             "<li>Verifique que el capturador esta bien connectado.</li>"
                             "<li>Contacte su administrador para verificar que el capturador esta bien configurado</li>"
                             "</ul>")
                             );
    }


}

int SeriesWidget::validateVideo(QString filePath){
    int errorCode = 0;

    // verify id the file exists
    if (!QFile::exists(filePath)) {
        errorCode = FILE_NOT_FOUND;
    }
//    else {
//        // execute program for validating video
//        studydesc* protocol = new studydesc();
//        protocol->loadData(_studies.getValue("id_protocols").toInt());
//        QString program = "py main.cpython-39.pyc " + filePath + " " + protocol->getValue("name").toString();

//        QProcess* validate = new QProcess;
//        validate->start(program);
//        qDebug() << "Start Video Validation" << program;
//        qDebug() << "Result:" << validate->waitForFinished(60000);
//        QString output(validate->readAllStandardOutput());
//        qDebug() << "Output:" << output;
//        delete validate;
//        delete protocol;
//        errorCode = INVALID_PROTOCOL;
//    }

    // execute validation program
    return errorCode;
}

void SeriesWidget::processData(){
    QByteArray dt(QDateTime::currentDateTime().toString().toStdString().c_str());
    QByteArray iErr = _captureProcess->readAllStandardError();
    QByteArray iOut = _captureProcess->readAllStandardOutput();
    dt = "["+dt+"] Study " + QString::number(idStudy).toStdString().c_str()+ ": ";

    if(!iErr.isEmpty()){
        ffmpegErr += QString(iErr);
        QFile errorFile("error.logs");
        if (errorFile.open(QIODevice::WriteOnly | QIODevice::Append)) {
            errorFile.write(dt+iErr);
            errorFile.close();
        }
    }

    if(!iOut.isEmpty()){
        QFile logsFile("logs.logs");
        if (logsFile.open(QIODevice::WriteOnly | QIODevice::Append)) {
            logsFile.write(dt+iOut);
            logsFile.close();
        }
    }
}

void SeriesWidget::look(){
    config conf;
    QString folder =  QDir::currentPath()+"/"+f;
    QString device = folder+"/"+uncompressedvideoname;
    QString p = "ffplay  "+myffplay::basicOption+" -f rawvideo -pix_fmt [PIXELCONF] -video_size [SIZE] -framerate [FPS] "+device;
    p = p.replace("[FPS]",conf.getValue("fps").toString());
    p = p.replace("[SIZE]",conf.getValue("SIZE").toString());
    p = p.replace("[PIXELCONF]",conf.getValue("PIXELCONF").toString());
    myffplay::player->start(p);

//---------------------------------------------
//  Christiam
    myffplay::player->waitForFinished();
//---------------------------------------------

}
void SeriesWidget::send(){
    int id = _sweepsline->actualId();
    emit sendToQueue(id);

    _captureButton->setTime();
    _captureButton->setTimeMs(0);
    _captureButton->setInfo(tr("Toca para grabar"));
    _captureButton->setBlock(false);
    setToolsEnabled(false);
    sendButton->setEnabled(false);


    if (!_sweepsline->isLast()){
        cpt->slideInPrev();
        _sweepsline->next();
    }
    else {
        QHash<QString,QString> data;
        data.insert("finishtime",studies::getCurrentDateTime());
        data.insert("state","0");
        //_studies.UpdateLastElement(data);
        _studies.update(data,idStudy);
        if (QMessageBox::question(this,tr("¿Nuevo Estudio?"),tr("¿Empezar un nuevo estudio con el mismo paciente?"),QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
            emit finishedStudy(true);
        else
            emit finishedStudy(false);
    }
}

void SeriesWidget::finishStudy(){
    _captureProcess->kill();
    emit finished(false);
}


void SeriesWidget::refreshCaptureTime(){
    QTime t(0,0,0);
    t = t.addMSecs(_time.elapsed());
    if(_time.elapsed() > 1000 && _captureButton->isBlocked())
        _captureButton->setBlock(false);
    _captureButton->setTime(t.toString("mm:ss"));
    _captureButton->setInfo(tr("Grabación"));
    _captureButton->setTimeMs(_time.elapsed());
}

void SeriesWidget::restartSerie(){
    if (QMessageBox::question(this,tr("¿Reiniciar la adquisición?"),tr("¿Esta seguro de realizar nuevamente la adquisicón?"),QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
    {
        initCaptureButton();
        cpt->slideInPrev();
    }
}


void SeriesWidget::cptSlideFinished(){
    int index =  cpt->currentIndex();
    if(index == 0){ //Capture


    }
    if(index == 1){ //Sender
        //Activate Tools
        setToolsEnabled(true);
        //Show Time passed and enabled button
        QTime t(0,0,0);
        t = t.addMSecs(_time.elapsed());
        sendButton->setTime(t.toString());
        sendButton->setEnabled(true);
    }
}



