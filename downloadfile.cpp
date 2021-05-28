#include "downloadfile.h"
#include <QDesktopServices>
#include <QFileInfo>
#include <QMessageBox>
#include <QDir>

DownloadFile::DownloadFile(QWidget*parent, const QString &url, const QString &saveDir, const QString &srcFilePath){
    this->url = url;
    this->srcFilePath = srcFilePath;
    this->overrideSrcFile = false;
    this->saveDir = saveDir;
    QFile srcFile(srcFilePath);
    this->fileName = srcFile.fileName();
    QString saveFileTmp = this->saveDir;
    m_file = new QFile(saveFileTmp.append(QFileInfo(srcFilePath).fileName()));
    if (!m_file->open(QIODevice::WriteOnly)) {
        QMessageBox::information(parent, "提示", "打开临时文件错误");
        return;
    }
    qInfo() << "DownloadFile 初始化";
}

DownloadFile::DownloadFile(QWidget*parent, const QString &url, const QString &srcFilePath, bool overrideSrcFile){
    this->url = url;
    this->srcFilePath = srcFilePath;
    this->overrideSrcFile = overrideSrcFile;
    m_file = new QFile(srcFilePath);
    if (!m_file->open(QIODevice::WriteOnly)) {
        QMessageBox::information(parent, "提示", "打开临时文件错误");
        return;
    }
    qInfo() << "DownloadFile 初始化";
}

void DownloadFile::startDownload()
{
    qInfo() << "DownloadFile startDownload";
    this->m_networkManager = new QNetworkAccessManager();
    m_reply = m_networkManager->get(QNetworkRequest(url));

    connect(m_reply, SIGNAL(finished()), this, SLOT(onFinished()));
    connect(m_reply, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    qInfo() << "DownloadFile startDownload end";
}

//网络响应结束
void DownloadFile::onFinished()
{
    qInfo() << "DownloadFile onFinished";
    QFileInfo fileInfo;
    fileInfo.setFile(m_file->fileName());

    m_file->close();
    delete m_file;
    m_file = nullptr;

    m_reply->deleteLater();
    m_reply = nullptr;

    delete m_networkManager;
    m_networkManager = nullptr;

    qInfo() << "文件下载成功";
    emit onDownloadSuccess(srcFilePath,fileInfo.filePath());

    //    if (ui->checkBox->isChecked())   //勾选了，下载完成之后，打开下载的文件               //absoluteFilePath() 返回包含文件名的绝对路径。
    //        QDesktopServices::openUrl(QUrl::fromLocalFile(fileInfo.absoluteFilePath()));  //使用默认软件的打开下载的文件

    //    ui->btnDown->setEnabled(true);
    delete this;
}

//读取下载的数据
void DownloadFile::onReadyRead()
{
    if (this->saveDir != nullptr && !overrideSrcFile) {
        QFileInfo tmpfileInfo(srcFilePath);
        QDir dir = tmpfileInfo.dir();
        m_file  = new QFile(dir.path().append(QDir::separator()).append(prefix).append(fileName));
    }
    m_file->write(m_reply->readAll());   //将返回的数据进行读取，写入到临时文件中
}

DownloadFile::~DownloadFile()
{
    this->url = nullptr;
    this->srcFilePath = nullptr;
    this->saveDir = nullptr;
    this->fileName = nullptr;
}
