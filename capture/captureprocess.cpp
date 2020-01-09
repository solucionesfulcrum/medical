#include "captureprocess.h"


captureProcess* captureProcess::_captureProcess = NULL;
captureProcess::captureProcess()
{

}


void captureProcess::setFolder(QString f){
    folder = f;
}


QString captureProcess::getFolder(){
    return folder;
}

bool captureProcess::killProcess(){
    if(isRunning()){
        kill();
        qDebug() << "Kill Process FFMPEG PID " << processId();
    }
    if(isRunning())
        return false;
    else return true;
}

bool captureProcess::isRunning(){
    if(processId() != 0)
        return true;
    else return false;
}


bool captureProcess::startRecord(){
    //Check if exe exist
    if (!QFile::exists("ffmpeg.exe"))
    {
        //QMessageBox::warning(this,"Problema con la adquisición","No se puede adquirir las imagenes (error 0050)");
        return false;
    }
    config conf;
    conf.load();

    QString program = "ffmpeg -f dshow -crossbar_video_input_pin_number [VIDEOPIN] -i video=\"[VIDEODEVICE]\" -vcodec rawvideo -pix_fmt [PIXELCONF] -s [SIZE] -r [FPS] [FOLDER]/[VIDEONAME] -loglevel 16";
    QSettings settings("ffmpeg.ini", QSettings::IniFormat);
    settings.beginGroup("COMMAND");
    QString pg = settings.value("line").toString();
    settings.setValue("Default", program);

    if(pg == "")
        settings.setValue("line", program);
    else
        program = pg;
    settings.setValue("default",program);
    settings.endGroup();

    program = program.replace("[VIDEODEVICE]",conf.getValue("device").toString());
    program = program.replace("[VIDEOPIN]",conf.getValue("VIDEOINPUT").toString());
    program = program.replace("[FOLDER]",folder);
    program = program.replace("[VIDEONAME]",uncompressedvideoname);
    program = program.replace("[FPS]",conf.getValue("fps").toString());
    program = program.replace("[SIZE]",conf.getValue("SIZE").toString());
    program = program.replace("[PIXELCONF]",conf.getValue("PIXELCONF").toString());
    qDebug() << "Command FFMPEG: " << program;
    if(killProcess())
        start(program);
    return true;
}

bool captureProcess::stopRecord(){
    //Quit and save video from ffmpeg
    write("q");
}
