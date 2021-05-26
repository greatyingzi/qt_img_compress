#ifndef NETWORKREQUESTCALLBACK_H
#define NETWORKREQUESTCALLBACK_H

#include <QObject>
#include <QNetworkReply>
#include <QDebug>

class NetworkRequestCallback : public QObject
{
    Q_OBJECT
public:
    explicit NetworkRequestCallback(QObject *parent = nullptr);
    bool success;
    void reset(QNetworkReply *reply = nullptr){
        this->reply = reply;
        this->success = true;
    }

private:
    QNetworkReply* reply;
public slots:
    void onError(QNetworkReply::NetworkError status) {
        success = false;
        qInfo() << "Request Error " << status;
    }
    void onRead() {
        QNetworkReply::NetworkError status = reply->error();
        QByteArray byteArray = reply->readAll();
        qInfo() << status;
        qInfo() << byteArray;
        qInfo() << "Read Byte Stream";
    }
signals:

};

#endif // NETWORKREQUESTCALLBACK_H
