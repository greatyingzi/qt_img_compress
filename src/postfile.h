#ifndef POSTFILE_H
#define POSTFILE_H

#include <QObject>

class PostFile : public QObject
{
    Q_OBJECT
public:
    explicit PostFile(const QString &picPath, const QString &qlty);
    void startPost();

    ~PostFile();
private:
    QString picPath;
    QString qlty;
signals:
    void compressedSuccess(const QString &url,const QString &srcFile);
};

#endif // POSTFILE_H
