#include "configuration.h"

void cleaner::setList(QStringList l){
    ids.clear();
    ids = l;
}

void cleaner::run(){
    int x = 0;
    foreach(QString str, ids){

        QDir d("studies/"+str);
        bool removed = d.removeRecursively();
        qDebug() << str << removed;
        if (removed){
            _studies.execute("DELETE FROM studies WHERE id = "+str);
            _studies.execute("DELETE FROM series WHERE id_studies = "+str);
        }
        x++;
        emit progress(x,ids.size());
    }
}

configuration::configuration(QMedicalBoxWidget *parent) : QMedicalBoxWidget(parent)
{
    createQLabels();
    setInputs();
    setServerForm();
    setAcquisitionForm();
    setCompressionForm();
    setMaintenanceForm();
    saveButton = new QPushButton(tr("Guardar"),this);
    saveButton->setObjectName("greenButton");
    saveButton->setFixedWidth(200);
    connect(saveButton,SIGNAL(clicked()),this,SLOT(save()));

    load();

    configWidget = new QTabWidget;
    configWidget->setObjectName("configWidget");
    configWidget->setContentsMargins(10,10,10,10);
    mainLayout->addWidget(configWidget,3);
    mainLayout->addWidget(saveButton,0,Qt::AlignCenter);
    mainLayout->setMargin(30);

}

configuration::~configuration()
{
}

void configuration::refreshOpe(){
    for(int x=0; x<configWidget->count(); x++)
        configWidget->removeTab(x);

    if(operators::isAdmin()){
        configWidget->addTab(_serverForm,tr("Servidor"));
        configWidget->addTab(_acquisitionForm,tr("Adquisición"));
        configWidget->addTab(_compressionForm,tr("Compresión"));
        configWidget->addTab(_maintenanceForm,tr("Mantenimiento"));
    }
    else {
//        configWidget->addTab(_acquisitionForm,tr("Adquisición"));
        configWidget->addTab(_maintenanceForm,tr("Mantenimiento"));
    }
}

void configuration::setInputs(){
    _name = new QVkLineEdit;
    _pass = new QVkLineEdit;
    _serverIp = new TouchComboBox;
    //_serverIp->setItems(QStringList() << "http://158.69.95.75" << "http://158.69.86.81" << "http://40.74.246.61");
    _serverIp->setItems(_config.serverList());
    _modelUS = new QVkLineEdit;

    equipo = new TouchComboBox;
    QStringList  hw;
    //Liste source video
    QList<QByteArray> listaCamaras = cam.availableDevices();
    if ( listaCamaras.size() > 0 ) {
        for (int i=0; i<listaCamaras.size(); i++) {
            hw << cam.deviceDescription(listaCamaras.at(i));
        }
    }
    else hw << tr("No se ha detectado ningún dispositivo conectado");
    equipo->setItems(hw);




    QStringList  inputNumbers;
    for (int x = -1; x < 100; x+=1)
        inputNumbers << QString::number(x);
    _videoInput = new TouchComboBox;
    _videoInput->setItems(inputNumbers);

    _fps = new TouchComboBox;
    _fps->setItems(QStringList() << tr("10") << tr("30"));

    _size = new TouchComboBox;
    _size->setItems(QStringList() << "864x768" << "640x480" << "768x576" << "800x480" << "800x600" << "854x480" << "1024x576" << "1024x600" << "1024x768" << "1152x768" << "1152x864" << "1280x720" << "1280x768" << "1280x8002" << "1280x854" << "1280x960" << "1280x1024" << "1366x768" << "1400x1050" << "1440x960" << "1440x1080" << "1600x900" << "1680x1050" << "1920x1080" << "1920x1200");

    _pixelConf = new TouchComboBox;
    _pixelConf->setItems(QStringList() << tr("yuv420p"));

    _keep_uncompressed = new QCheckBox();

    QStringList  bitrate;
    for (int x = 200; x < 2000; x+=100)
        bitrate << QString::number(x);
    _defaultvideobitrate = new TouchComboBox;
    _defaultvideobitrate->setItems(bitrate);
    _vbitratemin = new TouchComboBox;
    _vbitratemin->setItems(bitrate);
    _vbitratemax = new TouchComboBox;
    _vbitratemax->setItems(bitrate);

    QList<QLineEdit*> editline;
    editline << _name
             << _pass
             << _serverIp
             << _modelUS
             << _defaultvideobitrate
             << _videoInput
             << _fps
             << _size
             << _pixelConf
             << _vbitratemin
             << _vbitratemax
             << equipo;

    connect(_keep_uncompressed,SIGNAL(toggled(bool)),this,SLOT(isChanged(bool)));
    foreach (QLineEdit * edt, editline)
        connect(edt,SIGNAL(textChanged(QString)),this,SLOT(isChanged(QString)));

}

void configuration::createQLabels(){
    labelList << new QLabel(tr("IP"))
              << new QLabel(tr("Nombre"))
              << new QLabel(tr("Contraseña"))
              << new QLabel(tr("Video Input"))
              << new QLabel(tr("Video Bitrate (default)"))
              << new QLabel(tr("FPS"))
              << new QLabel(tr("Tamaño"))
              << new QLabel(tr("Pixel Conf"))
              << new QLabel(tr("Video Bitrate mínimo"))
              << new QLabel(tr("Video Bitrate máximo"))
              << new QLabel(tr("Modelo del Ecógrafo"))
              << new QLabel(tr("Guardar adquisición original"))
              << new QLabel(tr("Capturador"))
                 ;

    foreach(QLabel *l, labelList)
        l->setFixedWidth(300);
}

void configuration::save()
{
    QHash<QString,QString> data;
    data.insert("ip_server",_serverIp->text());
    data.insert("VIDEOINPUT",_videoInput->text());
    data.insert("name",_name->text());
    data.insert("pass",_pass->text());
    data.insert("DEFAULTVIDEOBITRATE",_defaultvideobitrate->text());
    data.insert("fps",_fps->text());
    data.insert("SIZE",_size->text());
    data.insert("PIXELCONF",_pixelConf->text());
    data.insert("VBITRATEMIN",_vbitratemin->text());
    data.insert("VBITRATEMAX",_vbitratemax->text());
    data.insert("modelUS",_modelUS->text());
    data.insert("device",equipo->text());
    if(_keep_uncompressed->isChecked())
        data.insert("keep_uncompressed","1");
    else
        data.insert("keep_uncompressed","0");
    _config.update(data);
    changed = false;
    saveButton->setEnabled(false);
    QMessageBox::information(this,tr("Configuración guardada"),tr("La configuración ha sido guardada."));
}

void configuration::load(){
    _pass->setText(_config.getValue("pass").toString());
    _name->setText(_config.getValue("name").toString());
    _serverIp->setText(_config.getValue("ip_server").toString());
    _videoInput->setText(_config.getValue("VIDEOINPUT").toString());
    _defaultvideobitrate->setText(_config.getValue("DEFAULTVIDEOBITRATE").toString());
    _fps->setText(_config.getValue("fps").toString());
    _size->setText(_config.getValue("SIZE").toString());
    _pixelConf->setText(_config.getValue("PIXELCONF").toString());
    _vbitratemin->setText(_config.getValue("VBITRATEMIN").toString());
    _vbitratemax->setText(_config.getValue("VBITRATEMAX").toString());
    _modelUS->setText(_config.getValue("modelUS").toString());
    equipo->setText(_config.getValue("device").toString());
    if(_config.getValue("keep_uncompressed").toInt() == 0)
        _keep_uncompressed->setChecked(false);
    else _keep_uncompressed->setChecked(true);

    changed = false;
    saveButton->setEnabled(false);
}

void configuration::setLayoutForm(QLayout * fl){
    fl->setMargin(20);
    fl->setAlignment(Qt::AlignTop);
    fl->setContentsMargins(200,50,200,0);
    fl->setSpacing(1);
}


void configuration::setServerForm()
{
    _serverForm = new QWidget;
    _serverForm->setObjectName("form");
    QVBoxLayout * fl = new  QVBoxLayout(_serverForm);
    setLayoutForm(fl);

    fl->addWidget(labelList.at(IP));
    fl->addWidget(_serverIp);
    fl->addSpacing(15);
    fl->addWidget(labelList.at(NOMBRE));
    fl->addWidget(_name);
    fl->addSpacing(15);
    fl->addWidget(labelList.at(PASS));
    fl->addWidget(_pass);
    fl->addSpacing(15);
    fl->addWidget(labelList.at(MODELUS));
    fl->addWidget(_modelUS);
}

void configuration::setAcquisitionForm()
{
    _acquisitionForm = new QWidget;
    _acquisitionForm->setObjectName("form");
    QVBoxLayout * fl = new  QVBoxLayout(_acquisitionForm);
    setLayoutForm(fl);

    fl->addWidget(labelList.at(VIDEODEVICE));
    fl->addWidget(equipo);
    fl->addSpacing(15);

    fl->addWidget(labelList.at(VIDEOINPUT));
    fl->addWidget(_videoInput);
    fl->addSpacing(15);

    fl->addWidget(labelList.at(FPS));
    fl->addWidget(_fps);
    fl->addSpacing(15);

    fl->addWidget(labelList.at(VSIZE));
    fl->addWidget(_size);
    fl->addSpacing(15);

    fl->addWidget(labelList.at(PIXCONF));
    fl->addWidget(_pixelConf);
    fl->addSpacing(15);

    fl->addWidget(labelList.at(KEEP_UNCOMPRESSED));
    fl->addWidget(_keep_uncompressed);
    fl->addSpacing(15);
}

void configuration::setCompressionForm()
{
    _compressionForm = new QWidget;
    _compressionForm->setObjectName("form");
    QVBoxLayout * fl = new  QVBoxLayout(_compressionForm);
    setLayoutForm(fl);
    fl->addWidget(labelList.at(VBITRATE));
    fl->addWidget(_defaultvideobitrate);
    fl->addSpacing(15);

    fl->addWidget(labelList.at(VBITRATEMIN));
    fl->addWidget(_vbitratemin);
    fl->addSpacing(15);

    fl->addWidget(labelList.at(VBITRATEMAX));
    fl->addWidget(_vbitratemax);
    fl->addSpacing(15);
}

void configuration::setMaintenanceForm()
{

    QSize bsize(200,200);
    QPushButton * videoButton = new QPushButton(tr("Probar la captura"));
    connect(videoButton,SIGNAL(clicked()),this,SLOT(checkVideo()));
    videoButton->setFixedSize(bsize);

    cleanButton = new QPushButton(tr("Borrar TODOS los")+"\n"+tr("estudios pendientes"));
    connect(cleanButton,SIGNAL(clicked()),this,SLOT(cleanStudies()));
    cleanButton->setFixedSize(bsize);

    QPushButton * setvideoButton = new QPushButton(tr("Configuración")+"\n"+tr("del video"));
    connect(setvideoButton,SIGNAL(clicked()),this,SLOT(setVideoDevice()));
    setvideoButton->setFixedSize(bsize);

    _maintenanceForm = new QWidget;
    _maintenanceForm->setObjectName("form");
    //_maintenanceForm->setWindowModality(Qt::ApplicationModal);
    QGridLayout * fl = new  QGridLayout(_maintenanceForm);
    setLayoutForm(fl);
    fl->setSpacing(30);
    fl->addWidget(videoButton,0,0);

    fl->addWidget(setvideoButton,0,1);
    fl->addWidget(cleanButton,0,2);

    if(operators::isAdmin())
    {
        setvideoButton->setHidden(false);
    }
    else
    {
        setvideoButton->setHidden(true);
    }

}

void configuration::checkVideo(){
    QString device;

    device=CheckAdquisitionDevice();
    qDebug()<<"Dispositivo: "<<device;
    if (device==""){
        QMessageBox::information(this,tr("Dispositivo de captura"),QString::fromUtf8("No se encontro dispositivo de captura válido\nVerifique conexiones."),QMessageBox::Ok);
        return;
    }

    config cf;
//    QString comando = "ffplay "+myffplay::basicOption+"  -f dshow -crossbar_video_input_pin_number "+cf.getValue("VIDEOINPUT").toString()+" -i video=\""+device+"\"";
//  -x ancho de la ventana donde se va a mostrar
//  -y alto de la ventana donde se va a mostrar
//  -autoexit 'Exit when video is done playing'
//  -f dshow muestra el video
//  -video_size 1024x768 Formato del video a digitalizar
//  -exitonmousedown sale al presionar boton del mouse

//    QString comando = "ffplay -x 640 -y 480 -autoexit -f dshow -video_size 1024x768 -i video=\""+device+"\"";
    QString comando = "ffplay -x 640 -y 480 -f dshow -window_title Video_Test -exitonmousedown -video_size 1024x768 -i video=\""+device+"\"";
    RunCmd(comando);
    return;

    myffplay::player->start("ffplay "+myffplay::basicOption+"  -f dshow -crossbar_video_input_pin_number "+cf.getValue("VIDEOINPUT").toString()+" -i video=\""+cf.getValue("device").toString()+"\"");


    // qDebug() << "ffplay "+myffplay::basicOption+"  -f dshow -crossbar_video_input_pin_number "+cf.getValue("VIDEOINPUT").toString()+" -i video=\""+cf.getValue("device").toString()+"\"";
    // -fs => fullscreen
    //videoplayer->start("ffplay -x 1024 -y 570 -autoexit -exitonmousedown -f dshow  -crossbar_video_input_pin_number 1 -i video=\"OEM Device\" ");
}


void configuration::setVideoDevice(){

    QString device;

    device=CheckAdquisitionDevice();
    qDebug()<<"Dispositivo: "<<device;
    if (device==""){
        QMessageBox::information(this,tr("Dispositivo de captura"),QString::fromUtf8("No se encontro dispositivo de captura válido\nVerifique conexiones."),QMessageBox::Ok);
        return;
    }


    QString comando = "ffmpeg  -f dshow -show_video_device_dialog true -crossbar_video_input_pin_number 1 -i video=\""+device+"\"";
    RunCmd(comando);
    return;


/*
    qDebug()<<"Previo llamada hilo desde padre";//<< app.thread()->currentThreadId();
    proceso.start();
    qDebug()<<"Despues llamada hilo desde padre";//<< app.thread()->currentThreadId();
    while (proceso.isRunning())
    {
        qDebug()<<"x";
        Sleep(500);
    }
//    proceso.wait();
    qDebug()<<"Fin llamada hilo desde padre";//<< app.thread()->currentThreadId();

    return;
*/


//    config cf1;
//    QString comando = "ffmpeg  -f dshow -show_video_device_dialog true -crossbar_video_input_pin_number 1 -i video=\""+cf1.getValue("device").toString()+"\"";

//    proceso.Argumento(comando);
//    qDebug()<<"Previo llamada hilo desde padre";//<< app.thread()->currentThreadId();
//    proceso.start();

//    Sleep(1000);
//    accesor::mw->showMinimized();


//    HWND hWnd = ::FindWindow(NULL,L"Propiedades de 0001");
//    HWND hWnd = ::FindWindow(L"PropiedadTest",NULL);
//    qDebug()<<"hWnd="<<hWnd;


//    Qt::WindowFlags flags = windowFlags();
//    this->setWindowFlags((flags | Qt::WindowStaysOnTopHint));
//    this->showMaximized();
//    this->setWindowFlags(flags);
//    this->showMinimized();

//    accesor::mw->setHidden(true);

//    while (proceso.isRunning())
//    {
//        QCoreApplication::processEvents();
//        ShowWindow(hWnd,SW_SHOW);
//        this->setWindowFlags(Qt::WindowStaysOnTopHint);
//        this->activateWindow();
//        this->clearFocus();
//        this->blockSignals(true);
//        this->hide();
//        this->setHidden(true);
//        this->setVisible(false);
//        this->setWindowOpacity(128);
//        this->showMinimized();

//    }
//    qDebug()<<"Fin llamada hilo desde padre";//<< app.thread()->currentThreadId();
//    entry=0;
//    accesor::mw->showMaximized();
//    accesor::mw->setEnabled(true);
//    return;



    //QProcess process;
    //QString program = "c:\\windows\\system32\\calc.exe";
    //process.start(program);

    //setWindowModality(Qt::WindowModal);

//    setWindowModality(Qt::NonModal);
//    setEnabled(false);

//    for (int i=0;i<100;i++) QCoreApplication::processEvents();
//    QCoreApplication::processEvents();
//    Sleep(200);



//    setvideoButton->setEnabled(false);
    //QProcess proc;
    config cf;
    //qDebug() << "ffmpeg  -f dshow -show_video_device_dialog true -crossbar_video_input_pin_number 1 -i video=\""+cf.getValue("device").toString()+"\"";
    //proc.execute("ffmpeg  -f dshow -show_video_device_dialog true -crossbar_video_input_pin_number 1 -i video=\""+cf.getValue("device").toString()+"\"");

    QProcess *proc = new QProcess();
    qDebug()<<"SetVideo Inicio";
    proc->execute("ffmpeg  -f dshow -show_video_device_dialog true -crossbar_video_input_pin_number 1 -i video=\""+cf.getValue("device").toString()+"\"");

    //proc.start("ffmpeg  -f dshow -show_video_device_dialog true -crossbar_video_input_pin_number 1 -i video=\""+cf.getValue("device").toString()+"\"");
    //proc.waitForFinished();
    //proc.waitForFinished(-1);
    //bool finished = proc.waitForFinished(-1);
    //qDebug() << finished;
    //proc.close();

    //proc.start("ffmpeg  -f dshow -show_video_device_dialog true -crossbar_video_input_pin_number 1 -i video=\""+cf.getValue("device").toString()+"\"");
    //myffplay::player->start("ffmpeg  -f dshow -show_video_device_dialog true -crossbar_video_input_pin_number 1 -i video=\"OEM Device\"");
    // -fs => fullscreen
    //videoplayer->start("ffplay -x 1024 -y 570 -autoexit -exitonmousedown -f dshow  -crossbar_video_input_pin_number 1 -i video=\"OEM Device\" ");
    qDebug()<<"SetVideo Fin";
//    setvideoButton->setEnabled(true);
//    setEnabled(true);

}


void configuration::cleanStudies(){

    QStringList todel = _studies.getPending();
    if(todel.size()> 0){
        if( QMessageBox::question(this,tr("Borrar"),tr("¿Esta seguro de borrar los ")+ QString::number(todel.size())+tr(" estudios pendientes?"),QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes ){
            cleaner *_cleaner = new cleaner;
            cleanButton->setEnabled(false);
            connect(_cleaner,SIGNAL(finished()),_cleaner,SLOT(deleteLater()));
            connect(_cleaner,SIGNAL(finished()),this,SLOT(cleanFinished()));
            connect(_cleaner,SIGNAL(progress(int,int)),this,SLOT(progressClean(int,int)));
            _cleaner->setList(_studies.getNotFinished());
            _cleaner->start();
        }
    }
}

void configuration::isChanged(QString){
    changed = true;
    saveButton->setEnabled(true);
}

void configuration::isChanged(bool){
    changed = true;
    saveButton->setEnabled(true);
}


void configuration::progressClean(int i, int b){
    cleanButton->setText(tr(" Borrando los ") + "\n" + tr(" estudios pendientes ") + "\n(" + QString::number(i) + " / " + QString::number(b) + ")" );
}

void configuration::cleanFinished(){
    cleanButton->setEnabled(true);
    cleanButton->setText(tr("Borrar TODOS los ") + "\n" + tr("estudios pendientes "));
}


void configuration::RunCmd(QString comando){
    static int entry=0;
    Ejecuta proceso;

    qDebug()<<"entry";
    if (entry!=0) return;
    accesor::mw->setEnabled(false);
    //accesor::mw->
    entry=1;
    QApplication::setOverrideCursor(Qt::WaitCursor);
    proceso.Argumento(comando);
    proceso.start();
    Sleep(2000);
    //accesor::mw->showMinimized();
    QApplication::restoreOverrideCursor();

//  Christiam
    while (proceso.isRunning())
    {
        //accesor::mw->showMinimized();
        QCoreApplication::processEvents();
    }

    entry=0;
    accesor::mw->showMaximized();
    accesor::mw->setEnabled(true);
    return;

}


QString configuration::CheckAdquisitionDevice(void)
{
    QString device;

    //Liste source video
    QList<QByteArray> listaCamaras = cam.availableDevices();
    qDebug()<<"Dispositivos detectados";
    for (int i=0; i<listaCamaras.size(); i++){
        device=cam.deviceDescription(listaCamaras.at(i));
        qDebug()<<device;
        if (device.left(10)=="CY3014 USB") return device;
        if (device.left(11)=="FHD Capture") return device;
    }
    return ("");
}

void configuration::updateCams()
{
    QStringList hw;

    QList<QByteArray> listaCamaras = cam.availableDevices();
    if(listaCamaras.size() > 0)
    {
        for(int i=0; i<listaCamaras.size(); i++)
        {
            hw <<  cam.deviceDescription(listaCamaras.at(i));
        }
    }
    else
    {
        hw << tr("No se ha detectado  ningún dispositivo conectado.");
    }

    equipo->setItems(hw);
}
