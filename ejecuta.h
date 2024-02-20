#ifndef EJECUTA_H
#define EJECUTA_H

#include <QThread>
#include <QProcess>
#include <qdebug.h>
#include <windows.h>

class Ejecuta : public QThread
{
    public:
        Ejecuta();
        void Argumento(QString & arg);
    private:
        QString comando;
        void run();
};

#endif // EJECUTA_H
