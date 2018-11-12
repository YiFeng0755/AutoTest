#include "hostsmanager.h"
#include <QFile>
#include <QDebug>
#include <QCryptographicHash>
#include <QDir>
#include <QFileSystemWatcher>

HostManager::HostManager(QObject * parent)
    : QObject(parent)
{
    initialize();
}

HostManager::~HostManager()
{

}

void HostManager::initialize()
{
    m_host_directory = "";
#ifdef Q_OS_MAC
    m_host_directory = "/etc/hosts" ;
#endif

#ifdef Q_OS_LINUX
    m_host_directory = "/etc/hosts" ;
#endif

#ifdef Q_OS_WIN
    m_host_directory = "C:/System32/etc/hosts" ;
#endif

    if(m_host_directory.isEmpty()){
        qDebug() << "The file name is empty ." ;
        return ;
    }
    // Read file conten from disk ;

    QCryptographicHash hashObj(QCryptographicHash::Md5);
    QFile file(m_host_directory);
    hashObj.addData(&file);
    m_md5Value = hashObj.result().toHex();//Save the file's md5 value

    QFileSystemWatcher watcher(this);
    watcher.addPath(m_host_directory);
    connect(&watcher,SIGNAL(fileChanged(QString)),this,SLOT(readDataFromDisk()));

    readDataFromDisk();
}

QString HostManager::readAllConf()
{
    QFile file(m_host_directory);
    if(!file.open(QIODevice::ReadOnly)) {
        qDebug() << file.errorString();
        return "" ;
    }

    return file.readAll() + m_host_directory ;
}

void HostManager::readDataFromDisk()
{
    QFile file(m_host_directory);

    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << file.errorString();
        return ;
    }

    m_iphosts.clear();
    QString line ;
    while(!file.atEnd()) {
        line = file.readLine();
        line = line.trimmed();
        line.replace("\n","");

        int pos = line.indexOf(" ");
        if (pos == -1) {
            //qDebug() << "invalid line here" ;
            continue ;
        }
        QString value = line.left(pos);
        QString key = line.right(line.size() - 1 - pos);

        IP tmp_ip ;
        tmp_ip.ip = value.replace("#","");
        tmp_ip.enable = !line.startsWith("#");
        m_iphosts.insert(key,tmp_ip);
    }
    file.close();
    emit dataChanged();
}

void HostManager::saveDataToDisk()
{
    /**
     * m_iphosts data save to disk
     */
    QFile file(m_host_directory);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << file.errorString();
        return ;
    }

    QTextStream in(&file);

    QHashIterator<QString,IP > iter(m_iphosts);
    //Write data to disk
    QString allStr ;
    while(iter.hasNext()) {
        iter.next();
        if(!iter.value().enable)
            allStr += '#' ;
        allStr += iter.key();
        allStr += " " ;
        allStr += iter.value().ip ;
        allStr += "\n" ;
    }

    QFile backfile(file.fileName());
    backfile.rename(file.fileName() + '~');
    file.seek(0);
    in << allStr ;
    backfile.remove();
    file.close();
}

void HostManager::saveDataToDisk(const QString & text)
{
    QFile file(m_host_directory);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << file.errorString();
        return ;
    }
    QTextStream in(&file);
    file.seek(0);
    in << text ;
    file.close();
}

QHash<QString,HostManager::IP> & HostManager::ipHosts() {
    return m_iphosts ;
}

QString HostManager::ip(const QString &host) const
{
    IP ip = m_iphosts.value(host);
    qDebug() << host ;
    return ip.ip ;
}

QStringList HostManager::hosts(const QString &ip) const
{
    QStringList hostList ;
    foreach (QString host, m_iphosts.keys()) {
        IP tip = m_iphosts.value(host);
        if(tip.ip == ip)
            hostList << host ;
    }
    return hostList ;
}

bool HostManager::enableHost(const QString & host)
{
    IP ip = m_iphosts[host] ;
    if(ip.ip == "0.0.0.0") {
        return false ;
    }
    ip.enable = true ;
    return true ;
}

bool HostManager::disableHost(const QString & host)
{
    IP ip = m_iphosts[host] ;
    if(ip.ip == "0.0.0.0" ) {
        return false ;
    }

    ip.enable = false ;
    return true ;
}

bool HostManager::setHost(const QString &hostname, QString ip)
{
    QFile file(m_host_directory);
    if(!file.open(QIODevice::Append | QIODevice::Text | QIODevice::ReadWrite)){
        qDebug() << "Open file failed ." ;
        return false ;
    }

    QHash<QString,IP >::Iterator it = m_iphosts.find(hostname);
    if(it == m_iphosts.end()) {
        //Apend at the end
        QTextStream out(&file);
        out << hostname <<' '<<ip ;
        file.close();
        return true ;

    }else{
        //Modified in the right position ;
        QString text = file.readAll();

        if(it.value().ip == ip ) {
            if(!it.value().enable) {
                text.replace(QRegExp(QString("^#").append(ip).append("\' \'+").
                                     append(hostname+"$")),QString("ip").append(" ").append(hostname));
            }
        }else{
            text.replace(QRegExp(QString("^#").append(it.value().ip).
                                 append("' '+").append(hostname+"$")),QString("ip").append(" ").append(hostname));
            IP local_ip ;
            local_ip.ip = ip ;
            local_ip.enable = true ;
            m_iphosts.insert(hostname,local_ip);
        }

        QFile backupFile(file.fileName());
        backupFile.rename(file.fileName() + "~");
        QTextStream out(&file);
        file.seek(0);
        out << text ;
        backupFile.remove();
        file.close();
        return true ;
    }
}
