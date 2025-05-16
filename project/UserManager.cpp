#include "UserManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QCryptographicHash>
#include <QDebug>

bool UserManager::login(const QString& username, const QString& password) {
    QSqlQuery query;

    if (!query.prepare("SELECT password FROM users WHERE username = ?")) {
        qDebug() << "Błąd PRZYGOTOWANIA zapytania:";
        qDebug() << query.lastError().text();
        return false;
    }

    query.addBindValue(username);

    if (!query.exec()) {
        qDebug() << "Błąd WYKONANIA zapytania:";
        qDebug() << query.lastError().text();
        return false;
    }

    if (!query.next()) {
        qDebug() << "Nie znaleziono użytkownika:" << username;
        return false;
    }

    QString storedHash = query.value(0).toString();

    return storedHash == hashPassword(password);
}

QString UserManager::getRole(const QString& username) {
    QSqlQuery query;
    query.prepare("SELECT role FROM users WHERE username = ?");
    query.addBindValue(username);

    if (!query.exec() || !query.next()) {
        qDebug() << "Nie znaleziono roli:" << query.lastError().text();
        return "";
    }

    return query.value(0).toString();
}

QString UserManager::hashPassword(const QString& password) {
    QByteArray hash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);
    return hash.toHex();
}

int UserManager::getUserId(const QString& username) {
    QSqlQuery query;
    query.prepare("SELECT id FROM users WHERE username = ?");
    query.addBindValue(username);

    if (!query.exec() || !query.next()) {
        qDebug() << "Nie znaleziono ID użytkownika:" << query.lastError().text();
        return -1;
    }

    return query.value(0).toInt();
}
