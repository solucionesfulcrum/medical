   #include "queuewidget.h"

QString HTTPsender::API= apiurlsend;

HTTPsender::HTTPsender(QObject *o) : QObject(o)
{       
    //connect(&m_WebCtrl,SIGNAL(authenticationRequired(QNetworkReply*, QAuthenticator*)),SLOT(provideAuth(QNetworkReply*, QAuthenticator*)));
    connect(&m_WebCtrl,SIGNAL(finished(QNetworkReply*)),this,SLOT(finished(QNetworkReply*)));
    connect(&m_WebCtrl,&QNetworkAccessManager::authenticationRequired,this,&HTTPsender::provideAuth);
    connect(&m_WebCtrl,&QNetworkAccessManager::sslErrors,this,&HTTPsender::sslErrors);

    //connect(&m_WebCtrl,SIGNAL(sslErrors),this,SLOT(SSL))
}

HTTPsender::~HTTPsender(){
}

void HTTPsender::sslErrors(QNetworkReply* reply,const QList<QSslError>& errors){
    qDebug()<<"CR: SSL Errors";
    QString errorstring;
    for (const QSslError &error:errors) {
        if(!errorstring.isEmpty())
            errorstring += '\n';
        errorstring +=error.errorString();
    }
    reply->ignoreSslErrors();
}


void HTTPsender::provideAuth(QNetworkReply*, QAuthenticator* auth)
{    
    auth->setUser("MEDICALBOX1");
    auth->setPassword("123123");
}

void HTTPsender::send(int i)
{
    id = i;
    _cfg.load();
    _series.loadData(id);
    _studies.loadData(_series.id_study());
    _patient.loadData(_studies.id_patient());

    QString folder = "studies/"+QString::number(_series.id_study())+"/"+QString::number(i);
    QString meta_c = folder+"/"+cryptedmetafilename;
    QString video_c = folder+"/"+cryptedcompressedvideoname;



    url.setUrl(_cfg.ip()+HTTPsender::API+"?api-key=$2gyf$10a$nRLwp4Ea2R9MMoIqJ4ateloceT6ua59LMuC5WLlQamGKT5PU8GVcXfDukO");
    request.setUrl(url);

    mtp = new QHttpMultiPart(QHttpMultiPart::FormDataType);

//  Add apikey
    addPart("api-key",apikey);

    file = new QFile(video_c);
    QHttpPart videopart;
    videopart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"videofile\""));
    file->open(QIODevice::ReadOnly);
    qint64 videoFileSize = file->size();
    qDebug()<<"Video size: " +QString::number(videoFileSize);
    videopart.setBodyDevice(file);
    file->setParent(mtp);
    mtp->append(videopart);

    filemeta = new QFile(meta_c);
    QHttpPart metapart;
    metapart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"metadata\""));
    filemeta->open(QIODevice::ReadOnly);
    qint64 metaFileSize = filemeta->size();
    metapart.setBodyDevice(filemeta);
    filemeta->setParent(mtp);
    mtp->append(metapart);


//  Add Meta Size
    addPart("msize",QString::number(metaFileSize));

//  Add Video Size
    addPart("vsize",QString::number(videoFileSize));

    //Add check
    addPart("check",check);

    //Add Model US
    addPart("modelUS",_cfg.getValue("modelUS").toString());

    //Add Operator Fullname
    addPart("operator",_studies.operatorName());

    //Add Study Start Datetime
    addPart("study[start_date_time]",_studies.datetimetoFormat(_studies.getValue("starttime").toString(),"yyyy-MM-dd HH:mm:ss"));
    qDebug()<<"StartTime"<<_studies.getValue("starttime").toString();

    //Add Study Stop Datetime
    addPart("study[stop_date_time]",_studies.datetimetoFormat(_studies.getValue("finishtime").toString(),"yyyy-MM-dd HH:mm:ss"));
    qDebug()<<"StartTime"<<_studies.getValue("finishtime").toString();

    //Add Study Reason
    addPart("study[reason]","-"+_studies.reason());

    //Add Study Patient Sex
    addPart("study[patient_sex]",_patient.sex());

    //Add Study Patient Name
    addPart("study[patient_name]",_patient.name()+" "+_patient.lastName());

    //Add Study Box ID (to remove)
    addPart("study[box_id]","1");

    //Add Study Model US (to remove)
    addPart("study[modelUS]",_cfg.getValue("modelUS").toString());

    //Add Study Protocol ID
    addPart("study[protocol_id]",_studies.getValue("id_protocols").toString());
    // addPart("study[protocol_id]","1");

    //Add Study UID
    addPart("study[uid]",_studies.getValue("uid").toString());

    //Add Study Patient ID
    addPart("study[patient_id]",_patient.getValue("idp").toString());

    //Add Study Patient Size
    addPart("study[patient_size]",_patient.size());

    QString bday = _patient.birthday();
    bday = bday.insert(4,"-");
    bday = bday.insert(7,"-");
    //Add Study Patient Birthday
    addPart("study[patient_bday]",bday);

    //Add Study Patient Height
    addPart("study[patient_height]",_patient.height());

    //Add Study Patient Weight
    addPart("study[patient_weight]",_patient.weight());

    //Add Study Patient Phone
    addPart("study[patient_phone]",_patient.phone());

    //Add Study Patient Cellphone
    addPart("study[patient_cellphone]",_patient.cellphone());

    //Add Study Patient Email
    addPart("study[patient_email]",_patient.email());

    //Add Serie UID
    addPart("serie[uid]",_series.uid());

    //Add Serie start_date_time
    addPart("serie[start_date_time]",_series.datetimetoFormat(_series.getValue("starttime").toString(),"yyyy-MM-dd HH:mm:ss"));

    //Add Serie stop_date_time
    addPart("serie[stop_date_time]",_series.datetimetoFormat(_series.getValue("finishtime").toString(),"yyyy-MM-dd HH:mm:ss"));

    //Add Serie Try
    addPart("serie[try]",_series.getValue("intent").toString());

    //Add Serie Sweep_id
    addPart("serie[sweep_id]",_series.getValue("id_sweeps").toString());

    //Add Serie BOX ID (to remove)
    addPart("serie[box_id]","1");

    //Add Study Operator (to remove)
    addPart("study[operator]",_studies.operatorName());

    //Add BOX name
    addPart("namebox",_cfg.getValue("name").toString());
    //addPart("namebox","medbox1");

    //Add BOX password
    addPart("password",_cfg.getValue("pass").toString());
    //addPart("password","123123");

    //Add Study Data JSON
    addPart("study[data]",_studies.getData());

    //Add Study Urgent Value
    addPart("study[urgent]",_studies.getValue("urgent").toString());


    //Add Study Trainning Value    
    addPart("study[trainning]",_studies.getValue("trainning").toString());

    //Add Study Trainning Value
    addPart("study[ConsentimientoInformado]",_studies.getValue("ConsentimientoInformado").toString());


    QFile errfile("HTTPres.txt");
    if (errfile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
    {
        QTextStream out(&errfile);
        out << _studies.getValue("uid").toString()+ ": " << _series.uid() + ": Send to " << url.toString();
        errfile.close();
    }

    QNetworkReply *p = m_WebCtrl.post(request,mtp);
    connect(p,SIGNAL(uploadProgress(qint64,qint64)),this,SLOT(dl(qint64,qint64)));
    connect(p,SIGNAL(finished()), this, SLOT(GetReply()));
    connect(p,SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT (UploadError(QNetworkReply::NetworkError)));
    qDebug()<<"CR: Initialize sending";

}

void HTTPsender::GetReply(){
    qDebug()<<"Termino post";
}

void HTTPsender::UploadError(QNetworkReply::NetworkError err){
    qDebug()<<"Network Reply error: "<<err;
}

void HTTPsender::addPart(QString key, QString value, QString type){    
    QHttpPart Part;    
    Part.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(type+"; name=\""+key+"\""));
    Part.setBody(value.toStdString().c_str());
    mtp->append(Part);
}

void HTTPsender::addDevicePart(QString key, QFile* value, QString type){    
    QHttpPart Part;
    Part.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(type+"; name=\""+key+"\""));
    Part.setBodyDevice(value);
    mtp->append(Part);
}

void HTTPsender::showError(QNetworkReply::NetworkError ){
    //qDebug() << err;
    emit isError(id);
}

void HTTPsender::dl(qint64 a ,qint64 n){
    emit progress(a,n,id);
}

void HTTPsender::state(){
    qDebug() << id << rp->url() <<rp->errorString() << rp->canReadLine() << rp->isOpen() << rp->isFinished();
}

void HTTPsender::finished(QNetworkReply* pReply){
    qDebug()<<"CR: Post finished";
    QByteArray res = pReply->readAll();
    qDebug() << "CR: Finished " << id << pReply->errorString() << res ;

    QFile errfile("HTTPres.txt");
    if (errfile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
    {
        QDateTime datetime;
        QTextStream out(&errfile);
        out  <<  "\n"
             <<  "Studie UID: " + _studies.getValue("uid").toString() + "\n"
             << "Serie UID: " + _series.uid() + "\n"
             << "Error string: " + pReply->errorString() + "\n"
             << "Error number: " + QString::number(pReply->error()) + "\n"
             << "DateTime: " + datetime.currentDateTime().toString() +  "\n"
             << "Result: " << res << "\n";
        errfile.close();
    }

    QJsonDocument itemDoc = QJsonDocument::fromJson(res);
    QJsonObject rootObject = itemDoc.object();
    //qDebug() << rootObject;
    bool success = false;
    if(rootObject.contains("result") && rootObject.value("result").toString() == "success" )
        success = true;
    if(res == "success")
        success = true;

    if(pReply->error() == QNetworkReply::NoError && success){
        file->close();
        filemeta->close();
        delete file;
        delete filemeta;


        //Copy Uncompressed to other folder
        _series.loadData(id);
        QDir d;
        QString folderMove = "uncompressed/"+QString::number(_series.id_study())+"/"+QString::number(id)+"/";
        QString folderOrig = "studies/"+QString::number(_series.id_study())+"/"+QString::number(id);
        QString video = folderOrig+"/"+uncompressedvideoname;
        QString videoCompressed   = folderOrig+"/"+"compressedvideo.mp4";
        QString metaStudies         = folderOrig+"/"+"meta";
        QString video_m = folderMove+"/"+uncompressedvideoname;
        QString meta_c = folderOrig+"/"+cryptedmetafilename;
        QString video_c = folderOrig+"/"+cryptedcompressedvideoname;

        if (_cfg.getValue("keep_uncompressed").toInt() == 1){
            //To put on uncompressed folder
            if(d.mkpath(folderMove))
                d.rename(video,video_m);
        }
        else {
            //Remove uncompressed File
            QFile::remove(video);
        }

        QFile::remove(videoCompressed);
        QFile::remove(metaStudies);

        //Remove Crypted File
        QFile::remove(video_c);        
        QFile::remove(meta_c);

        emit isFinished(id,_series.id_study());
    }
    else{
        qDebug() << rootObject.value("error").toString();
        emit isError(id);
    }
}

void Queue::add(int id){
    queues.append(id);
}
void Queue::stop(){
    qDebug() << "Stop Queue Thred";
    finish = true;
}
void Queue::removeID(int id){
    for(int i =0; i<queues.size(); i++){
        if(id == queues.at(i))
            queues.removeAt(i);
    }
}

void Queue::run(){
    finish = false;
    wait = false;

    while(!finish){

        if(!wait){

            if (queues.size() > 0){
                bool err = false;
                int i = queues.first();

                // Start Process
                _series.loadData(i);
                _studies.loadData(_series.getValue("id_studies").toInt());
                _patient.loadData(_studies.getValue("id_patients").toInt());

                QString folder = "studies/"+QString::number(_series.id_study())+"/"+QString::number(i);
                QString meta = folder+"/"+metafilename;
                QString meta_c = folder+"/"+cryptedmetafilename;
                QString video = folder+"/"+uncompressedvideoname;
                QString video_cpr = folder+"/"+compressedvideoname;
                QString video_c = folder+"/"+cryptedcompressedvideoname;

                //Create Meta data file
                QByteArray m = createMetaData(i);                
                QFile f(meta);
                if (f.open(QIODevice::WriteOnly))
                    f.write(m);
                f.close();

                //Compression
                if (QFile::exists(video_cpr))
                    QFile::remove(video_cpr);
                QProcess * compress = new QProcess;
                QString program = "ffmpeg -rtbufsize 1500M -f rawvideo -vcodec "
                                  "rawvideo -s [SIZE] "
                                  "-r [FPS] -pix_fmt [PIXELCONF] "
                                  "-i "+video+" -c:v libx264 "
                                              "-pix_fmt yuv420p -b [VIDEOBITRATE]k "+video_cpr+"";
                config cf;
                qDebug() << cf.getValue("DEFAULTVIDEOBITRATE").toString();
                program = program.replace("[FPS]",cf.getValue("fps").toString());
                program = program.replace("[SIZE]",cf.getValue("SIZE").toString());
                program = program.replace("[PIXELCONF]",cf.getValue("PIXELCONF").toString());
                program = program.replace("[VIDEOBITRATE]",cf.getValue("DEFAULTVIDEOBITRATE").toString());
                compress->start(program);
                qDebug() << "Start Compression" << program;
                qDebug() << "Result:" << compress->waitForFinished(60000);

                delete compress;
                emit isCompressed(i);

                if (!QFile::exists(video_c) || !QFile::exists(meta_c)){
                    //Ciframiento
                    QFile::copy(meta,meta_c);
                    QFile::copy(video_cpr,video_c);
                    Medisecure _medisecure;

                    _medisecure.setFile(meta_c);
                    int medisecure_meta_c = _medisecure.start();
                    _medisecure.setFile(video_c);
                    int medisecure_video_c = _medisecure.start();

                    QFile errfile("HTTPres.txt");
                    if (errfile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
                    {
                        QDateTime datetime;
                        QTextStream out(&errfile);
                        out  <<  "\n"
                             << "DateTime: " + datetime.currentDateTime().toString() +  "\n"
                             <<  "MD-meta-C: " + QString::number(medisecure_meta_c) + "\n"
                             <<  "MD-video-C: " + QString::number(medisecure_video_c) + "\n";
                        errfile.close();
                    }

                    int metaSize = getFileSize(meta);
                    int metacSize = getFileSize(meta_c);
                    int videoSize = getFileSize(video_cpr);
                    int videocSize = getFileSize(video_c);



                    if(videoSize !=  videocSize-4 || metaSize !=  metacSize-4  ){
                        QFile::remove(video_c);
                        QFile::remove(meta_c);
                        //qDebug() << "Error de ciframiento";
                        err = true;
                        emit isError(i);
                    }
                }
                if(!err)
                {
                    emit isCrypted(i);
                    //Waiting for send
                    wait = true;
                    emit isWaiting(wait);
                }
            }
            sleep(1);
        }
        //qDebug() << "Run loop finish, is Finishd:" << finish ;
    }
    qDebug() << "Queue thread finish running" ;
    emit isFinish();
}

int Queue::getFileSize(QString g){
    QByteArray bytes;
    QFile f(g);
    if (f.open(QIODevice::ReadOnly))
        bytes = f.readAll();
    f.close();
    return bytes.size();
}

void Queue::next(){
    qDebug() << "next" << queues.size();
    if(!queues.isEmpty())
        queues.removeFirst();
    wait = false;
    emit isWaiting(wait);
}

QByteArray Queue::createMetaData(int){
    QByteArray metadata;
    config _cfg;
    _cfg.load();
    //SOP = Ultra Sound Storage
    QString SOPClassUID = "1.2.840.10008.5.1.4.1.1.6.1";
    studydesc sdt;
    sdt.loadData(_studies.getValue("id_protocols").toInt());
    QStringList metaString;
    metaString << "0008|0005&&ISO_IR 100;;"
               << "0008|0008&&ORIGINAL;;"
               << "0008|0020&&"+dateTimeToDCM(_studies.datetime())+";;"
               << "0008|0021&&"+dateTimeToDCM(_series.datetime())+";;"
               << "0008|0030&&"+dateTimeToDCM(_studies.datetime(),"time")+";;"
               << "0008|0031&&"+dateTimeToDCM(_series.datetime(),"time")+";;"
               << "0008|1030&&"+sdt.getValue("name").toString()+";;"
               << "0008|103E&&"+_series.serieNameValue()+";;"
               << "0008|0060&&US;;"


               << "0008|0070&&Medical Innovation And Technology;;"
                  //<< "0008|1010&&"+_cfg.AETitle()+";;"
               << "0008|1090&&Medical Box;;"
               << "0010|0010&&"+_patient.name()+" "+_patient.lastName()+";;"
               << "0010|0020&&"+_patient.id()+";;"
               << "0010|0030&&"+_patient.birthday()+";;"
               << "0010|0040&&"+_patient.sex()+";;"
               << "0010|1020&&"+_patient.size()+";;"
               << "0020|0010&&"+QString::number(_series.id_study())+";;"
               << "0020|0011&&"+QString::number(_series.getSerieNumber())+";;"
               << "0020|000e&&"+_series.uid()+";;"
               << "0020|000d&&"+_studies.uid()+";;"
               << "0032|4000&&"+_studies.reason()+";;"
               << "0008|0016&&"+SOPClassUID+";;"
                  ;

    foreach (QString s, metaString) {
        metadata.append(s.toStdString().c_str());
    }
    return metadata;

}

QString Queue::dateTimeToDCM(QString v, QString type){
    QDateTime d;
    bool ok;
    qint64 timestamp = v.toLongLong(&ok);
    d.setMSecsSinceEpoch(timestamp);
    if(type == "time")
        return d.toString("HHmmss");
    else return d.toString("yyyyMMdd");
}


QueueObject::QueueObject(int id, QWidget *parent) : QWidget(parent)
{
    QWidget * qo = new QWidget;
    qo->setObjectName("queueObject");
    QHBoxLayout * m = new QHBoxLayout(this);
    m->addWidget(qo);
    qo->setFixedWidth(270);
//    m->setSpacing(0);
//    m->setMargin(0);
    _series.loadData(id);
    //QLabel * title = new QLabel(studies::datetimetoFormat(_series.datetime()));

//------------------------------------------------------------------
//  CR:
    QLabel * title = new QLabel(studies::datetimetoFormat(_series.datetime(),"dd/MM/yy <br /> hh:mm:ss"));
//------------------------------------------------------------------

    title->setFixedWidth(100);
    //title->setFixedHeight(21);
    QFont f( "Arial", 9);
    title->setFont(f);
    title->setObjectName("queueObjectTitle");

    compressed = new QLabel("");
    crypted = new QLabel("");

    QFont fsent( "Arial", 11);
    sent = new QLabel("");
    sent->setFont(fsent);

    compressed->setAlignment(Qt::AlignLeft);
    crypted->setAlignment(Qt::AlignLeft);
    sent->setAlignment(Qt::AlignLeft);


    statut << compressed << crypted << sent;
    foreach(QLabel *l, statut)
    {
        l->setFixedHeight(21);
        l->setFixedWidth(40);
        l->setObjectName("queueObjectStatut");
    }

    pb = new QProgressBar;
    pb->setFixedHeight(5);
    //pb->setFixedWidth(270);
    pb->setTextVisible(false);


    del = new QPushButton(QIcon(":/icon/res/img/close.png"),"");
    del->setFixedSize(20,21);
    del->setObjectName("redButton");

    connect(del,SIGNAL(clicked()),this,SLOT(deleteQueue()));

    QHBoxLayout * adv = new QHBoxLayout;
    adv->addWidget(title,Qt::AlignLeft);
    adv->addWidget(compressed,Qt::AlignLeft);
    adv->addWidget(crypted,Qt::AlignLeft);
    adv->addWidget(sent,Qt::AlignLeft);
    adv->addWidget(del,Qt::AlignLeft);
    adv->setSpacing(1);
    adv->setMargin(0);


    QVBoxLayout *wl = new QVBoxLayout(qo);
    wl->addLayout(adv);
    wl->addWidget(pb);
    wl->setSpacing(0);
    wl->setMargin(0);

    //wl->setSpacing(1);
    //wl->setMargin(5);
    pb->show();
    _id = id;
    numError = 0;
    finished = false;
}

QueueObject::~QueueObject()
{

}
bool QueueObject::isFinished(){
    return finished;
}

void QueueObject::deleteQueue(){
    qDebug() << "Del";
    emit deleteWidg(_id);
}

void QueueObject::setProgress(qint64 v, qint64 m){
    pb->setMaximum(m);
    pb->setValue(v);
    pb->show();
}

void QueueObject::isSent(){
    sent->setProperty("finish",true);
    sent->style()->unpolish(sent);
    sent->style()->polish(sent);
    sent->update();
    setProgress(1,1);
    del->setEnabled(false);
    finished = true;
}

void QueueObject::isCompressed(){
    compressed->setProperty("finish",true);
    compressed->style()->unpolish(compressed);
    compressed->style()->polish(compressed);
    compressed->update();
}

void QueueObject::isCrypted(){
    crypted->setProperty("finish",true);
    crypted->style()->unpolish(crypted);
    crypted->style()->polish(crypted);
    crypted->update();
}

void QueueObject::isQueued(){
    foreach(QLabel *l, statut)
    {
        l->setProperty("finish",false);
        l->style()->unpolish(l);
        l->style()->polish(l);
        l->update();
    }
}

void QueueObject::error(){
    isQueued();
    numError++;
    sent->setText(QString::number(numError));
}

int QueueObject::id(){
    return _id;
}


//  QueueWidget
QueueWidget::QueueWidget(QWidget *parent) : QWidget(parent)
{
    isRunning = false;
    QHBoxLayout * l = new QHBoxLayout(this);
    QWidget * queueWidget = new QWidget();
    queueWidget->setObjectName("QueueWidget");

//------------------------------------------------
//  CR: 01/02/21
//  queueWidget->setFixedWidth(380);
    queueWidget->setFixedWidth(280);
//------------------------------------------------

    l->addWidget(queueWidget);
//    l->setSpacing(0);
//    l->setMargin(0);

    QVBoxLayout * layout = new QVBoxLayout(queueWidget);

//  Create Header
    QWidget * header = new QWidget();
    header->setObjectName("QueueWidgetHeader");
    header->setMaximumHeight(70);

    QPushButton * cleanButton = new QPushButton(QIcon(":/icon/res/img/clean.png"),"");
    cleanButton->setIconSize(QSize(50,50));
    cleanButton->setMaximumSize(60,60);
    cleanButton->setObjectName("greenButton");
    connect(cleanButton,SIGNAL(clicked()),this,SLOT(clean()));

    titlelabel * htitle = new titlelabel(tr("Lista de envío"));
    htitle->setAlignment(Qt::AlignLeft);

    QHBoxLayout * hlayout = new QHBoxLayout(header);
    hlayout->addWidget(htitle,Qt::AlignCenter);
    hlayout->addWidget(cleanButton);

//  CR: 01/02/21
//  hlayout->setSpacing(0);
//  hlayout->setMargin(0);

    _infoWidget = new QWidget;
    _infoWidget->setObjectName("QueueInfoWidget");
    _infoWidget->setFixedSize(380,60);
    infoLabel = new QLabel("");
    queueStatut = new QLabel("");
    queueStatut->setObjectName("statut");
    queueStatut->setFixedSize(30,60);
    queueStatut->setProperty("run",true);
    queueStatut->setAlignment(Qt::AlignCenter);
    numberStatut = new QLabel("");

    QHBoxLayout * infoLayout = new QHBoxLayout(_infoWidget);
    infoLayout->addWidget(infoLabel);
    infoLayout->addWidget(numberStatut);
    //infoLayout->addWidget(queueStatut);
    infoLayout->setMargin(0);
    infoLayout->setSpacing(1);

    //cb = new checkBandwith;
    QueueWidgetList = new QWidget;
    QueueWidgetList->setObjectName("QueueWidgetList");

//----------------------------------------------------------
//  CR: Set size of list of queus
//----------------------------------------------------------
    QueueWidgetList->setFixedWidth(290);

    queueLayout = new QVBoxLayout(QueueWidgetList);
    queueLayout->setMargin(0);
    queueLayout->setSpacing(1);
    queueLayout->setSizeConstraint(QLayout::SetNoConstraint);
    queueLayout->setAlignment( Qt::AlignTop );

    _area = new QScrollArea();
    _area->setObjectName("Queue");
    _area->viewport()->setAttribute(Qt::WA_AcceptTouchEvents);
    QScroller::grabGesture(_area->viewport(),QScroller::LeftMouseButtonGesture);
    //_area->setFixedSize(282,670);
    _area->setWidgetResizable(true);
    _area->setWidget(QueueWidgetList);

    layout->addWidget(header);
    layout->addWidget(_area);
    //layout->addWidget(cb);
    layout->setSpacing(0);
    layout->setMargin(0);

    _httpsend = new HTTPsender;

    connect(_httpsend,SIGNAL(isFinished(int,int)),this,SLOT(isFinished(int,int)));
    connect(_httpsend,SIGNAL(isError(int)),this,SLOT(isError(int)));
    connect(_httpsend,SIGNAL(progress(qint64,qint64,int)),this,SLOT(setUpProgress(qint64,qint64,int)));

//-----------------------------------------------------
//  CR: q is a thread
//-----------------------------------------------------
    connect(&q,SIGNAL(isCrypted(int)),this,SLOT(isCrypted(int)));
    connect(&q,SIGNAL(isCompressed(int)),this,SLOT(isCompressed(int)));
    connect(&q,SIGNAL(isError(int)),this,SLOT(isError(int)));
    connect(&q,SIGNAL(isWaiting(bool)),this,SLOT(waitingThread(bool)));
    connect(&q,SIGNAL(isFinish()),this,SLOT(threadIsFinished()));
    connect(&q,SIGNAL(finished()),this,SLOT(threadIsFinished()));
    load();

}





QueueWidget::~QueueWidget()
{
    delete _httpsend;
    stop();
    q.deleteLater();
}

void QueueWidget::start(){
    q.start();
    isRunning = true;
}

void  QueueWidget::stop(){
    q.stop();
    // q.deleteLater();
}

QWidget* QueueWidget::infoWidget(){
    return _infoWidget;
}

void QueueWidget::waitingThread(bool b){
    if(b) queueStatut->setProperty("run",true);
    else queueStatut->setProperty("run",false);
    queueStatut->style()->unpolish(queueStatut);
    queueStatut->style()->polish(queueStatut);
}


void QueueWidget::setUpProgress(qint64 v ,qint64 m,int id){
    QueueObject * q = queueObject(id);
    if(q != NULL)
    {
        q->setProgress(v,m);
        qDebug()<<"CR: "<< v << m ;
    }

}

void QueueWidget::isCompressed(int id){
    QueueObject * q = queueObject(id);
    if(q != NULL)
        q->isCompressed();
}

void QueueWidget::isCrypted(int id){
    QueueObject * q = queueObject(id);
    if(q != NULL){
        q->isCrypted();
        _httpsend->send(id);
        infoLabel->setText(tr("Envío (")+QString::number(id)+tr(")"));
    }
}

void QueueWidget::isError(int id){
    foreach (QueueObject *queue, queuesObjects) {
        if(queue->id() == id){            
            queue->error();
            infoLabel->setText(tr("Error (")+QString::number(id)+tr(")"));
            q.add(id);
        }
    }
    q.next();
    refreshInfo();
}
//--------------------------------------------------------------------------
// CR: 24/01/23
void QueueWidget::isFinished(int id, int id_study){
    QHash<QString,QString> data;
    //data.insert("sent",QString::number(v));
    data.insert("sent",QString::number(1));
    _series.update(data,id);
    QueueObject * quee = queueObject(id);
    if(quee != NULL){
        quee->isSent();        
        infoLabel->setText(tr("Enviado (")+QString::number(id)+tr(")"));
    }
    q.next();
    refreshInfo();
//----------------------------------------------------------------------------------
//  CR: 25/01/23
    QDir dir("uncompressed/"+QString::number(id_study)+"/"+QString::number(id));
    if(dir.exists()){
        dir.removeRecursively();
    }
/*
//  CR: 27/01/23
    if(dir.setCurrent("studies/"+QString::number(id_study)+"/"+QString::number(id))==true)
        dir.removeRecursively();
        */
}
//----------------------------------------------------------------------------------

void QueueWidget::clean(){
    foreach(QueueObject* obj, queuesObjects){
        if(obj->isFinished()){
            queueLayout->removeWidget(obj);
            queuesObjects.removeOne(obj);
            delete obj;
        }
    }
    refreshInfo();
}

void QueueWidget::threadIsFinished(){
    isRunning = false;    
    emit isThreadFinished();
}

bool QueueWidget::IsThreadRunning(){
    return isRunning;
}



void QueueWidget::add(int id){
    QueueObject *obj = new QueueObject(id);
    obj->isQueued();
    connect(obj,SIGNAL(deleteWidg(int)),this,SLOT(deleteQueue(int)));
    queueLayout->insertWidget(0,obj);
    queuesObjects.append(obj);;
    q.add(id);
    refreshInfo();
}

void QueueWidget::refreshInfo(){
    int f = 0;
    foreach (QueueObject *queue, queuesObjects)
        if(queue->isFinished()) f++;
    numberStatut->setText(QString::number(f)+" / "+QString::number(queuesObjects.size()));
}

void QueueWidget::deleteQueue(int id){
    bool admin = operators::isAdmin();
    if(admin) return;

    int n = 0;
    foreach (QueueObject *queue, queuesObjects) {
        if(queue->id() == id){
            delete queue;
            q.removeID(id);
            queuesObjects.removeAt(n);
            QHash<QString,QString> data;
            data.insert("toqueue","0");
            _series.update(data,id);
        }
        n++;
    }
    refreshInfo();
}

void QueueWidget::load(){
    foreach(int s, _series.listeIDtoQueue())
        add(s);
    refreshInfo();
}



QueueObject* QueueWidget::queueObject(int id){
    QueueObject* res = NULL;
    foreach (QueueObject *q, queuesObjects) {
        if(q->id() == id)
            res = q;
    }
    return res;
}

