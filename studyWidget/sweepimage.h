#ifndef SWEEPIMAGE_H
#define SWEEPIMAGE_H

#include <QLabel>
#include <QPixmap>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QDialog>

class ImageLabel : public QLabel {
    Q_OBJECT

public:
    //ImageLabel(const QPixmap& pixmap, QWidget* parent = nullptr);
    ImageLabel(QWidget* parent = nullptr);

signals:
    void clicked(void);

protected:
    void mousePressEvent(QMouseEvent* event) override;

};

#endif // SWEEPIMAGE_H
