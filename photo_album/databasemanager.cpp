#include "databasemanager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

DatabaseManager::DatabaseManager() {}

DatabaseManager::~DatabaseManager()
{
    if (database.isOpen()) {
        database.close();
    }
}

bool DatabaseManager::connectToDatabase(const QString &dbPath)
{
    database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(dbPath);


    if (!database.open()) {
        qDebug() << "Failed to open database:" << database.lastError().text();
        return false;
    }
    // 创建表（如果不存在）
    QSqlQuery query(database);
    query.exec("CREATE TABLE IF NOT EXISTS pic_file_info (id INTEGER PRIMARY KEY, file_path TEXT)");

    return true;
}

void DatabaseManager::saveFileInfo(const QStringList &fileList)
{
    QSqlQuery query;
    query.exec("DELETE FROM pic_file_info"); // 清空表

    query.prepare("INSERT INTO pic_file_info (file_path) VALUES (?)");
    for (const QString& file : fileList) {
        query.addBindValue(file);
        query.exec();
    }
}

QStringList DatabaseManager::loadFileInfo()
{
    QStringList fileList;
    QSqlQuery query("SELECT file_path FROM pic_file_info");

    while (query.next()) {
        fileList.append(query.value(0).toString());
    }

    return fileList;
}
