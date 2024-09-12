#include "serieswidget.h"


SeriesWidget::SeriesWidget( QWidget *parent) : QWidget(parent)
{
    protocol_id = 0;

    setAttribute(Qt::WA_DeleteOnClose);
    isCapturing = false;
    poller = new QTimer;
    connect(poller,SIGNAL(timeout()),this,SLOT(refreshCaptureTime()));

//  Recover capture process
    _captureProcess = captureProcess::_captureProcess;
    connect(_captureProcess,SIGNAL(finished(int)),this,SLOT(processFinished(int)));
    connect(_captureProcess,SIGNAL(started()),this,SLOT(processStarted()));
    connect(_captureProcess,SIGNAL(readyReadStandardOutput()),this,SLOT(processData()));
    connect(_captureProcess,SIGNAL(readyReadStandardError()),this,SLOT(processData()));

//  Create and add Signal to finish study button
    finishStudyButton = new QPushButton(QIcon(":/icon/res/img/capture_button/close.png"),tr("Cerrar")+"\n"+tr("estudio"));
    finishStudyButton->setObjectName("greenButton");
    finishStudyButton->setFixedSize(220,70);
    finishStudyButton->setIconSize(QSize(42,42));
    finishStudyButton->setStyleSheet({"font-size: 18px; font-weight: bold;"}); //JB-06082020
    connect(finishStudyButton,&QPushButton::clicked,this,&SeriesWidget::finishStudy);

    sendStudyButton = new QPushButton(tr("Enviar")+"\n"+tr("estudio"));
    sendStudyButton->setObjectName("greenButton");
    sendStudyButton->setStyleSheet({"font-size: 18px; font-weight: bold;"});
    sendStudyButton->setFixedSize(220,70);
    sendStudyButton->setEnabled(false);
    connect(sendStudyButton,&QPushButton::clicked,this,&SeriesWidget::sendStudy);


    //Create slidinwidget for study capture
    cpt = new SlidingStackedWidget(this);
    createToolBox();
    createListBox();
    createCaptureBox();
    cpt->addWidget(_captureButton);
    cpt->addWidget(toolBox);
    connect(cpt,SIGNAL(animationFinished()),this,SLOT(cptSlideFinished()));

    StudiesFinished = false;
/*
    //Set Windows Capture Widget
    QVBoxLayout * hl = new QVBoxLayout(this);
    //hl->addSpacing(30);
    hl->addWidget(listBox);
    hl->addWidget(cpt,5,Qt::AlignCenter);
    hl->addWidget(finishStudyButton,0,Qt::AlignCenter | Qt::AlignTop);
    hl->addWidget(sendStudyButton,0,Qt::AlignCenter | Qt::AlignTop);
    hl->addSpacing(80);
    hl->setSpacing(0);
    hl->setMargin(0);
    */

    QGridLayout * hl = new QGridLayout(this);
    hl->addWidget(listBox,0,0,1,5,Qt::AlignCenter);
    hl->addWidget(cpt,1,0,5,5,Qt::AlignCenter);
    hl->addWidget(finishStudyButton,6,1,1,1,Qt::AlignTop|Qt::AlignRight);
    hl->addWidget(sendStudyButton,6,3,1,1,Qt::AlignTop|Qt::AlignLeft);
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
    /*
    listBox = new QWidget;
    _sweepsline = new sweepsLine();
    QVBoxLayout * lsb = new QVBoxLayout(listBox);
    lsb->setSpacing(0);
    lsb->setMargin(0);
    lsb->setSizeConstraint(QLayout::SetNoConstraint);
    lsb->setAlignment( Qt::AlignTop );
    lsb->addWidget(_sweepsline);*/

    listBox = new QWidget;
    _sweepsline = new sweepsLine();

    backButton = new QPushButton(tr("Atrás"));
    backButton->setObjectName("greenButton");
    backButton->setStyleSheet({"font-size: 18px; font-weight: bold;"});
    backButton->setFixedSize(200,60);
    backButton->setEnabled(true);
    connect(backButton,SIGNAL(clicked()),this,SLOT(backButtonSlot()));

    nextButton = new QPushButton(tr("Siguiente"));
    nextButton->setObjectName("greenButton");
    nextButton->setStyleSheet({"font-size: 18px; font-weight: bold;"});
    nextButton->setFixedSize(200,60);
    nextButton->setEnabled(true);
    connect(nextButton,SIGNAL(clicked()),this,SLOT(nextButtonSlot()));

    QGridLayout *lsb = new QGridLayout(listBox);
    //lsb->setSpacing(0);
    //lsb->setMargin(0);
    lsb->setSizeConstraint(QLayout::SetNoConstraint);
    lsb->setAlignment(Qt::AlignTop);

    /*lsb->addWidget(backButton,Qt::AlignRight);
    lsb->addWidget(_sweepsline);
    lsb->addWidget(nextButton,Qt::AlignLeft);*/

//---------------------------------------------------
//  CR: 02/07/23
    _sweepsline->setFixedSize(800,150);
//---------------------------------------------------

    lsb->addWidget(backButton,1,0);
    lsb->addWidget(_sweepsline,0,1);
    lsb->addWidget(nextButton,1,2);

}

void SeriesWidget::backButtonSlot()
{
    int i = _sweepsline->actual();
    if(i==0)    return;

    sendButton->setEnabled(false);

    emit changePicture(protocol_id,i);
    _sweepsline->prev();


    int id_serie = _sweepsline->actualId();
    _series.loadData(id_serie);
    quint64 startime = _series.getValue("starttime").toULongLong();
    quint64 finishTime = _series.getValue("finishtime").toULongLong();
    QTime t(0,0,0);
    t = t.addMSecs(finishTime-startime);
    sendButton->setTime(t.toString());
    sendButton->setInfo(tr("Grabación \nFinalizada"));
    sendButton->update();



    cpt->slideInNext();


}

void SeriesWidget::nextButtonSlot()
{
    int i,j;

    i = _sweepsline->actual()+1;
    j = _sweepsline->getSweepsCompleted();

    if(i==j)    return;

    emit changePicture(protocol_id,i+1);
    _sweepsline->next();


    //emit changePicture(protocol_id,i);

    /*

    if((i+1)==j)
    {
        cpt->slideInPrev();
        sendButton->setEnabled(true);
    }
    else
    {
        int id_serie = _sweepsline->actualId();
        _series.loadData(id_serie);
        quint64 startime = _series.getValue("starttime").toULongLong();
        quint64 finishTime = _series.getValue("finishtime").toULongLong();
        QTime t(0,0,0);
        t = t.addMSecs(finishTime-startime);
        sendButton->setTime(t.toString());
        sendButton->update();

        cpt->slideInNext();
    }
    */

    if((i+1)<=j)
    {

        int id_serie = _sweepsline->actualId();
        _series.loadData(id_serie);
        quint64 startime = _series.getValue("starttime").toULongLong();
        quint64 finishTime = _series.getValue("finishtime").toULongLong();
        QTime t(0,0,0);
        t = t.addMSecs(finishTime-startime);
        sendButton->setTime(t.toString());
        sendButton->setInfo(tr("Grabación \nFinalizada"));
        sendButton->update();

        sendButton->setEnabled(true);

        cpt->slideInNext();

    }



}

void SeriesWidget::initCaptureButton(){
    _captureButton->setTime();
    _captureButton->setTimeMs(0);
    _captureButton->setBlock(false);
    _captureButton->setInfo(tr("Toca para grabar"));
}


// CR: 01/02/23
void SeriesWidget::setStudy(int i){

    idStudy = i;
    _studies.loadData(i);
    setToolsEnabled(false);

    cpt->setCurrentIndex(0);
    initCaptureButton();

    backButton->setEnabled(false);
    nextButton->setEnabled(false);

    if(_sweepsline->setStudy(i)==true){
        sendButton->text = tr("Pasos\nTerminados");
        sendButton->update();
        sendButton->setEnabled(false);

        sendStudyButton->setEnabled(true);
        backButton->setEnabled(true);
        nextButton->setEnabled(true);
        lookButton->setEnabled(true);
        StudiesFinished = true;


        _captureButton->setTime("");
        _captureButton->setTimeMs(0);
        _captureButton->setInfo(tr(""));
        cpt->slideInNext();



    }



/*
    idStudy = i;
    _studies.loadData(i);
    setToolsEnabled(false);

    if(_sweepsline->setStudy(i)==true){
        sendButton->text = "Paso\nTerminado";
        sendButton->update();
        sendButton->setEnabled(false);

        //_sweepsline->next();
        sendStudyButton->setEnabled(true);
        backButton->setEnabled(true);
        nextButton->setEnabled(true);
        lookButton->setEnabled(true);        
    }

    if(_sweepsline->isLast())
    {
        sendButton->text = "Paso\nTerminado";
        sendButton->update();
        sendButton->setEnabled(false);
        cpt->slideInNext();
    }

    cpt->setCurrentIndex(0);
    initCaptureButton();


    if(_sweepsline->IsCompleted()){
        sendButton->text = "Paso\nTerminado";
        sendButton->update();
        sendButton->setEnabled(false);
        cpt->slideInNext();
        _captureButton->setTime("");
        _captureButton->setTimeMs(0);
        _captureButton->setInfo(tr(""));

    }
*/
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
    sendButton->setStyleSheet({"font-size: 17px; font-weight: bold"});//JB-06082020
    sendButton->text = tr("Siguiente\nPaso");
    sendButton->update();

    connect(sendButton,SIGNAL(clicked()),this,SLOT(send()));

    QWidget * lb = new QWidget();
    lb->setObjectName("toolbox");
    lb->setFixedHeight(80);

    lookButton = new QToolButton();
    lookButton->setIcon(QIcon(":/icon/res/img/capture_button/play.png"));
    lookButton->setIconSize(is);
    lookButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    lookButton->setText(tr("Visualizar")+"\n"+tr("el paso"));
    lookButton->setObjectName("greenButton");
    lookButton->setFixedSize(380,60);
    lookButton->setStyleSheet({"font-size: 18px; font-weight: bold"});//JB-06082020
    lookButton->setFixedWidth(bw);
    connect(lookButton,SIGNAL(clicked()),this,SLOT(look()));

    restartButton = new QToolButton();
    restartButton->setIcon(QIcon(":/icon/res/img/capture_button/undo.png"));
    restartButton->setIconSize(is);
    restartButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    restartButton->setText(tr("Rehacer")+"\n"+tr("el paso"));
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
            backButton->setDisabled(true);
            nextButton->setDisabled(true);
            sendStudyButton->setDisabled(true);
            finishStudyButton->setDisabled(true);
            _captureButton->setBlock(true);
            isCapturing = true;
            setToolsEnabled(false);
        }
    }
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


        if (_sweepsline->isLast())
        {
            sendButton->text = tr("Pasos\nTerminados");

            sendButton->update();
            sendButton->setEnabled(false);

             _captureButton->setInfo(tr(""));
             _captureButton->setTime(0);

            //_sweepsline->next();
            sendStudyButton->setEnabled(true);
            //backButton->setEnabled(true);
            //nextButton->setEnabled(true);
            lookButton->setEnabled(true);
            StudiesFinished = true;

        }

        nextButton->setEnabled(true);
        backButton->setEnabled(true);


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



void SeriesWidget::processFinished(int i){
//  Kill Process in case exist
    _captureProcess->killProcess();
    isCapturing = false;
    _captureButton->setRecord(false);
    poller->stop();
    finishStudyButton->setDisabled(false);
    qDebug() << i;

//  i is the exit code of ffmpeg, if 0 exit with success
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

        //  CR: 01/02/23
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

    int id_serie = _sweepsline->actualId();
    QString ff = "studies/"+QString::number(idStudy)+"/"+QString::number(id_serie);
    QString folder =  QDir::currentPath()+"/"+ff;

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
/*
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
*/
    if(_sweepsline->isLast())   return;

    int i,j;
    i = _sweepsline->actual()+1;
    j = _sweepsline->getSweepsCompleted();



    if(i==j)
    {
        _captureButton->setTime();
        _captureButton->setTimeMs(0);
        _captureButton->setInfo(tr("Toca para grabar"));
        _captureButton->setBlock(false);
        backButton->setDisabled(true);
        nextButton->setDisabled(true);
        sendStudyButton->setDisabled(true);
        cpt->slideInPrev();
        _sweepsline->next();

        emit changePicture(protocol_id,i+1);



    }
    else    return;
    /*
    {
        backButton->setEnabled(true);
        nextButton->setEnabled(true);
        sendButton->text = "Siguiente\nPaso";
        cpt->slideInPrev();
        _sweepsline->next();

    }*/



//-----------------------------------------------------
//  CR: 23/01/23

    //setToolsEnabled(false);





/*
    if (!_sweepsline->isLast()){
        backButton->setEnabled(true);
        nextButton->setEnabled(true);
        sendButton->text = "Siguiente\nPaso";
        cpt->slideInPrev();
        _sweepsline->next();

    }
*/




    /*
    else {
        sendButton->text = "Paso\nTerminado";
        sendButton->update();
    //-------------------------------------------------------
    //  CR: 07/02/23
        sendButton->setEnabled(false);
         _captureButton->setInfo(tr(""));
         _captureButton->setTime(0);
    //-------------------------------------------------------
        _sweepsline->next();
        sendStudyButton->setEnabled(true);
        backButton->setEnabled(true);
        nextButton->setEnabled(true);
        lookButton->setEnabled(true);
        StudiesFinished = true;
    }*/

    /*
    if(_sweepsline->IsCompleted()){
        sendButton->text = "Paso\nTerminado";
        sendButton->update();
        sendButton->setEnabled(false);
         _captureButton->setInfo(tr(""));
         _captureButton->setTime(0);
        _sweepsline->next();
        sendStudyButton->setEnabled(true);
        backButton->setEnabled(true);
        nextButton->setEnabled(true);
        lookButton->setEnabled(true);
    }
    */


//-----------------------------------------------------
}

//-----------------------------------------------------
// CR: 23/01/23
void SeriesWidget::sendStudy(void){

    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("Envio de estudio"));
    msgBox.setText(tr("¿Esta seguro de enviar el estudio?"));
    // Cambiar el texto de los botones
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setButtonText(QMessageBox::Yes, tr("Sí"));
    msgBox.setButtonText(QMessageBox::No, tr("No"));
    msgBox.setIcon(QMessageBox::Question);
    if (msgBox.exec() == QMessageBox::Yes)
    //if (QMessageBox::question(this,tr("Envio de estudio"),tr("¿Esta seguro de enviar el estudio?"),QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
    {        
        for(uint8_t i=0;i<_sweepsline->sweepsSize();i++){
            int id = _sweepsline->getActual(i);
            emit sendToQueue(id);            
        }

        QHash<QString,QString> data;
        data.insert("finishtime",studies::getCurrentDateTime());
        data.insert("state",state_ontransmission);
        _studies.update(data,idStudy);

        emit changePicture(0,0);

        QMessageBox msgBox2;
        msgBox2.setWindowTitle(tr("¿Nuevo Estudio?"));
        msgBox2.setText(tr("¿Empezar un nuevo estudio con el mismo paciente?"));
        // Cambiar el texto de los botones
        msgBox2.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox2.setButtonText(QMessageBox::Yes, tr("Sí"));
        msgBox2.setButtonText(QMessageBox::No, tr("No"));
        msgBox2.setIcon(QMessageBox::Question);
        if (msgBox2.exec() == QMessageBox::Yes)
        //if (QMessageBox::question(this,tr("¿Nuevo Estudio?"),tr("¿Empezar un nuevo estudio con el mismo paciente?"),QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
            emit finishedStudy(true);
        else
            emit finishedStudy(false);

        backButton->setDisabled(true);
        nextButton->setDisabled(true);
        sendStudyButton->setDisabled(true);
        StudiesFinished = false;
    }
}
//-----------------------------------------------------

void SeriesWidget::finishStudy(){


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

    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("¿Reiniciar el paso?"));
    msgBox.setText(tr("¿Esta seguro de realizar nuevamente el paso?"));
    // Cambiar el texto de los botones
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setButtonText(QMessageBox::Yes, tr("Sí"));
    msgBox.setButtonText(QMessageBox::No, tr("No"));
    msgBox.setIcon(QMessageBox::Question);
    if (msgBox.exec() == QMessageBox::Yes)
    //if (QMessageBox::question(this,tr("¿Reiniciar el paso?"),tr("¿Esta seguro de realizar nuevamente el paso?"),QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
    {        
        backButton->setDisabled(true);
        nextButton->setDisabled(true);
        sendStudyButton->setDisabled(true);
        initCaptureButton();
        cpt->slideInPrev();
    }
}


void SeriesWidget::cptSlideFinished(){
    int index =  cpt->currentIndex();
    if(index == 0){ //Capture


    }
    if(index == 1){ //Sender
    //  Activate Tools
        setToolsEnabled(true);

    //  Show Time passed and enabled button
        int id_serie = _sweepsline->actualId();
        _series.loadData(id_serie);
        quint64 startime = _series.getValue("starttime").toULongLong();
        quint64 finishTime = _series.getValue("finishtime").toULongLong();
        QTime t(0,0,0);
        t = t.addMSecs(finishTime-startime);
        sendButton->setInfo(tr("Grabación \nFinalizada"));
        sendButton->setTime(t.toString());

        if(StudiesFinished){
            sendButton->setEnabled(false);
            sendStudyButton->setEnabled(true);
        }
        else
            sendButton->setEnabled(true);

        /*if(_sweepsline->IsCompleted())
            sendButton->setEnabled(false);
        else
            sendButton->setEnabled(true);*/
    }



}



