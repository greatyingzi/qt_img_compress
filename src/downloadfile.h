#ifndef DOWNLOADIMG_H
#define DOWNLOADIMG_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>

class DownloadFile : public QObject
{
    Q_OBJECT
public:
    DownloadFile(QWidget *parent, const QString &url, const QString &saveDir, const QString &srcFilePath);
    DownloadFile(QWidget *parent, const QString &url, const QString &srcFilePath, bool overrideSrcFile);
    void startDownload();

    ~DownloadFile();
private:
    QString prefix = "compressed_";
    QString url;
    QString srcFilePath;
    QString saveDir;
    QString fileName;
    bool overrideSrcFile;

    QNetworkAccessManager* m_networkManager;   //网络管理
    QNetworkReply* m_reply;                    //网络响应
    QFile* m_file;                             //下载保存的临时文件

signals:
    void onDownloadSuccess(const QString &srcFilePath, const QString &newPath);
public slots:
    void onFinished();          //网络响应结束
    void onReadyRead();         //读取下载的数据
};

#endif // DOWNLOADIMG_H
