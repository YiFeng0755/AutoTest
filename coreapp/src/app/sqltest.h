#ifndef SQLTEST_H
#define SQLTEST_H

#include <QSqlDriver>
#include <QSqlDatabase>

class MySqlTest : public QObject
{
    Q_OBJECT
public:
    MySqlTest();
    ~MySqlTest() {}

    bool get_user(QString username);

private:
    QSqlDatabase * db;
};

#endif // SQLTEST_H
