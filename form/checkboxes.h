#ifndef CHECKBOXES_H
#define CHECKBOXES_H

#include <QtWidgets>

class checkboxes : public QWidget
{
    Q_OBJECT
public:
    explicit checkboxes(QStringList, QStringList, QWidget *parent = nullptr);
    QString text();
    QStringList getChecked();
    QCheckBox* getItem(int);
    void clear();
    void setEnabled(bool status);

signals:

public slots:

private:
    QList<QCheckBox *> checklist;
};


class radiobuttons : public QWidget
{
    Q_OBJECT
public:
    explicit radiobuttons(QStringList, QStringList, QWidget *parent = nullptr);
    QString text();
    void reset();

signals:

public slots:

private:
    QList<QRadioButton *> radioList;
};

#endif // CHECKBOXES_H
