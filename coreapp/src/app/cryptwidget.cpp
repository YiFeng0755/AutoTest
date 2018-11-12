#include "cryptwidget.h"
#include "cryptutil.h"
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QLayout>
#include <QGridLayout>
#include <QDateTime>
#include <QLabel>
#include <QCheckBox>
#include <QSettings>


CryptDialog::CryptDialog(QWidget *parent)
    :QDialog(parent)
{
    setWindowTitle(tr("Crypt a text"));
    setMinimumSize(600, 400);
    init();
}

void CryptDialog::init()
{
    m_algorithmList = new QComboBox();
    m_timestampStyleList = new QComboBox();
    setupCryptOptionCombo();
    setupTimestampCombo();

    m_preText_lineedit = new QLineEdit ;
    m_mainText_lineedit = new QLineEdit ;
    m_timestamp_lineedit = new QLineEdit ;
    m_timeSwitcher = new QCheckBox ;
    m_timeSwitcher->setCheckState(Qt::Checked);
    connect(m_timeSwitcher, SIGNAL(clicked(bool)), this, SLOT(updateTimeEidtLook()));

    m_source_edit = new QLineEdit ;

    m_result_edit = new QLineEdit ;
    m_result_edit->setReadOnly(true);

    QLabel * pretextLabel = new QLabel(tr("Prefix Text"));
    pretextLabel->setBuddy(m_preText_lineedit);
    QLabel * mainTextLabel = new QLabel(tr("Main Text"));
    mainTextLabel->setBuddy(m_mainText_lineedit);
    QLabel * sourceTextLabel = new QLabel(tr("Source Text"), this);
    sourceTextLabel->setBuddy(m_source_edit);
    QLabel * resultTextLabel = new QLabel(tr("Result Text"), this);
    resultTextLabel->setBuddy(m_result_edit);
    QLabel * algorithmOptionsLabel = new QLabel(tr("Algorithm options"));
    algorithmOptionsLabel->setBuddy(m_algorithmList);
    QLabel * timeStyleOptionsLablel = new QLabel(tr("Time format"));
    timeStyleOptionsLablel->setBuddy(m_timestampStyleList);
    QLabel * timestampLabel = new QLabel(tr("Timestamp"));
    timestampLabel->setBuddy(m_timestamp_lineedit);

    QDateTime datetime = QDateTime::currentDateTime();
    m_timestamp_lineedit->setText(QString::number(datetime.toMSecsSinceEpoch()));
    m_timestamp_lineedit->setReadOnly(true);

    QPushButton * cryptButton = new QPushButton(tr("&Generate"));
    connect(cryptButton, SIGNAL(clicked(bool)), this, SLOT(cryptResult()));

    QPushButton * updateTimestampButton = new QPushButton(tr("RefreshTime"));
    connect(updateTimestampButton, SIGNAL(clicked(bool)), this, SLOT(refreshTime()));

    QPushButton *closeButton = new QPushButton(tr("&Close"));
    connect(closeButton, SIGNAL(clicked(bool)), this, SLOT(close()));

    QGridLayout * gridlayout = new QGridLayout ;
    gridlayout->addWidget(pretextLabel, 0, 0, 1, 1);
    gridlayout->addWidget(m_preText_lineedit, 0, 1, 1, 3);
    gridlayout->addWidget(mainTextLabel, 1, 0, 1, 1);
    gridlayout->addWidget(m_mainText_lineedit, 1, 1, 1, 3 );
    gridlayout->addWidget(timestampLabel, 2, 0, 1, 1 );
    gridlayout->addWidget(m_timestamp_lineedit, 2, 1, 1, 3 );
    gridlayout->addWidget(algorithmOptionsLabel, 0, 4, 1, 1);
    gridlayout->addWidget(m_algorithmList, 0, 5, 1, 1);
    gridlayout->addWidget(timeStyleOptionsLablel, 1, 4, 1, 1);
    gridlayout->addWidget(m_timestampStyleList, 1, 5, 1, 1 );
    gridlayout->addWidget(m_timeSwitcher, 2, 4, 1, 1);
    gridlayout->addWidget(updateTimestampButton, 2, 5, 1, 1 );

    QHBoxLayout * hLayout = new QHBoxLayout ;
    hLayout->addWidget(cryptButton);
    hLayout->addStretch();
    hLayout->addWidget(closeButton);

    QVBoxLayout * vLayout = new QVBoxLayout ;
    QFrame * frame = new QFrame ;
    frame->setFrameShape(QFrame::HLine);
    frame->setFrameShadow(QFrame::Sunken);

    vLayout->addLayout(gridlayout);
    vLayout->addWidget(frame);

    QHBoxLayout * sourceLayout = new QHBoxLayout ;
    sourceLayout->addWidget(sourceTextLabel);
    sourceLayout->addWidget(m_source_edit, Qt::AlignLeft);
    vLayout->addLayout(sourceLayout);

    QHBoxLayout * resultLayout = new QHBoxLayout ;
    resultLayout->addWidget(resultTextLabel );
    resultLayout->addWidget(m_result_edit, Qt::AlignLeft);

    vLayout->addLayout(resultLayout);
    vLayout->addStretch();
    vLayout->addLayout(hLayout);

    setLayout(vLayout);
 }


void CryptDialog::refreshTime()
{
    qint8 data = m_timestampStyleList->currentData().toInt();

    QDateTime currentDateTime = QDateTime::currentDateTime();
    switch (data){
    case MSecs:
        m_timestamp_lineedit->setText(QString::number(currentDateTime.toMSecsSinceEpoch()));
        break;
    case Secs:
        m_timestamp_lineedit->setText(QString::number(currentDateTime.toTime_t()));
        break ;
    case YYMMDD:
        m_timestamp_lineedit->setText(currentDateTime.toString("yy-MM-dd"));
        break ;
    case YYMMDDHHMMSS:
        m_timestamp_lineedit->setText(currentDateTime.toString("yy-MM-dd hh:mm:ss"));
        break ;
    default:
        m_timestamp_lineedit->setText(currentDateTime.toString(Qt::TextDate));
        break;
    }
}

void CryptDialog::setupCryptOptionCombo()
{
    QStringList items ;
    items << "Md5" << "Sha1" << "Md4"
          << "Sha224" << "Sha256"<< "Sha384"
          << "Sha512" << "base64" << "DES" << "AES";
    m_algorithmList->addItems(items);
}

void CryptDialog::setupTimestampCombo()
{
    m_timestampStyleList->addItem(tr("msecs"), MSecs);
    m_timestampStyleList->addItem(tr("secs"), Secs);
    m_timestampStyleList->addItem( tr("yyMMdd"), YYMMDD);
    m_timestampStyleList->addItem( tr("yyMMddhhmmss"), YYMMDDHHMMSS);
    connect(m_timestampStyleList, SIGNAL(currentIndexChanged(int)), this, SLOT(refreshTime()));
}

void CryptDialog::updateTimeEidtLook()
{
    m_timestamp_lineedit->setEnabled(
                m_timeSwitcher->checkState()== Qt::Checked);
}

void CryptDialog::cryptResult()
{
    QByteArray combineText = (m_preText_lineedit->text()+
            m_mainText_lineedit->text()+ (m_timeSwitcher->checkState()==
                              Qt::Checked? m_timestamp_lineedit->text():
                                           "")).toStdString().c_str();
    m_source_edit->setText(QString(combineText));

    int index = m_algorithmList->currentIndex();

    QCryptographicHash::Algorithm Alg = QCryptographicHash::Md5 ;
    if (index == 1) {
        Alg = QCryptographicHash::Md5 ;
    }else if (index == 2) {
        Alg = QCryptographicHash::Sha1 ;
    }else if (index == 3) {
        Alg = QCryptographicHash::Md4 ;
    }else if (index == 4) {
        Alg = QCryptographicHash::Sha224 ;
    }else if (index == 5) {
        Alg = QCryptographicHash::Sha256 ;
    }else if (index == 6) {
        Alg = QCryptographicHash::Sha384 ;
    }else if (index == 7) {
        Alg = QCryptographicHash::Sha512 ;
    }else if (index == 8) {
        //base64
        QByteArray result = CryptAdaptor::toBase64(combineText);
        m_result_edit->setText(result);
        return;
    }else if (index == 9) {

    }


    QByteArray result = CryptAdaptor::hashData(combineText, Alg, true);// hard code
    m_result_edit->setText(QString(result));
}

