#include "UserRepository.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant> // Do QVariant(), toValue()
#include <QDebug>

bool UserRepository::insertUser(const QString& username, const QString& hashedPassword, const QString& role) {
    qDebug() << "Próba dodania użytkownika:" << username << role;

    if (username.isEmpty() || hashedPassword.isEmpty() || role.isEmpty()) {
        qDebug() << "Niepoprawne dane wejściowe.";
        return false;
    }

    if (role != "waiter" && role != "manager" && role != "admin") {
        qDebug() << "Niepoprawna rola.";
        return false;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO users (username, password, role) VALUES (?, ?, ?)");
    query.addBindValue(username);
    query.addBindValue(hashedPassword);
    query.addBindValue(role);

    bool ok = query.exec();
    if (!ok) {
        qDebug() << "Błąd INSERT:" << query.lastError().text();
        return false;
    }
    qDebug() << "Dodano użytkownika, zmodyfikowanych wierszy:" << query.numRowsAffected();
    return true;
}

bool UserRepository::deleteUser(int id) {
    if (id <= 0) {
        qDebug() << "Niepoprawne ID użytkownika podane do deleteUser:" << id;
        return false;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM users WHERE id = ?");
    query.addBindValue(id);

    if (!query.exec()) {
        qDebug() << "Błąd Bazy Danych (deleteUser):" << query.lastError().text();
        return false;
    }
    return query.numRowsAffected() == 1;
}
