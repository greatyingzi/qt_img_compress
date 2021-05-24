#include "postfile.h"
#include "networkrequestcallback.h"
#include <QHttpPart>
#include <QFile>

PostFile::PostFile(QString *picPath) : QObject(nullptr)
{
    this->picPath = picPath;

}

void PostFile::startPost(){

    //网络请求处理器
    QNetworkAccessManager manager;

    //设置基本URL
    QUrl url("http://api.resmush.it/ws.php");

    //设置URL额外参数
//    QUrlQuery query;
//    query.addQueryItem("qlty", "90");
//    url.setQuery(query);

    //构建请求
    QNetworkRequest request(url);

    //设置表单参数
    QHttpMultiPart* formData = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    //添加文本表单:folder=001
    QHttpPart textPart;
    textPart.setHeader(QNetworkRequest::ContentDispositionHeader, "form-data; name=\"folder\"");
    textPart.setBody("001");
    formData->append(textPart);
    //添加文件表单:file=File
    QHttpPart filePart;
    filePart.setHeader(QNetworkRequest::ContentTypeHeader, "image/png");
    filePart.setHeader(QNetworkRequest::ContentDispositionHeader, "form-data; name=\"files\"; filename=\"png_example_original.png\"");
    QFile* file = new QFile(*picPath);
    file->open(QIODevice::ReadOnly);
    file->setParent(formData);  //绑定父级对象，Qt在父级对象回收时，会自动回收子级对象
    filePart.setBodyDevice(file);
    formData->append(filePart);

    //执行请求
    QNetworkReply* reply = manager.post(request, formData);

    //请求结果回调
//    NetworkRequestCallback *callback = new NetworkRequestCallback;
//    callback->reset(reply);
//    QObject::connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), callback, SLOT(onError(QNetworkReply::NetworkError)));
//    QObject::connect(reply, SIGNAL(readyRead()), callback, SLOT(onRead()));
}
