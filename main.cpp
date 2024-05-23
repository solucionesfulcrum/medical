#include <QApplication>
#include <QtWidgets>
#include <QTranslator>
#include <QInputDialog>
#include <QLockFile>
#include <QCryptographicHash>


#include "mainwindow.h"
#include "entitites/operators.h"




bool EncryptFile(const QString &inputFilePath,const QString &outputFilePath)
{
    QFile inputFile(inputFilePath);
    QFile outputFile(outputFilePath);

    if (!inputFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "Failed to open input file";
        return false;
    }

    if (!outputFile.open(QIODevice::WriteOnly))
    {
        qDebug() << "Failed to open output file";
        inputFile.close();
        return false;
    }

    QCryptographicHash hash(QCryptographicHash::Sha256);
    QByteArray fileData = inputFile.readAll();

    hash.addData(fileData);
    QByteArray key = hash.result();

    // XOR each byte of the file with the corresponding byte of the key
    for (int i = 0; i < fileData.size(); ++i)
    {
        fileData[i] = fileData[i] ^ key.at(i % key.size());
    }

    for(int i=0;i<key.length();i++)
    {
        fileData.append(key[i]);
    }

    // Write the encrypted data to the output file
    outputFile.write(fileData);

    // Close the files
    inputFile.close();
    outputFile.close();

    return true;
}

bool DecryptFile(const QString &inputFilePath, const QString &outputFilePath)
{
    QFile inputFile(inputFilePath);
    QFile outputFile(outputFilePath);

    if (!inputFile.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open input file";
        return false;
    }

    if (!outputFile.open(QIODevice::WriteOnly)) {
        qDebug() << "Failed to open output file";
        inputFile.close();
        return false;
    }

    QByteArray fileData = inputFile.readAll();
    QByteArray key = fileData.mid(fileData.length()-32,32);

    // XOR each byte of the file with the corresponding byte of the key
    for (int i = 0; i < fileData.size(); ++i) {
        fileData[i] = fileData[i] ^ key.at(i % key.size());
    }

    // Write the decrypted data to the output file
    outputFile.write(fileData);

    // Close the files
    inputFile.close();
    outputFile.close();
    return true;
}



void updateBD(){

    bool updated = false;
    operators o;

    //Create First Database;
    //TODO
    QString query;
    query = "ALTER TABLE protocols ADD COLUMN data TEXT;";
    updated = o.execute(query);

    query = "ALTER TABLE studies ADD COLUMN data TEXT;";
    updated = o.execute(query);

    query = "ALTER TABLE studies ADD COLUMN urgent INT DEFAULT 0;";
    updated = o.execute(query);
    query = "ALTER TABLE configuration ADD COLUMN keep_uncompressed INT DEFAULT 0;";
    updated = o.execute(query);
    query = "ALTER TABLE studies ADD COLUMN new_report INT DEFAULT 0;";
    updated = o.execute(query);
    query = "ALTER TABLE configuration ADD COLUMN device TEXT DEFAULT \"OEM Device\";";
    updated = o.execute(query);
    query = "ALTER TABLE patients ADD COLUMN phone TEXT;";
    updated = o.execute(query);
    query = "ALTER TABLE patients ADD COLUMN cellphone TEXT;";
    updated = o.execute(query);
    query = "ALTER TABLE patients ADD COLUMN email TEXT;";
    updated = o.execute(query);
    query = "ALTER TABLE patients ADD COLUMN height REAL;";
    updated = o.execute(query);
    query = "ALTER TABLE patients ADD COLUMN weight REAL;";
    updated = o.execute(query);
//  CR: 21/04/24 (Add AI diagnostic)
    query = "ALTER TABLE studies ADD COLUMN AI_Flag INT DEFAULT 0";
    updated = o.execute(query);
    query = "ALTER TABLE studies ADD COLUMN AI_INFO TEXT";
    updated = o.execute(query);
    query = "ALTER TABLE studies ADD COLUMN IMG_WI_OVERLAY TEXT";
    updated = o.execute(query);
    query = "ALTER TABLE studies ADD COLUMN IMG_WO_OVERLAY TEXT";
    updated = o.execute(query);
    query = "ALTER TABLE studies ADD COLUMN IMG_FETUS TEXT";
    updated = o.execute(query);
    qDebug() << "BDD was updated " << updated;
    if(updated)
        QMessageBox::information(NULL,QObject::tr("Base de datos actualizado"),QObject::tr("La base de datos se actualizó con éxito"));

}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


//---------------------------------------------------------------
    QLockFile lockFile("./MB_041019.lock");

    if(!lockFile.tryLock(100))
    {
        return 1;
    }

//---------------------------------------------------------------
//  Folder to store images
    if(!QDir("sweeps").exists())
        QDir().mkdir("sweeps");

//---------------------------------------------------------------
//  Select language
    QTranslator t;    
    QSettings settings("setting.ini", QSettings::IniFormat);
    settings.beginGroup("IDIOMA");
    QString idio = settings.value("idioma").toString();

    if(idio == "")
        settings.setValue("idioma", "ES");

    settings.endGroup();
    idio = idio.toUpper();

    //EN=Ingles  //ES=Español
    if (idio == "EN"){
        t.load(":/language_english.qm");
    }

    if(idio != "ES")
    {
        a.installTranslator(&t);
    }

//---------------------------------------------------------------

    QString plaindb = "medicalbox.db";
    QString encryptdb = "mbox.txt";

    QFile inputFilePlain(plaindb);

    if(!inputFilePlain.exists())
    {
        QFile inputFile(encryptdb);

        if(inputFile.exists())
        {
            DecryptFile(encryptdb,"medicalbox.db");
        }
    }



    QPixmap pixmap(":/icon/res/img/splash.png");

    QSplashScreen *splash = new QSplashScreen(pixmap);
    splash->show();

    QFontDatabase::addApplicationFont(":/fonts/res/font/Soft-Elegance.ttf");

 // Christiam
 // Font of version and date
    QFont newFont("Soft Elegance", 16);
    newFont.setLetterSpacing(QFont::AbsoluteSpacing,1.8);
    QApplication::setFont(newFont);//set font of application

    /*
    QTranslator qtTranslator;
    //qtTranslator.load("qt_es", QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    if(qtTranslator.load("qt_es",QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
        qDebug() << "translation loaded";
    qDebug()<< "translation ruta: " << QLibraryInfo::location(QLibraryInfo::TranslationsPath);
    a.installTranslator(&qtTranslator);
    */


    QTranslator qtbaseTranslator;
    qtbaseTranslator.load(":/qtbase_es.qm");
    if(idio == "ES"){
        //qtbaseTranslator.load("qtbase_es.qm");
        a.installTranslator(&qtbaseTranslator);
    }


    Q_INIT_RESOURCE(res);
    QFile file(":/qss/style.qss");
    if(file.open(QFile::ReadOnly)) {
        QString StyleSheet = QLatin1String(file.readAll());
        qApp->setStyleSheet(StyleSheet);
    }
    splash->showMessage(QObject::tr("Verficación de la base de datos"),Qt::AlignBottom | Qt::AlignCenter ,Qt::white);
    updateBD();
    splash->showMessage(QObject::tr("Creación de la interfaz"),Qt::AlignBottom | Qt::AlignCenter ,Qt::white);
    MainWindow medicalBox;
    medicalBox.show();
    splash->finish(&medicalBox);
    return a.exec();
}


