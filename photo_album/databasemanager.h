#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QString>
#include <QSqlDatabase>

class DatabaseManager
{
public:
    DatabaseManager();
    ~DatabaseManager();

    bool connectToDatabase(const QString& dbPath);
    void saveFileInfo(const QStringList& fileList);
    QStringList loadFileInfo();

private:
    QSqlDatabase database;


};

#endif // DATABASEMANAGER_H
