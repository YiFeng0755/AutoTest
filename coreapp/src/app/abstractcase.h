#ifndef ABSTRACTCASE_H
#define ABSTRACTCASE_H
#include <QObject>

class AbastractCase : public QObject
{
    Q_OBJECT
public:
    explicit AbastractCase(QObject * parent = 0);
    /*case执行接口*/
    virtual void execute() {
        setup();
        run();
        teardown();
    }

    virtual bool checkResult() { return false; }
protected:
    /**
     * @brief setup case执行前的数据准备
     */
    virtual void setup() {}

    /**
     * @brief setup case执行后的数据清理接口
     */
    virtual void teardown() {}

    /**
     * @brief case核心部分的逻辑，重载此接口实现端到端的测试逻辑
     */
    virtual void run() {}

    /*数据检查逻辑*/

private:
    // input data
    // ouput data
};

#endif // ABSTRACTCASE

