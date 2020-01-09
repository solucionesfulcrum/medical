#ifndef CHECKBOXES_H
#define CHECKBOXES_H

#include <QtWidgets>

class checkboxes : public QWidget
{
    Q_OBJECT
public:
    explicit checkboxes(QStringList, QStringList, QWidget *parent = 0);
    QString text();
    QStringList getChecked();

signals:

public slots:

private:
    QList<QCheckBox *> checklist;
};


class radiobuttons : public QWidget
{
    Q_OBJECT
public:
    explicit radiobuttons(QStringList, QStringList, QWidget *parent = 0);
    QString text();

signals:

public slots:

private:
    QList<QRadioButton *> radioList;
};

#endif // CHECKBOXES_H
