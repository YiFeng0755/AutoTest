#ifndef GLOBALSETTING
#define GLOBALSETTING

#include <QObject>

class GlobalSettings : public QObject
{
    Q_OBJECT
public:
    static GlobalSettings* getInstance();

    QByteArray getIMAppkey(bool test=true);
    QByteArray getIMAppSecret(bool test=true);
    QByteArray getTalkAppKey(bool test=true);
    QByteArray getTalkAppSecret(bool test=true);
    QByteArray getConfigDir();

    QString getHost(bool online = false);

    void setIMAppkey(const QByteArray &appkey, bool test=true);
    void setIMAppSecret(const QByteArray &appsec, bool test=true);
    void setTalkAppSecret(const QByteArray &appkey, bool test=true);
    void setTalkAppkey(const QByteArray &appkey, bool test=true);

private slots:
    void loadSettings();
    void saveSettings();

private:
    GlobalSettings();

    static GlobalSettings *_instance;

    QByteArray m_im_appkey;
    QByteArray m_im_appsecret;
    QByteArray m_im_appkey_online;
    QByteArray m_im_appsecret_online;

    QByteArray m_talk_appkey;
    QByteArray m_talk_appsecret;
    QByteArray m_talk_appkey_online;
    QByteArray m_talk_appsecret_online;
};

#endif // GLOBALSETTING

