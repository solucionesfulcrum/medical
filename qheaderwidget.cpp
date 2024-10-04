#include "qheaderwidget.h"

QHeaderWidget::QHeaderWidget(QString t, QString icon, QPushButton *b, QWidget *parent) : QWidget(parent)
{
    setObjectName("widgetHeader");
    //setFixedHeight(100);

    QHBoxLayout * headl = new QHBoxLayout(this);
    QLabel * title = new QLabel(t);
    QLabel * img = new QLabel();
    img->setPixmap(QPixmap::fromImage(QImage(icon)));
    headl->addWidget(img);
    headl->addWidget(title,5,Qt::AlignLeft);
    if (b != NULL)
     headl->addWidget(b,1);
    headl->setMargin(0);
    headl->setSpacing(0);
}

QHeaderWidget::~QHeaderWidget()
{

}

