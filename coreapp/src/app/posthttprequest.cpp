#include "posthttprequest.h"
#include <QNetworkAccessManager>
#include <QNetworkCookie>
#include <QNetworkReply>
#include <QTimer>
#include <QTextCodec>

HttpRequest::HttpRequest(QObject *parent)
    :QObject(parent),
    m_pNetworkManager(new QNetworkAccessManager(this)),
      m_pNetworkReply(NULL)
{
    m_pTimer = new QTimer(this);
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(slot_request_timeout()));
}

QByteArray HttpRequest::sendPost(const QString &url,
                                     const QString &params)
{
    QNetworkRequest request;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QUrl furl = QUrl(url + params);
    request.setUrl(furl);

    m_pNetworkReply = m_pNetworkManager->post(request, params.toLatin1());
    connect(m_pNetworkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(slot_request_finished()));

    return  m_pNetworkReply->readAll();
}

void HttpRequest::slot_request_timeout()
{
    m_pTimer->stop();
    emit request_timeout();
}

void HttpRequest::slot_request_finished()
{
    m_pTimer->stop();
    QByteArray alldata = m_pNetworkReply->readAll();
    //trans to unicode
    QString resultText = QTextCodec::codecForName("utf-8")->toUnicode(alldata);
    int nretcode = m_pNetworkReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    emit request_success(nretcode, resultText);

    m_pNetworkReply->deleteLater();
}
