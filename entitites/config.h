#ifndef CONFIG_H
#define CONFIG_H


#include <entitites/entities.h>

class config : public entities
{
public:
    config();
    ~config();
    QString ip();
    int videoInput();
    int maxTimeSend();
    int maxFileSize();
    int minFileSize();
    int fileSize();
    QString AETitle();
    void loadData(int);
    QStringList serverList();

    bool update(QHash<QString, QString> data);
    bool load();

private:
    QString _ip, _AETitle;
    int _videoInput, _maxTimeSend, _maxFileSize, _minFileSize, _fileSize;
};

#endif // CONFIG_H
