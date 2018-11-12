#ifndef HOSTSMANAGER
#define HOSTSMANAGER
#include <QObject>
#include <QHash>
#include <QFileSystemWatcher>
#include <QDebug>

class HostManager : public QObject
{
    Q_OBJECT  //中途添加此物要小心
public:
    struct IP {
       bool enable  ;
       QString ip ;
    };

    explicit HostManager(QObject * parent = 0);
    virtual ~HostManager();

    QString path() const {
        return m_host_directory ;
    }

    /** Return the ip of the  given host
     *
     * */
    QString ip(const QString & host) const ;

    /** Return the hostnames of the given ip **/
    QStringList hosts(const QString & ip) const ;

    bool setHost(const QString & hostname,QString ip);
    /*Change the host ip ;If the host doesn't exsits,append it on host file */

    QHash<QString,IP> & ipHosts();

    bool enableHost(const QString & host);

    bool disableHost(const QString & host);

    QString readAllConf()  ;

    //bool checkValid();

    /*Save file by hash data*/
    void saveDataToDisk();

    /**Save file by given text*/
    void saveDataToDisk(const QString & file);

Q_SIGNALS:
    void dataChanged();
private slots:
    void readDataFromDisk();

private:
    void initialize();

    QHash<QString,IP> m_iphosts ;
    QString m_host_directory ;

    QByteArray m_md5Value ;
};

#endif // HOSTSMANAGER

