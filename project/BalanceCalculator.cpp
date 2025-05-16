#include "BalanceCalculator.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDate>
#include <QVariant>
#include <QDebug>
//klasa spelnia SRP - zajmuje sie tylko jedna rzecza, spelnia OCP - dodanie np rocznego balansu nie wymaga modyfikacji istniejacego kodu
BalanceCalculator::BalanceCalculator() {};

double BalanceCalculator::getBalanceInRange(int requesterId, const QString& role, int targetWaiterId, const QDate& from, const QDate& to) {
    if (role == "waiter" && requesterId != targetWaiterId) {
        qDebug() << "Brak dostępu.";
        return -1.0;
    }

    QSqlQuery query;
    query.prepare(R"(
        SELECT waiter1_id, waiter2_id, amount
        FROM tips
        WHERE date BETWEEN ? AND ?
        AND (waiter1_id = ? OR waiter2_id = ?)
    )");

    query.addBindValue(from.toString("yyyy-MM-dd"));
    query.addBindValue(to.toString("yyyy-MM-dd"));
    query.addBindValue(targetWaiterId);
    query.addBindValue(targetWaiterId);

    if (!query.exec()) {
        qDebug() << "Błąd zapytania:" << query.lastError().text();
        return -1.0;
    }

    double total = 0.0;
    while (query.next()) {
        int id2 = query.value(1).isNull() ? -1 : query.value(1).toInt();
        double amount = query.value(2).toDouble();
        total += (id2 == -1) ? amount : amount / 2.0;
    }

    return total;
}

double BalanceCalculator::getTodayBalance(int requesterId, const QString& role, int targetWaiterId) {
    QDate today = QDate::currentDate();
    return getBalanceInRange(requesterId, role, targetWaiterId, today, today);
}

double BalanceCalculator::getMonthlyBalance(int requesterId, const QString& role, int targetWaiterId) {
    QDate current = QDate::currentDate();
    QDate startOfMonth = current.addDays(1 - current.day());
    QDate endOfMonth = startOfMonth.addMonths(1).addDays(-1);
    return getBalanceInRange(requesterId, role, targetWaiterId, startOfMonth, endOfMonth);
}

