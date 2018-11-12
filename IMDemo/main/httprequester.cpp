#include "httprequester.h"
#include "globalsetting.h"
#include "cryptutil.h"

HttpRequester::HttpRequester(QObject *parent)
    :QObject(parent),
      m_manager(nullptr),
      m_reply(nullptr)
{
    m_manager = new QNetworkAccessManager(this);
    connect(m_manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(slot_replyFinished(QNetworkReply*)));
}

HttpRequester::~HttpRequester()
{
    if (m_manager) {
        delete m_manager;
        m_manager = nullptr;
    }

    if (m_reply) {
        delete m_reply;
        m_reply = nullptr;
    }
}

void HttpRequester::sendData(QString method, QString strUrl, QString data)
{
    QUrl url(strUrl);
    if (!url.isValid()) {
        qDebug() << "Url:" << url.toString() << " is invalid!";
        return;
    }
    qDebug() << "Request: " << strUrl;
    QNetworkRequest request(url);
    if (strUrl.startsWith("https")) {
        QSslConfiguration sslconfig;
        sslconfig.setProtocol(QSsl::TlsV1SslV3);
        sslconfig.setPeerVerifyMode(QSslSocket::VerifyNone);
        request.setSslConfiguration(sslconfig);
    }

    if (method.toLower() == "get") {
        m_reply = m_manager->get(request);
    } else if (method.toLower() == "post") {
        request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));

        m_reply = m_manager->post(request, data.toUtf8());
    } else if (method.toLower() == "head") {
        m_manager->head(request);
    } else if (method.toLower() == "put") {
        // ...
    }

    if (m_reply) {
        connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)),
                this, SLOT(slot_error(QNetworkReply::NetworkError)));
    }
}

void HttpRequester::youme_sendData(QString strUrl, QString data)
{
    QString curtime = QString::number(QDateTime::currentDateTime().toTime_t(), 10);
    QByteArray appkey = GlobalSettings::getInstance()->getIMAppkey();
    QByteArray appsect= GlobalSettings::getInstance()->getIMAppSecret();

    //Default value setting
    if (appkey.isEmpty())
        appkey = "YOUMEAA5EE5689436B39DC46E6195FF45F81CD46A1804";
    if (appsect.isEmpty())
        appsect = "zdDTMqLwVm9FTAT/ATIJ0fa2e4DPdtlzSrWu0z+mYHXYElO1ROJGuU4v1/AgRLRNEJ0uxgwEdfH6gKGLdH3N5wCXhPv7zJHJYgPYlTl5r1I8MzBOU3SLw3HXGPxcealhk4jGwSnUO1+oaaa9WZik9mFP4NEoAMkpM5gRnfrpODkBAAE=";


    QByteArray checksum = "";
    if (strUrl.contains("/v2/im")) {
        const QByteArray MD5_SALT = "YOUME8140";
        QByteArray restapi_key = CryptAdaptor::cryptMd5(appkey + MD5_SALT, true);
        checksum = CryptAdaptor::cryptSha1(appsect + restapi_key + curtime.toLatin1(), true);
    } else if (strUrl.contains("/v1/im")) {
        checksum = CryptAdaptor::cryptSha1(appsect + curtime.toLatin1(), true);
    }

    QString crypt_data;
    crypt_data += QString("?appkey=%1&identifier=admin&curtime=%2&checksum=%3")
            .arg(QString(appkey))
            .arg(curtime)
            .arg(QString(checksum));

    strUrl += crypt_data;
    sendData("post", strUrl, data);
}

void HttpRequester::youme_sendFile(QString filename)
{
    sendFile(filename);
}

void HttpRequester::sendFile(QString filename)
{

}

//slot
void HttpRequester::slot_error(QNetworkReply::NetworkError error)
{
    qDebug() << "Request error " << error;
}

void HttpRequester::slot_replyFinished(QNetworkReply *reply)
{
    if (reply) {
        m_recv_data = reply->readAll();
        //qDebug() << m_recv_data;
        emit data_recved(m_recv_data);
    }
}
