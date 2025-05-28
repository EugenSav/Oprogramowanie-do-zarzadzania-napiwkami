#include "TipManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDate>
#include <QDebug>
#include <QString>

bool TipManager::addTip(double amount, int waiter_id, std::optional<int> waiter2_id) {
    QSqlQuery query;

    if (!query.prepare("INSERT INTO tips (waiter1_id, amount, date, waiter2_id) VALUES (?, ?, ?, ?)")) {
        qDebug() << "Błąd PRZYGOTOWANIA zapytania INSERT:";
        qDebug() << query.lastError().text();
        return false;
    }

    query.addBindValue(waiter_id);
    query.addBindValue(amount);
    query.addBindValue(QDate::currentDate().toString("yyyy-MM-dd"));

    if (waiter2_id.has_value()) {
        query.addBindValue(waiter2_id.value());
    } else {
        query.addBindValue(QVariant());
    }

    if (!query.exec()) {
        qDebug() << "Błąd WYKONANIA zapytania INSERT:";
        qDebug() << query.lastError().text();
        return false;
    }

    qDebug() << "Napiwek dodany: " << amount << " dla kelnera " << waiter_id;
    if (waiter2_id.has_value()) {
        qDebug() << " oraz kelnera " << waiter2_id.value();
    }

    return true;
}

bool TipManager::correctLastTipAmount(int requesterId, const QString& role, double newAmount) {

    QSqlQuery findQuery;
    if (!findQuery.prepare(R"(
        SELECT id, waiter1_id
        FROM tips
        ORDER BY id DESC
        LIMIT 1
    )")) {
        qDebug() << "Błąd PRZYGOTOWANIA zapytania FIND GLOBAL LAST TIP:";
        qDebug() << findQuery.lastError().text();
        return false;
    }
    if (!findQuery.exec()) {
        qDebug() << "Błąd WYKONANIA zapytania FIND GLOBAL LAST TIP:";
        qDebug() << findQuery.lastError().text();
        return false;
    }

    if (!findQuery.next()) {
        qDebug() << "Nie znaleziono żadnego napiwku w bazie danych do skorygowania.";
        return false;
    }

    int lastTipId = findQuery.value(0).toInt();         // ID ostatniego napiwku znalezionego w bazie
    int originalWaiter1Id = findQuery.value(1).toInt(); // waiter1_id TEGO ostatniego napiwku


    bool accessGranted = false;

    if (role == "manager") {
        accessGranted = true;
        qDebug() << "Dostęp przyznany: Manager może edytować ostatni globalnie napiwek (ID:" << lastTipId << ").";
    } else if (role == "waiter") {
        // Kelner ma dostęp tylko, jeśli był PIERWSZYM kelnerem TEGO konkretnego ostatniego (globalnie) napiwku
        if (requesterId == originalWaiter1Id) {
            accessGranted = true;
            qDebug() << "Dostęp przyznany: Kelner (ID:" << requesterId << ") może edytować swój ostatni napiwek (ID:" << lastTipId << ").";
        } else {
            qDebug() << "Odmowa dostępu: Kelner (ID:" << requesterId << ") nie może edytować ostatniego globalnie napiwku (ID:" << lastTipId << "), bo go nie dodał jako główny kelner (dodał ID:" << originalWaiter1Id << ").";
            accessGranted = false; // Jawna odmowa
        }
    } else {
        qDebug() << "Odmowa dostępu: Rola '" << role << "' nie ma uprawnień do edycji napiwków.";
        accessGranted = false;
    }

    if (accessGranted) {
        QSqlQuery updateQuery;
        if (!updateQuery.prepare(R"(
            UPDATE tips
            SET amount = ?
            WHERE id = ?
        )")) {
            qDebug() << "Błąd PRZYGOTOWANIA zapytania UPDATE TIP AMOUNT:";
            qDebug() << updateQuery.lastError().text();
            return false;
        }
        updateQuery.addBindValue(newAmount);
        updateQuery.addBindValue(lastTipId);

        if (!updateQuery.exec()) {
            qDebug() << "Błąd WYKONANIA zapytania UPDATE TIP AMOUNT:";
            qDebug() << updateQuery.lastError().text();
            return false;
        }

        if (updateQuery.numRowsAffected() == 1) {
            qDebug() << "Pomyślnie skorygowano kwotę ostatniego napiwku (ID:" << lastTipId << ") na:" << newAmount;
            return true;
        } else {
            qDebug() << "Zapytanie UPDATE wykonane, ale nie zmodyfikowano dokładnie 1 wiersza (zmodyfikowano:" << updateQuery.numRowsAffected() << ").";
            return false;
        }
    } else {
        return false;
    }
}
