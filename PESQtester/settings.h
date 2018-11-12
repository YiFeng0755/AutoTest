#ifndef SETTINGS
#define SETTINGS

#include <QSettings>
#include <QDialog>

namespace Ui {
    class SettingForm;
}

namespace Ui {
    class CmdsForm;
}

class CmdsWidget : public QWidget
{
    Q_OBJECT
public:
    CmdsWidget(QWidget *parent = 0);

public slots:
    void add_to_list();
    void remove_from_list();

    void apply();

private:
    void setupList();

    friend class SettingDialog;
    Ui::CmdsForm *ui;
};

class SettingDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SettingDialog(QWidget *parent = 0);

    QStringList commands();

    void accept();
private:
    void setupCmdWidget();

private:

    Ui::SettingForm *ui;
};

class GlobalSettings : public QObject
{
    Q_OBJECT
public:
    QStringList getCommandList() {
        return m_commands;
    }

    void setCommandList(const QStringList &cmds);//store in global cache
    void setStateIntervals(int secs);//设置获取服务器网络状态的时间间隔

    int getStateIntervals();
    QString getScoreRecCmdDirectory() const;
    QString getWavDelayRecCmdDirectory() const;
    QString getWavDelayCmdDirectory() const;

    static GlobalSettings *instance() {
        static GlobalSettings s;
        return &s;
    }

    void setRounds(int rounds) {
        m_rounds = rounds;
    }
    int rounds() const {
        return m_rounds;
    }

    void readSettings();
    void saveSettings();

private:
    GlobalSettings(){/*default constructor*/
        readSettings();
        if (m_commands.isEmpty()) {
            //default setting
//             << "tc qdisc show"
//             << "tc qdisc add dev enp3s0 root netem loss 5%"
//             << "tc qdisc change dev enp3s0 root netem loss 10%"
//             << "tc qdisc change dev enp3s0 root netem loss 20%"
//             << "tc qdisc change dev enp3s0 root netem loss 30%"
//             << "tc qdisc del dev enp3s0 root netem loss 10%"
//             << "tc qdisc add dev enp3s0 root netem delay 50ms 50ms"
//             << "tc qdisc change dev enp3s0 root netem delay 100ms 100ms"
//             << "tc qdisc change dev enp3s0 root netem delay 200ms 200ms"
//             << "tc qdisc change dev enp3s0 root netem delay 400ms 400ms"
//             << "tc qdisc del dev enp3s0 root netem delay 400ms 400ms";
            m_commands << "tc qdisc show"
                       << "tc qdisc add dev enp3s0 root netem loss 5%"
                             << "tc qdisc change dev enp3s0 root netem loss 10%"
                             << "tc qdisc change dev enp3s0 root netem loss 20%"
                             << "tc qdisc change dev enp3s0 root netem loss 30%"
                             << "tc qdisc change dev enp3s0 root netem loss 50%"
                             << "tc qdisc change dev enp3s0 root netem loss 70%"
                             << "tc qdisc del dev enp3s0 root netem loss 10%"
                             << "tc qdisc add dev enp3s0 root netem delay 50ms 10ms"
                             << "tc qdisc change dev enp3s0 root netem delay 100ms 20ms"
                             << "tc qdisc change dev enp3s0 root netem delay 200ms 50ms"
                             << "tc qdisc change dev enp3s0 root netem delay 400ms 100ms"
                             << "tc qdisc del dev enp3s0 root netem delay 400ms 100ms";
        }
        m_rounds = 2; //default setting
    }

    ~GlobalSettings() {
        saveSettings();
    }

    int m_state_intervals;
    int m_rounds;
    QStringList m_commands;
};

#endif // SETTINGS

