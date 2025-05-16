#include "DatabaseService.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>

bool DatabaseService::connect(const QString& dbPath) {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbPath);

    if (!db.open()) {
        qDebug() << "Błąd połączenia z bazą:" << db.lastError().text();
        return false;
    }

    qDebug() << "Połączono z bazą danych:" << dbPath;
    return true;
}
