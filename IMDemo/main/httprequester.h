#ifndef HTTPREQUESTER
#define HTTPREQUESTER
#include <QNetworkReply>

#include<QNetworkAccessManager>
#include<QNetworkRequest>

class HttpRequester : public QObject
{
    Q_OBJECT
public:
    explicit HttpRequester(QObject *parent = 0);
    ~HttpRequester();

    void sendData(QString method, QString strUrl, QString data);
    void sendFile(QString filename);

    void youme_sendData(QString strUrl, QString data);
    void youme_sendFile(QString filename);

    QByteArray data() const {
        return m_recv_data;
    }
Q_SIGNALS:
    void data_recved(QByteArray data);

private slots:
    void slot_error(QNetworkReply::NetworkError);
    void slot_replyFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager *m_manager;
    QNetworkReply *m_reply;

    QByteArray m_recv_data;
};

#endif // HTTPREQUESTER

