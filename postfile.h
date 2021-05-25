#ifndef POSTFILE_H
#define POSTFILE_H

#include <QObject>

class PostFile : public QObject
{
    Q_OBJECT
public:
    void startPost();
    explicit PostFile(QString *picPath, QString *qlty);
private:
    QString picPath;
    QString qlty;
signals:

};

#endif // POSTFILE_H
