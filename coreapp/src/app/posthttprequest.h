#ifndef POSTHTTPREQUEST
#define POSTHTTPREQUEST

#include <QObject>
#include <QNetworkAccessManager>

class QTimer;
class HttpRequest : public QObject
{
    Q_OBJECT
public:
    explicit HttpRequest(QObject *parent = 0);
    /**Send a post request, return data store in QByteArray style*/
    QByteArray sendPost(const QString& url, const QString& params);

    /**Send a get request, return data store in QByteArray style*/
    QByteArray sendGet(const QString& url, const QString& parmas);

Q_SIGNALS:
    void request_timeout();
    void request_success(int retcode, const QString & text);

private slots:
    void slot_request_timeout();
    void slot_request_finished();

private:
    QNetworkAccessManager* m_pNetworkManager;//网络管理类
    QNetworkReply* m_pNetworkReply; //封装请求返回信息
    QTimer* m_pTimer; //请求超时计时器
    QString m_strUrl; //记录当前请求的url
};

#endif // POSTHTTPREQUEST

