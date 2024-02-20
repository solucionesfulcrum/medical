#ifndef CAPTUREBUTTON_H
#define CAPTUREBUTTON_H

#include <QtWidgets>

class captureButton : public QWidget
{
    Q_OBJECT
public:
    captureButton(QWidget * parent = 0);
    ~captureButton();
    void setTime(QString = "00:00");
    void setInfo(QString);
    void setRecord(bool);
    void setBlock(bool);
    bool isBlocked();
    void setTimeMs(int);
    void setMaxTime(int i = 60000);
protected:
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);
signals:
    void clicked();
private:
    QTimer poller;
    QString time;
    QString info;

    int msTime;
    int tiempoMax;

    bool isRecord;
    bool mousePressed;
    bool isBloqued;
    QPointF a,b,c,d;
    QPointF f2;
    QRectF textRect;
    QRectF infoRect;
    int changeH;
    int
    playWidth
    ,playHeight
    ,timeWidth
    ,timeHeight
    ,wstart
    ,hstart
    ;



};

#endif // CAPTUREBUTTON_H
