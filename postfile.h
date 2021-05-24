#ifndef POSTFILE_H
#define POSTFILE_H

#include <QObject>

class PostFile : public QObject
{
    Q_OBJECT
public:
    explicit PostFile(QString *picPath = nullptr);
    void startPost();
private:
    QString *picPath;
signals:

};

#endif // POSTFILE_H
