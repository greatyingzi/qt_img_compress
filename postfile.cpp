#include "postfile.h"
#include "networkrequestcallback.h"
#include <QHttpPart>
#include <QFile>
#include <QFileInfo>
#include <QHttpMultiPart>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonParseError>
#include <QEventLoop>

PostFile::PostFile(QString *picPath, QString *qlty) : QObject(nullptr)
{
    this->picPath = *picPath;
    this->qlty = *qlty;
}

void PostFile::startPost(){

    //设置基本URL
    QUrl url("http://api.resmush.it/ws.php");

    //生成对应的网址请求
    QFileInfo fileInfo(picPath);
    QString fileName =fileInfo.fileName();

    QFile* inputFile = new QFile(picPath);
    inputFile->open(QIODevice::ReadOnly);

    //multipart请求
    QHttpMultiPart* multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType, nullptr);
    //文件块
    QHttpPart filePart;
    filePart.setHeader(QNetworkRequest::ContentDispositionHeader,
                                 QVariant(QString("form-data; name=\"files\";filename=\"/home/lihneg/Downloads/q.png\"").arg(fileName)));
    filePart.setBodyDevice(inputFile);
    inputFile->setParent(multiPart);
    multiPart->append(filePart);

    QHttpPart paramer1Part;
    paramer1Part.setHeader(QNetworkRequest::ContentDispositionHeader,QVariant("form-data; name=\"qlty\""));

    paramer1Part.setBody(qlty.toUtf8());
    multiPart->append(paramer1Part);

    //生成对应的网址请求
    QNetworkRequest request;
    request.setUrl(url);

    //发送请求
    QNetworkAccessManager manager;
    QNetworkReply *reply = manager.post(request,multiPart);
    multiPart->setParent(reply);


    QEventLoop eventLoop;
    QObject::connect(&manager, SIGNAL(finished(QNetworkReply*)),&eventLoop, SLOT(quit()));
    eventLoop.exec(QEventLoop::ExcludeUserInputEvents);

    inputFile->close();
    qInfo() << "文件上传结束";

    NetworkRequestCallback *callback = new NetworkRequestCallback;

    if(reply->error() != QNetworkReply::NoError)
    {
        qInfo() << reply->error();
        return;
    }

    //解析返回的Json结果
    QByteArray replyData = reply->readAll();
    QJsonParseError json_error;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(replyData, &json_error));
    if(json_error.error != QJsonParseError::NoError)
    {
        qInfo() << "json 解析失败";
        return;
    }
    QJsonObject rootObj = jsonDoc.object();
    QString codeStr = rootObj.value("code").toString();


    QString destUrl = rootObj.value("dest").toString();
    qInfo() << "上传结果：" << destUrl;

//    //请求结果回调
//    NetworkRequestCallback *callback = new NetworkRequestCallback;
//    callback->reset(reply);
//    QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), callback, SLOT(onError(QNetworkReply::NetworkError)));
//    QObject::connect(reply, SIGNAL(readyRead()), callback, SLOT(onRead()));
}
