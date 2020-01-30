#ifndef MYFFPLAY_H
#define MYFFPLAY_H

#include <QtWidgets>
#include "accesor.h"
class myffplay : public QObject
{
    Q_OBJECT
public:
    myffplay();
    ~myffplay();
    void start(QString = "");
    bool waitForFinished();
    static myffplay *player;
    static QString basicOption;
    bool isplaying;

protected:
    void closeEvent(QCloseEvent *);
private slots:
    void videoStarted();
    void videoFinished(int);
    void videoClose();
    void videoPlayPause();
    void videoNextFrame();
    void videoForward();
    void videoBackward();
    void videoHaveByteWritted(qint64);
private:
    void setPlayButton();
    void setToolBar();
    QProcess * videoplayer;
    QWidget * videoToolBar, *videoHideBar;
    QToolButton
    * button_quit,
    * button_pause,
    * button_nextFrame,
    * button_backward,
    * button_forward
    ;
};

#endif // MYFFPLAY_H
