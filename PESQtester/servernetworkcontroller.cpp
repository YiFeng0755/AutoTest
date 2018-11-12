#include "servernetworkcontroller.h"

NetworkController *NetworkController::m_instance = nullptr;

NetworkController::NetworkController(const QString &url)
    :m_url(url)
{
    m_networkmgr = new QNetworkAccessManager(this);
    connect(m_networkmgr, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(slot_finished(QNetworkReply*)));
    m_reponse = "";
}

NetworkController * NetworkController::createInstance()
{
    if (m_instance == nullptr)
        m_instance = new NetworkController();
    return m_instance;
}

void NetworkController::setUrl(const QString &url)
{
    m_url = url;
}

QByteArray NetworkController::getResponse()
{
    return m_reponse;
}

void NetworkController::request_server(QString cmd, QString type, int scale)
{
    QNetworkRequest request;

    QUrl url;
    QString data = ""; //m_url不要改变
    if (cmd.isEmpty() && type.isEmpty()) {
        return;
    }

    if (!cmd.isEmpty())
        data += QString("?cmd=%1").arg(cmd);
    if (!type.isEmpty())
        data += QString("&type=%1").arg(type);
    if (cmd != "show")
        data += QString("&scale=%1").arg(scale);
    url.setUrl(m_url + data);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setUrl(url);
    qDebug() << url.toString();
    QNetworkReply *reply = m_networkmgr->post(request, data.toUtf8());
    /*connect(reply, SIGNAL(finished()), this, SLOT(slot_finished()));*/
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this,
            SLOT(slot_error(QNetworkReply::NetworkError)));
}

void NetworkController::request_server(QString cmd)
{
    QNetworkRequest request;

    QUrl url;
    if (cmd.isEmpty()) {
        return;
    }

    QByteArray data;
    data += "?cmdstr=";
    data += cmd;

    url.setUrl("http://192.168.155.11:8999/execmd" + data);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setUrl(url);
    qDebug() << url.toString();
    QNetworkReply *reply = m_networkmgr->post(request, data);
    /*connect(reply, SIGNAL(finished()), this, SLOT(slot_finished()));*/
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this,
            SLOT(slot_error(QNetworkReply::NetworkError)));
}

void NetworkController::request_status()
{
    QNetworkRequest request;

    QUrl url;
    QString data = "?cmd=show"; //m_url不要改变

    url.setUrl(m_url + data);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setUrl(url);

    QNetworkReply *reply = m_networkmgr->post(request, data.toUtf8());
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this,
            SLOT(slot_error(QNetworkReply::NetworkError)));
}

void NetworkController::slot_finished(QNetworkReply* reply)
{
    if (!reply)
        return;
    QByteArray content = reply->readAll();
    m_reponse = content;

    if (reply->request().url().toString().contains("show"))
        emit statusRecved(content);

    if (!content.isEmpty())
        emit dataRecved(content);

}

void NetworkController::slot_error(QNetworkReply::NetworkError errorCode)
{
    QNetworkReply *pReplay = qobject_cast<QNetworkReply*>(sender());
       // 输出错误码及错误信息;
    qDebug()<< errorCode;
    qDebug()<< pReplay->errorString();
}

