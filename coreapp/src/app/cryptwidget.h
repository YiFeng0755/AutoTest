#ifndef CRYPTWIDGET_H
#define CRYPTWIDGET_H

#include <QDialog>

class QComboBox;
class QLineEdit ;
class QCheckBox ;
class CryptDialog : public QDialog
{
    Q_OBJECT

    enum TimeStyle
    {
        MSecs,
        Secs,
        YYMMDD,
        YYMMDDHHMMSS
    };

public:
    explicit CryptDialog(QWidget * parent = 0);

private slots:
    void refreshTime();
    void cryptResult();
    void updateTimeEidtLook();
private:
    void init();
    void setupTimestampCombo();
    void setupCryptOptionCombo();

    QComboBox * m_algorithmList ;
    QComboBox * m_timestampStyleList ;
    QLineEdit * m_preText_lineedit;
    QLineEdit * m_mainText_lineedit ;
    QLineEdit * m_timestamp_lineedit ;

    QLineEdit * m_source_edit ;
    QLineEdit * m_result_edit ;

    QCheckBox * m_timeSwitcher ;
};

#endif // CRYPTWIDGET_H
