#ifndef TITLELABEL_H
#define TITLELABEL_H

#include <QtWidgets>

class titlelabel : public QWidget
{
    Q_OBJECT
public:
    explicit titlelabel(QString t = "", QWidget *parent = 0);
    void setText(QString);
    void setLine(bool);
    void setLineWidth(int);
    void setAlignment( Qt::Alignment);
protected:
    void paintEvent(QPaintEvent *);
signals:

public slots:

private:
    Qt::Alignment textalign;
    QString title;
    bool line;
    int lineWidth;
};

#endif // TITLELABEL_H
