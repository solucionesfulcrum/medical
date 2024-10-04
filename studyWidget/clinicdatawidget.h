#ifndef CLINICDATAWIDGET_H
#define CLINICDATAWIDGET_H

#include <QtWidgets>
#include <studyWidget/clinicinput.h>
#include <entitites/studydesc.h>
#include <widgets/titlelabel.h>

class clinicDataWidget : public QWidget
{
    Q_OBJECT
public:
    explicit clinicDataWidget(QWidget *parent = 0);
    void setProtocols(int);
    QByteArray getJson();
    QString getReason();
    void reset();

    bool getUrgent(void);
    bool getTrainnning(void);
    bool getCampaign(void);

signals:

private:
    void createDataForm(void);

    titlelabel *protocolname;
    titlelabel *historytitle;
    studydesc protocol;

    QScrollArea * area;
    QButtonGroup * _reason;
    QWidget * dataWidget;
    QVBoxLayout * dataLayout;
    QList<QWidget*> datasWidget;
    QList<clinicInput*> datas;

    QRadioButton *exam;
    QRadioButton *urgent;
    QRadioButton *trainning;
    QRadioButton *campaign;

private slots:
    void confirmReason(int);

};

#endif // CLINICDATAWIDGET_H