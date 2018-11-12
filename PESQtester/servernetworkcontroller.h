#ifndef SERVERNETWORKCONTROLLER
#define SERVERNETWORKCONTROLLER

#include <QObject>
#include <QtNetwork>

class NetworkController : public QObject
{
    Q_OBJECT
public:
    NetworkController(const QString& url = "http://192.168.155.11:8999/setting");
    void request_server(QString cmd, QString type, int scale = 0);
    void request_server(QString cmd);
    void request_status();
    void setUrl(const QString& url);
    QByteArray getResponse();

    static NetworkController* createInstance();
Q_SIGNALS:
    void dataRecved(QByteArray);
    void statusRecved(QByteArray);

private slots:
    void slot_finished(QNetworkReply* reply);
    void slot_error(QNetworkReply::NetworkError);
private:
    QNetworkAccessManager *m_networkmgr;
    QString m_url;
    QByteArray m_reponse;

    static NetworkController* m_instance;
};

#endif // SERVERNETWORKCONTROLLER

