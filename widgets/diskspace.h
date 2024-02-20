#ifndef DISKSPACE_H
#define DISKSPACE_H

#include <QtWidgets>

class diskSpace : public QWidget
{
    Q_OBJECT
public:
    explicit diskSpace(QWidget *parent = nullptr);

signals:

public slots:

private slots:
    void refresh();

private:
    QProgressBar *pg;

    QTimer poller;
    QLabel * bat;
};

#endif // DISKSPACE_H
