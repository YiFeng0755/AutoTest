#ifndef REQUESTWIDGET_H
#define REQUESTWIDGET_H


#include <QWidget>
#include <QStack>
#include <QSet>
#include <QNetworkReply>

class QLabel;
class QPushButton;
class QLineEdit;
class QTextEdit;
class QComboBox;
class QCheckBox;
class QNetworkAccessManager;
class QNetworkReply;
class QNetworkRequest;
class RequestWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RequestWidget(QWidget *parent = 0);
    virtual ~RequestWidget();

private:
    void setup();
    void setupComboBox();
    void storeHistoryUrl(const QString& url);

private slots:
    //void slot_restmethod_changed(QString);
    void slot_replyFinished(QNetworkReply* reply);
    void slot_request();
    void slot_finished();
    void slot_error(QNetworkReply::NetworkError);
    void loadSettings();
    void saveSettings();

    void updateCurrentUrl(QString url);

private:
    void httpsRequestSetting(QNetworkRequest *request);
    void initHistoryRecords();
    void saveItemToDb(const QString& uri, const QVariant& data);

private:
    QPushButton *m_btnrequest;
    QLabel *m_hostLabel;
    //QLabel *m_portLabel;
    QLabel *m_methodLabel;
    QLabel *m_bodyLabel;

    QLineEdit *m_urleidt;
    QTextEdit *m_bodyedit;
    QTextEdit *m_outputEdit;
    QComboBox *m_restmethods_combo;
    QCheckBox *m_youmecrypt_checkbox;
    QComboBox *m_youmeV_combobox;
    QComboBox *m_history_records_combo;
    QNetworkReply *m_reply;
    QNetworkAccessManager *m_net_manager;

    QSet<QString> m_history_urls;
};

#endif // REQUESTWIDGET_H
