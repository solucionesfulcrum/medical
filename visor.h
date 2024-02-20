#ifndef VISOR_H
#define VISOR_H

#include <qmedicalboxwidget.h>
#include <QtWidgets>
#include <QtMultimedia>
#include <QVideoWidget>

class visorItem : public QWidget
{
    Q_OBJECT
public:
    explicit visorItem(int, QString, QString, QWidget *parent = 0);
    void isPlaying(bool);
    QString filename();
protected:
    void mouseReleaseEvent(QMouseEvent  * event);
signals:
    void setCurrent(int);
private slots:
    void play();
private:
    int id;
    QLabel * name;
    QString _filename;
};

class visor : public QMedicalBoxWidget
{
    Q_OBJECT
public:
    explicit visor(QMedicalBoxWidget *parent = 0);
    ~visor();

signals:

public slots:

private slots:
    void togglePlay();
    void slidePositionUpdate(qint64);
    void refresh();
    void setCurrentVideo(int);
    void playerStateChanged(QMediaPlayer::State);
    void videoSliderReleased();


private:

    void videoPlayerWidget();
    QWidget * _videoPlayerWidget;
    QScrollArea * area;
    QWidget *listBox;
    QVBoxLayout *listBoxLayout;
    QLabel * name;
    QString filename;
    QMediaPlayer *player;
    QMediaPlaylist *playlist;
    QVideoWidget * videoWidget;
    QSlider * videoSlider;
    QTimer poller;
    QList<QJsonObject> protocols;
    QList<visorItem*> visorItems;
    QPushButton * playPause;

};

#endif // VISOR_H
