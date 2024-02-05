#ifndef STUDYINFOWIDGET_H
#define STUDYINFOWIDGET_H

#include <QtWidgets>

#include "sweepimage.h"

class studyInfoWidget : public QWidget
{
    Q_OBJECT
public:
    studyInfoWidget(QWidget *parent = 0);
    ~studyInfoWidget();
public slots:
    void setStudyInfoPatientName(QString);
    void setStudyInfoPatient(QString, QString);
    void setStudyInfoPatientLastName(QString);
    void setStudyInfoProtocols(QString);
    void setStudyInfoDateTime(QString);
    void setStudyInfoReason(QString);

    void setSweepPicture(int protocol_id, int sweep);
    void resizePicture(void);

private:
    QWidget * studyInfo;
    QTextEdit *studyInfoReason;
    QLabel
    * studyInfoPatientName,
    * studyInfoProtocols,
    * studyInfoDateTime;
//    * studyInfoPicture

    ImageLabel *studyInfoPicture;

};

#endif // STUDYINFOWIDGET_H
