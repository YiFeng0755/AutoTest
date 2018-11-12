#ifndef NETWORK_H
#define NETWORK_H
#include <QtNetwork>
#include <QNetworkAccessManager>

class QNetworkReply;
class QNetworkAccessManager;
class NetworkUtil : public QObject
{
    Q_OBJECT
public:
    NetworkUtil();
    void startRequest();
private:
    void setup();

private slots:
    void httpReady();
    void httpFinished();

private:
    QNetworkReply *m_reply;
    QNetworkAccessManager *m_manger;
};

#endif // NETWORK_H
