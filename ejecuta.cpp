#include "ejecuta.h"

Ejecuta::Ejecuta()
{
    comando="";
}

void Ejecuta::Argumento(QString & arg)
{
    comando=arg;
}

void Ejecuta::run()
{
//    qDebug()<<"Inicio de ejecucion de hilo "<< thread()->currentThreadId();

    QProcess *proc = new QProcess();
//    qDebug()<<"ObjectName: "<<proc->objectName();
//    proc->setObjectName("PropiedadTest");
//    qDebug()<<"ObjectName: "<<proc->objectName();
//    qDebug()<<"processId : "<<proc->processId();
//    qDebug()<<"pid : "<<proc->pid();
    proc->execute(comando);

//    qDebug()<<comando;
//    qDebug()<<"Fin de ejecucion de hilo "<< thread()->currentThreadId();
}

