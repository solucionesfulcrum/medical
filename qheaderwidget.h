#ifndef QHEADERWIDGET_H
#define QHEADERWIDGET_H

#include <QtWidgets>

class QHeaderWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QHeaderWidget(QString title = "", QString icon = "", QPushButton *b = 0, QWidget *parent = 0);
    ~QHeaderWidget();


signals:

public slots:
};

#endif // QHEADERWIDGET_H
