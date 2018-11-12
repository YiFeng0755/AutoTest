#include <QSettings>
#include "globalsetting.h"

GlobalSettings* GlobalSettings::_instance = nullptr;

GlobalSettings* GlobalSettings::getInstance() {
    if (nullptr == _instance) {
        _instance = new GlobalSettings();
    }
    return _instance;
}

GlobalSettings::GlobalSettings()
{
    //m_im_appkey = ""; // default value
    //m_im_appsecret = "";
    //m_talk_appkey = "";
    //m_talk_appsecret = "";
}

void GlobalSettings::loadSettings()
{
    QSettings settings("config.ini", QSettings::IniFormat);
    settings.beginGroup("AppConfig");
    m_im_appkey = settings.value("appkey_for_im").toByteArray();
    m_im_appsecret = settings.value("appsecret_for_im").toByteArray();
    m_talk_appkey = settings.value("appkey_for_talk").toByteArray();
    m_talk_appsecret = settings.value("appsecret_for_talk").toByteArray();

    m_im_appkey_online = settings.value("appkey_for_im_online").toByteArray();
    m_im_appsecret_online = settings.value("appsecret_for_im_online").toByteArray();
    m_talk_appkey_online = settings.value("appkey_for_talk_online").toByteArray();
    m_talk_appsecret_online = settings.value("appsecret_for_talk_online").toByteArray();
    settings.endGroup();
}

void GlobalSettings::saveSettings()
{
    //此处将内存中的变量值持久化到磁盘之中
    QSettings settings("config.ini", QSettings::IniFormat);
    settings.beginGroup("AppConfig");
    settings.setValue("appkey_for_im", QVariant(m_im_appkey));
    settings.setValue("appsecret_for_im", QVariant(m_im_appsecret));
    settings.setValue("appkey_for_talk", QVariant(m_talk_appkey));
    settings.setValue("appsecret_for_talk", QVariant(m_talk_appsecret));

    settings.setValue("appkey_for_im_online", QVariant(m_im_appkey_online));
    settings.setValue("appsecret_for_im_online", QVariant(m_im_appsecret_online));
    settings.setValue("appkey_for_talk_online", QVariant(m_talk_appkey_online));
    settings.setValue("appsecret_for_talk_online", QVariant(m_talk_appsecret_online));
    settings.endGroup();
}

QByteArray GlobalSettings::getConfigDir() {
    return "";
}

QByteArray GlobalSettings::getIMAppkey(bool test) {
    if (m_im_appkey.isEmpty() || m_im_appkey_online.isEmpty()) {
        loadSettings();
    }
    return test ? m_im_appkey : m_im_appkey_online;
}

QByteArray GlobalSettings::getIMAppSecret(bool test) {
    if (m_im_appsecret.isEmpty() || m_im_appkey_online.isEmpty()) {
        loadSettings();
    }
    return test ? m_im_appsecret: m_im_appsecret_online;
}

QByteArray GlobalSettings::getTalkAppKey(bool test) {
    if (m_talk_appkey.isEmpty() || m_talk_appkey_online.isEmpty()) {
        loadSettings();
    }
    return test ? m_talk_appkey : m_talk_appkey_online;
}

QByteArray GlobalSettings::getTalkAppSecret(bool test) {
    if (m_talk_appsecret.isEmpty()) {
        loadSettings();
    }
    return test ? m_talk_appsecret : m_talk_appsecret_online;
}

void GlobalSettings::setIMAppkey(const QByteArray &appkey, bool test) {
    if (test) {
       m_im_appkey = appkey;
    } else {
       m_im_appkey_online = appkey;
    }

    saveSettings();
}

void GlobalSettings::setIMAppSecret(const QByteArray &appsec, bool test) {
    if (test) {
        m_im_appsecret = appsec;
    } else {
        m_im_appsecret_online = appsec;
    }
    saveSettings();
}

void GlobalSettings::setTalkAppSecret(const QByteArray &appsec, bool test) {
    if (test) {
        m_talk_appsecret = appsec;
    } else {
        m_talk_appsecret_online = appsec;
    }

    saveSettings();
}

void GlobalSettings::setTalkAppkey(const QByteArray &appkey, bool test) {
    if (test) {
        m_talk_appkey = appkey;
    } else {
        m_talk_appkey_online = appkey;
    }

    saveSettings();
}

QString GlobalSettings::getHost(bool online)
{
    if (online) {
        return QString("http://api.youme.im/");
    }
    //return QString("http://api.youme.im");
    return QString("http://106.75.14.78:18080/");
}
