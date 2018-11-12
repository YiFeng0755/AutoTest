#include "database.h"
#include <QDebug>

//SqlManager SqlManager::inst;

SqlManager::SqlManager()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
}

bool SqlManager::initDb()
{
    if (!QSqlDatabase::drivers().contains("QSQLITE")) {
        qDebug() << "available drivers are :" << QSqlDatabase::drivers();
        return false;
    }
    db.setDatabaseName("youmedb.db");
    db.setUserName("test");
    db.setPassword("test");
    if (!db.open()) {
        qDebug() << "Db open failed";
        return false;
    }

    QSqlQuery q;
    if (db.tables().isEmpty()) {
        if (!q.exec(QLatin1String("create table actions(id integer primary key, name varchar, url varchar, args varchar)"))) {
            qDebug() << q.lastError();
            return false;
        }

        if (!q.exec(QLatin1String("create table apps(id integer primary key, appid integer, appkey varchar, appsecret date)"))) {
            qDebug() << q.lastError();
            return false;
        }
    }

    return true;
}

QSqlDatabase SqlManager::getSb()
{
    if (!initDb())
        return QSqlDatabase();
    return db;
}
