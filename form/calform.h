#ifndef CALFORM_H
#define CALFORM_H

#include <QWidget>
#include <QCalendarWidget>

class calform : public QWidget
{
    Q_OBJECT
public:
    explicit calform(QWidget *parent = nullptr);
    void setEnabled(bool);

signals:

public slots:

private:
    QCalendarWidget* cal;
};

#endif // CALFORM_H
