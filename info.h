#include <qmedicalboxwidget.h>


class info : public QMedicalBoxWidget
{
    Q_OBJECT
public:
    explicit info(QMedicalBoxWidget *parent = 0);
    ~info();


signals:

private slots:
public slots:


private:
    QWidget
    * _protocolObstetric,
    * _protocolPulmonar,
    * _protocolQuadrant;

    QWidget * infoWidget;

    void setProtocolObstetric(void);
    void setProtocolPulmonar(void);
    void setProtocolQuadrant(void);

public:
    void refresh();
    QGridLayout *InfoLayout;

};

