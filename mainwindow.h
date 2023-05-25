#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include <QNetworkConfigurationManager>
#include <QEvent>
#include <QMouseEvent>

#include <queuewidget.h>
#include <configuration.h>
#include <include.h>
#include <study.h>
#include <visor.h>
#include <historical.h>
#include <info.h>

#include <studyfinished.h>
#include <studycheck.h>
#include <checkbandwith.h>
#include <accesor.h>

#include <widgets/wlogin.h>
#include "form/qvirtualkeyboard.h"
#include "entitites/operators.h"

#include "dialog/dialogoperator.h"
#include "dialog/myffplay.h"

#include <widgets/battery.h>
#include <widgets/diskspace.h>
#include <widgets/slidingstackedwidget.h>
#include <widgets/captureprocesswidget.h>

#include <capture/captureprocess.h>



#include <windows.h>
#include <shellapi.h>

// Need to link with shell32.lib
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "strmiids")


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    //static QGraphicsBlurEffect* effect;
    //static MainWindow *mw;

protected:
    void mousePressEvent(QMouseEvent *event) override;
    //void mouseMoveEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent* event) override;
    void closeEvent(QCloseEvent *ev) override;


private slots:
    void setmenuUS();
    void setmenuConfig();
    void setmenuHist();
    void setmenuOperador();
    void setmenuHelp();
    void setmenuInfo();
    void toggleQueue();
    void setMainWindow();
    void setLogin();
    void loadStudy(int);
    void logout();
    void ethernetConfig();
    void wifiConfig();
    void closeSystem();
    void queueFinished();
    void setMenuDisabled(bool);
    void LogoutForTimeout(void);
    void Slot_Timeout(int);


private:
    void setQueueGeometry();
    void uncheckMenu();
    void setMainWidget();
    void setQueueWidget();
    void setmenu(int);
    void header();
    void menu();
    void footer();
    void setSizes();

    bool isCapturing();

    bool isclosing;
    QDialog * stopDiag;

    int footerheight, headerheight, mainwidth, mainheight, queueWidth;

    SlidingStackedWidget * mainWidget;
    QString version;

    QTimer *TimerInactivity;

    checkBandwith *cb;
    operators ope;
    QWidget * _header;
    QWidget * _menu;
    QWidget * _footer;
    QWidget * mainwindow;
    WLogin *login;
    QToolButton
    * menuUS,
    * menuConfig,
    * menuHist,
    * menuAddOp,
    * menuHelp,
    * menueQueue,
    * menuClose,
    * menuInfo;

    QStackedWidget * _main;

    studyCheck *_studycheck;
    StudyFinished *pStudyFinished;

    study           * _study;
    visor           * _visor;
    configuration   * _configuration;
    historical      * _historical;
    dialogOperator  * _operatores;
    info            * _info;

    QLabel *user;

    QueueWidget * _queue;
    QPropertyAnimation *showQueueAnimation;
    QPushButton * showQueue;
    QRect showQueueGeometry;
    QRect hideQueueGeometry;
    bool isQueue;
    QList<QToolButton* > toolButtons;
    QList<QToolButton* > menuButtons;

};

#endif // MAINWINDOW_H
