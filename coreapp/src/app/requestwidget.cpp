#include <QLineEdit>
#include <QLabel>
#include <QTextBlock>
#include <QTextEdit>
#include <QPushButton>
#include <QComboBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <qnetworkrequest.h>
#include <QDebug>
#include <QSettings>
#include <QCompleter>
#include <QLayout>
#include <QJsonArray>
#include <QHttpMultiPart>
#include <QSsl>
#include <QSslConfiguration>
#include <QCheckBox>
#include <QStringList>

#include "database.h"
#include "requestwidget.h"
#include "cryptutil.h"
#include "globalsetting.h"


RequestWidget::RequestWidget(QWidget *parent)
    :QWidget(parent)
{
    setup();
    loadSettings();
}

RequestWidget::~RequestWidget()
{
    saveSettings();
}

void RequestWidget::saveSettings()
{
    qDebug() << "[Youme] save settings.";
    QSettings settings("config.ini", QSettings::IniFormat);
    settings.beginGroup("historyUrl");
    QStringList urls;
    foreach (QString url, m_history_urls) {
        urls << url;
    }
    settings.setValue("historyhost", QVariant(urls));
    settings.endGroup();
}

void RequestWidget::storeHistoryUrl(const QString &url)
{
    const int max_record_num = 100;

    saveItemToDb(url, m_bodyedit->document()->toPlainText());

    if (m_history_urls.find(url) != m_history_urls.end()) {
        int x = m_history_records_combo->findText(url);
        if (x != -1) {
            m_history_records_combo->setItemData(x, m_bodyedit->document()->toPlainText());
        }
    } else {
        m_history_urls.insert(url);
    }

    QString first_url = *(m_history_urls.begin());
    if (m_history_urls.size() > max_record_num) {
        m_history_urls.remove(first_url);
    }
    m_history_records_combo->addItem(url, m_bodyedit->document()->toPlainText());
}

void RequestWidget::saveItemToDb(const QString &url, const QVariant &data)
{
    //http://www.baidu.com?a=1
    QString sections;
    QString copy = url;
    if (url.startsWith("http://"))
        sections = copy.remove(0,7);
    if (url.startsWith("https://"))
        sections = copy.remove(0,8);

    QString uri;

    if (!SqlManager::createInstance()->initDb()){
        return;
    }

    QSqlQuery query;
    int i = uri.indexOf("/");
    if (i == -1) {
        uri = url;
    } else {
        uri = url.right(url.size() - i);
    }

    QString qstatement = QString("select url from actions where url = '%1'").arg(url);

    if (!query.exec(qstatement)) {
        qDebug() << query.lastError();
        return;
    }
    query.next();
    QString s = query.record().value("url").toString();
    if (!s.isEmpty()) {
        //update
        QString statement = QString("update actions set name = '%1', args='%2' where url = '%3'").arg(uri)
                .arg(data.toString()).arg(url);
        qDebug() << "Exec" << statement;
        if (!query.exec(statement)) {
            qDebug() << query.lastError();
        }
    } else {
        //insert
        QString statement = QString("insert into actions (name, url, args) values ('%1', '%2', '%3')")
                .arg(uri)
                .arg(url)
                .arg(data.toString());
        //debug statement
        qDebug() << "Exec sql:" << statement;

        if (!query.exec(statement)) {
            qDebug() << query.lastError();
        }
    }
}

void RequestWidget::loadSettings()
{
    QStringList wordlist;

    QSettings settings("config.ini", QSettings::IniFormat);
    settings.beginGroup("historyUrl");
    wordlist = settings.value("historyhost").toStringList();
    QCompleter *completer = new QCompleter(wordlist, this);
    completer->setCompletionMode(QCompleter::PopupCompletion);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    m_urleidt->setCompleter(completer);

    if (!wordlist.isEmpty()) {
        m_urleidt->setText(wordlist[0]);
    }
    settings.endGroup();

//    settings.beginGroup("AppConfig");
//    m_appKey = GlobalSettings::getInstance()->getIMAppkey(true);
//    m_appsecret = settings.value("appsecret_for_im").toByteArray();


//    if (m_appKey.isEmpty()) {
//        m_appKey = "YOUMEAA5EE5689436B39DC46E6195FF45F81CD46A1804";
//        settings.setValue("appkey_for_im", m_appKey);
//    }

//    if (m_appsecret.isEmpty()) {
//        m_appsecret = "3hzCOsAPitudP7DiQN7ANrkbnTpEVdm0KJ1fFNmXzwL6BZTfEGwfBU4W2ef"
//                      "hnAxkx11idqN60lIJ26KkRhBrBQcgui8SahalzxtVv+hKHvDhg/KTMhmal8tuknnAcxlWkq7102ZG3EM6loBPCMp6t96078W7XCWNoszlzPxT6w0BAAE=";
//        settings.setValue("appsecret_for_im", m_appsecret);
//    }
//    settings.endGroup();
}

void RequestWidget::setup()
{
    m_hostLabel = new QLabel(tr("Url:"), this);
    m_methodLabel = new QLabel(tr("Methods:"), this);
    m_bodyLabel = new QLabel(tr("Body:"), this);

    m_btnrequest = new QPushButton(tr("request"), this);
    m_net_manager = new QNetworkAccessManager(this);

    m_urleidt = new QLineEdit(this);
    m_urleidt->setText("http://");

    m_bodyedit = new QTextEdit(this);

    m_outputEdit = new QTextEdit(this);

    m_youmecrypt_checkbox = new QCheckBox(this);
    m_youmecrypt_checkbox->setChecked(true);
    m_youmecrypt_checkbox->setText(tr("Crypt"));

    m_youmeV_combobox = new QComboBox(this);
    m_youmeV_combobox->addItem(tr("v1"));
    m_youmeV_combobox->addItem(tr("v2"));

    m_history_records_combo = new QComboBox(this);
    connect(m_history_records_combo, SIGNAL(currentTextChanged(QString)),
            this, SLOT(updateCurrentUrl(QString)));
    initHistoryRecords();

    connect(m_net_manager, SIGNAL(finished(QNetworkReply*)), this,
            SLOT(slot_replyFinished(QNetworkReply*)));
    connect(m_btnrequest, SIGNAL(clicked()), this,
            SLOT(slot_request()));

    m_restmethods_combo = new QComboBox(this);
    setupComboBox();

    m_hostLabel->setBuddy(m_urleidt);
    //m_portLabel->setBuddy(m_portedit);
    m_methodLabel->setBuddy(m_restmethods_combo);
    m_bodyLabel->setBuddy(m_bodyedit);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->addWidget(m_hostLabel, 1, 0, 1, 1);
    gridLayout->addWidget(m_youmecrypt_checkbox, 1, 5, 1, 1);
    gridLayout->addWidget(m_history_records_combo, 0, 1, 1, 4);
    gridLayout->addWidget(m_youmeV_combobox, 3, 5, 1, 1);
    //gridLayout->addWidget(m_portLabel, 1, 0, 1, 1);
    gridLayout->addWidget(m_methodLabel, 3, 0, 1, 1);
    gridLayout->addWidget(m_bodyLabel, 4, 0, 1, 1);
    gridLayout->addWidget(m_urleidt, 1, 1, 1, 4);
    gridLayout->addWidget(m_restmethods_combo, 3, 1, 1, 4);
    gridLayout->addWidget(m_bodyedit, 4, 1, 6, 4);

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(m_btnrequest, 0, Qt::AlignRight);

    mainLayout->addLayout(gridLayout);
    mainLayout->addWidget(m_outputEdit);
    mainLayout->addLayout(hLayout);

    QFrame *hline = new QFrame;
    hline->setFrameShadow(QFrame::Sunken);
    hline->setFrameShape(QFrame::HLine);
    mainLayout->addWidget(hline);

    setLayout(mainLayout);
}

void RequestWidget::updateCurrentUrl(QString url)
{
    QString current_url = m_history_records_combo->currentText();
    if (!current_url.isEmpty()) {
        m_urleidt->setText(current_url);
    } else {
        m_urleidt->setText(url);
    }
    QString body = m_history_records_combo->currentData().toString();

    m_bodyedit->setText(body);
}

void RequestWidget::initHistoryRecords()
{
   SqlManager *inst = SqlManager::createInstance();
   if (!inst || !inst->initDb())
       return;

//   QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
//   db.setDatabaseName("youmedb.db");

   QString sql_statement = QString("select count(*) as c from actions");
   QSqlQuery query;
   if(!query.exec(sql_statement)) {
       qDebug() << "load data failed" << query.lastError();
       return;
   }
   query.next();//must
   QSqlRecord record = query.record();
   int count = record.value(0).toInt();

   int b = 0;
   for (; b < count;) {
       int e = b + 100;
       QString statement = QString("select * from actions where id >= %1 and id < %2").arg(b).arg(e);
       if (!query.exec(statement)) {
           qDebug() << query.lastError();
           continue;
       }

       while (query.next()) {
           QSqlRecord record = query.record();
           m_history_records_combo->addItem(record.value("url").toString(),
                                            record.value("args"));
       }
       b += 100;
   }
}

void RequestWidget::setupComboBox()
{
    QStringList methods;
    methods << "POST" << "GET" << "PUT" << "DELETE" << "TRACE";
    m_restmethods_combo->setAutoCompletion(true);
    m_restmethods_combo->addItems(methods);
}

void RequestWidget::slot_request()
{
    m_outputEdit->clear();
    QString currentText = m_restmethods_combo->currentText().trimmed().toLower();
    if (currentText.isEmpty()) {
        qDebug() << tr("Request method is empty.");
        return;
    }
    QString host = m_urleidt->text().toLower();

    if (host.isEmpty()) {
        qDebug() << "Host is empty";
        return;
    }

    QUrl url(host);
    storeHistoryUrl(url.toString());

    m_reply = nullptr;

    if (currentText == "get") {
        //QByteArray params;
        if (!url.isValid()) {
            qDebug() << "Url:" << url.toString() << " is invalid!";
            return;
        }
        QNetworkRequest request(url);

        if (host.startsWith("https")) {
            QSslConfiguration sslconfig;
            sslconfig.setProtocol(QSsl::TlsV1SslV3);
            sslconfig.setPeerVerifyMode(QSslSocket::VerifyNone);
            request.setSslConfiguration(sslconfig);
        }

        m_reply = m_net_manager->get(request);
    } else if (currentText == "post") {
        QString curtime = QString::number(QDateTime::currentDateTime().toTime_t(), 10);
        QByteArray appkey = GlobalSettings::getInstance()->getIMAppkey();
        QByteArray appsect= GlobalSettings::getInstance()->getIMAppSecret();

        //Default value setting
        if (appkey.isEmpty()) appkey = "YOUMEAA5EE5689436B39DC46E6195FF45F81CD46A1804";
        //if (appkey.isEmpty())
        //    appkey = "YOUME00AD8574CEB27E309DBBEA4A3BA29B6A2FB804CB";

//        if (appsect.isEmpty())
//            appsect = "3hzCOsAPitudP7DiQN7ANrkbnTpEVdm0KJ1fFNmXzwL6BZTfEGwfBU4W2efhnAxkx11idqN60lIJ26KkRhBrBQcgui8SahalzxtVv+hKHvDhg/K"
//                      "TMhmal8tuknnAcxlWkq7102ZG3EM6loBPCMp6t96078W7XCWNoszlzPxT6w0BAAE=";

        //if (appsect.isEmpty())
        //    appsect = "zdDTMqLwVm9FTAT/ATIJ0fa2e4DPdtlzSrWu0z+mYHXYElO1ROJGuU4v1/AgRLRNEJ0uxgwEdfH6gKGLdH3N5wCXhPv7zJHJYgPYlTl5r1I8MzBOU3SLw3HXGPxcealhk4jGwSnUO1+oaaa9WZik9mFP4NEoAMkpM5gRnfrpODkBAAE=";

        QByteArray checksum = "";
        if (m_youmeV_combobox->currentText() == "v2") {
            const QByteArray MD5_SALT = "YOUME8140";
            QByteArray restapi_key = CryptAdaptor::cryptMd5(appkey + MD5_SALT, true);
            //qDebug() << restapi_key;
            checksum = CryptAdaptor::cryptSha1(appsect + restapi_key + curtime.toLatin1(), true);
        } else if (m_youmeV_combobox->currentText() == "v1") {
            checksum = CryptAdaptor::cryptSha1(appsect + curtime.toLatin1(), true);
        }

        QString cryt_data;

        cryt_data += "?";
        cryt_data += QString("appkey=%1&identifier=admin&curtime=%2&checksum=%3")
                .arg(QString(appkey))
                .arg(curtime)
                .arg(QString(checksum));
        QString strUrl = host;
        if (m_youmecrypt_checkbox->isChecked()) {
            strUrl += cryt_data;
        }

        qDebug() << "Request: " << strUrl;
        url.setUrl(strUrl);
        if (!url.isValid()) {
            qDebug() << tr("Url: %1 is not valid").arg(url.toString());
            return;
        }
        QString data = m_bodyedit->document()->toPlainText();

        QNetworkRequest request;
        request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));
        if (host.startsWith("https")) {
            QSslConfiguration sslconfig;
            sslconfig.setProtocol(QSsl::TlsV1_2);
            sslconfig.setPeerVerifyMode(QSslSocket::VerifyNone);
            request.setSslConfiguration(sslconfig);
        }
        request.setUrl(url);
        //for (int i =0 ; i < 200; ++i) {
        m_reply = m_net_manager->post(request, data.toUtf8());
        //m_reply = m_net_manager->post(request, data.toLatin1());
        //}

    } else if (currentText == "put") {

    } else {
        // to do
    }
    if (m_reply) {
        connect(m_reply, SIGNAL(finished()), this, SLOT(slot_finished()));
        connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)),
                this, SLOT(slot_error(QNetworkReply::NetworkError)));
    }
}

void RequestWidget::slot_error(QNetworkReply::NetworkError errorCode)
{
    QNetworkReply *pReplay = qobject_cast<QNetworkReply*>(sender());
       // 输出错误码及错误信息;
       qDebug()<< errorCode;
       qDebug()<< pReplay->errorString();
}

void RequestWidget::httpsRequestSetting(QNetworkRequest *request)
{
    if (!request)
        return;

    QSslConfiguration config = QSslConfiguration::defaultConfiguration();
    config.setProtocol(QSsl::TlsV1_1);
    config.setPeerVerifyMode(QSslSocket::VerifyNone);
    request->setSslConfiguration(config);
}

void RequestWidget::slot_replyFinished(QNetworkReply *reply)
{
    m_outputEdit->clear();
    if (!reply)
        return;
    quint32 once_read_cnt = 1024;
    //QTextDocument *doc = new QTextDocument(this);
    //QString alltext = "";
    while (reply->bytesAvailable()) {
        QByteArray data = reply->read(once_read_cnt);
        m_outputEdit->append(data);
        qDebug() << data;
    }
}

void RequestWidget::slot_finished()
{
    qDebug() << "request finished!";
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    QByteArray content = reply->readAll();
    qDebug() << content;
}


