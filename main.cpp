#include "mainwindow.h"
#include <QApplication>
#include <QtWidgets>
#include "entitites/operators.h"
#include <QTranslator>
#include <QInputDialog>

#include <QLockFile>

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
    qDebug() << "BDD was updated " << updated;
    if(updated)
        QMessageBox::information(NULL,QObject::tr("Base de datos actualizado"),QObject::tr("La base de datos se actualizó con éxito"));

}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Verifica que no halla instancia previa del programa (jchang 04/10/19)
//    QLockFile lockFile(QDir::temp().absoluteFilePath("<041019_1440>.lock"));
    QLockFile lockFile("C:/MedicalBox/MB_041019.lock");
    /* Trying to close the Lock File, if the attempt is unsuccessful for 100 milliseconds,
         * then there is a Lock File already created by another process.
         / Therefore, we throw a warning and close the program
         * */
    if(!lockFile.tryLock(100))
    {
/*            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setText("The application is already running.\n"
                           "Allowed to run only one instance of the application.");
            msgBox.exec();*/
            return 1;
    }

    //seleccionar idiomas
    //---------------------------------------------------------------------
    QTranslator t;

    //QStringList idiomas;
    //idiomas << "Español" << "English";
    //QString idio = QInputDialog::getItem(NULL,"Seleccionar Idioma","Seleccionar",idiomas);
    //--------------------------
    QSettings settings("setting.ini", QSettings::IniFormat);
    settings.beginGroup("IDIOMA");
    QString idio = settings.value("idioma").toString();

    if(idio == "")
        settings.setValue("idioma", "ES");

    settings.endGroup();
    //--------------------------

    idio = idio.toUpper();

    //EN=Ingles  //ES=Español
    if (idio == "EN"){
        t.load(":/MedicalBox_en.qm");
    }

    if(idio != "ES"){
        a.installTranslator(&t);
    }

    //---------------------------------------------------------------------

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


