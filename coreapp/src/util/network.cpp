#include "network.h"

#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QNetworkCookie>
#include <QNetworkReply>

NetworkUtil::NetworkUtil()
{

}

void NetworkUtil::setup()
{
    m_manger = new QNetworkAccessManager();
}


void NetworkUtil::httpFinished()
{

}

void NetworkUtil::httpReady()
{

}

void NetworkUtil::startRequest()
{
    QByteArray data;
//    m_reply = m_manger->post("10.101.38.132", &data);
//    connect(m_reply, SIGNAL(httpFinished(), this, SLOT()));

}
