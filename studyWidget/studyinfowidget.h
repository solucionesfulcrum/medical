#ifndef STUDYINFOWIDGET_H
#define STUDYINFOWIDGET_H

#include <QtWidgets>

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
private:
    QWidget * studyInfo;
    QTextEdit *studyInfoReason;
    QLabel
    * studyInfoPatientName,
    * studyInfoProtocols,
    * studyInfoDateTime
    ;

};

#endif // STUDYINFOWIDGET_H
