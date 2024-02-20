#include "sweepimage.h"

//studyProtocolsForm::studyProtocolsForm(QWidget * parent) : QWidget(parent)
//    ImageDialog(const QPixmap& pixmap, QWidget* parent = nullptr);


ImageLabel::ImageLabel(QWidget* parent) : QLabel(parent)
{


}

void ImageLabel::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit clicked();
    }
}
