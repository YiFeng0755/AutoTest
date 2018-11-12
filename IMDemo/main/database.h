#ifndef DATABASE
#define DATABASE

#include <QSqlDatabase>
#include <QSqlResult>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlError>

class SqlManager
{
public:
    static SqlManager* createInstance() {
        static SqlManager inst;
        return &inst;
    }
    QSqlDatabase getSb();
    bool initDb();
protected:


private:
    SqlManager();
    QSqlDatabase db;
};

#endif // DATABASE

