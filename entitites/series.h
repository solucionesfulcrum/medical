#ifndef SERIES_H
#define SERIES_H

#include <entitites/entities.h>
#include <include.h>

class series : public entities
{
public:
    series();
    ~series();

    int id_serie();
    int id_study();
    int sent();
    int checked();
    int toqueue();
    int capture();
    QString uid();
    QString datetime();
    int serieName();
    QString serieNameValue();
    QString serieEliminateAccent(QString s);
    QString serieNote();
    bool haveVideo();
    QString getVideoFile();

    int getSerieNumber();

    QList<int> listeIDFromStudy(int id);
    QList<int> listeIDtoQueue();
    int IDNextSend();

    bool updateSerie(QHash<QString, QString> data, QString uid);
private:

    int _id_serie, _id_study, _sent, _checked, _toqueue, _capture, _serie_name;
    QString _uid, _datetime, _serie_note, _serieNameValue;
};

#endif // SERIES_H
