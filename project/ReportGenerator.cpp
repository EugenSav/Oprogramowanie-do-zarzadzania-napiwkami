#include "ReportGenerator.h"
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>
#include <QFile>
#include <QTextStream>
#include <QDate>
#include <QDebug>

ReportGenerator::ReportGenerator(const QString& databasePath)
    : m_databasePath(databasePath){}

bool ReportGenerator::generateMonthlyReports(const QString& outputDir, int year, int month)
{
    QDate startDate(year, month, 1);
    QDate endDate = startDate.addMonths(1).addDays(-1);

    QString periodStart = startDate.toString("yyyy-MM-dd");
    QString periodEnd = endDate.toString("yyyy-MM-dd");

    QSqlQuery userQuery;
    if (!userQuery.exec("SELECT id, username FROM users WHERE role IN ('waiter', 'manager')")) {
        qDebug() << "Błąd zapytania użytkowników:" << userQuery.lastError().text();
        return false;
    }

    bool success = true;
    while (userQuery.next()) {
        int userId = userQuery.value(0).toInt();
        QString username = userQuery.value(1).toString();

        if (!generateReportForUser(userId, username, outputDir, periodStart, periodEnd)) {
            success = false;
        }
    }
    return success;
}

bool ReportGenerator::generateReportForUser(int userId, const QString& username, const QString& outputDir, const QString& periodStart, const QString& periodEnd)
{
    QSqlQuery query;
    query.prepare(R"(
        SELECT date, amount, waiter1_id, waiter2_id
        FROM tips
        WHERE date BETWEEN ? AND ?
        AND (waiter1_id = ? OR waiter2_id = ?)
        ORDER BY date ASC
    )");
    query.addBindValue(periodStart);
    query.addBindValue(periodEnd);
    query.addBindValue(userId);
    query.addBindValue(userId);

    if (!query.exec()) {
        qDebug() << "Błąd pobierania napiwków dla usera:" << username << query.lastError().text();
        return false;
    }

    QString filename = outputDir + "/raport_" + username + "_" + periodStart.left(7) + ".csv";
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Nie można zapisać pliku:" << filename;
        return false;
    }

    QTextStream out(&file);
    out << "Data,Napiwek,Dzielony,Udział\n";

    while (query.next()) {
        QString date = query.value(0).toString();
        double amount = query.value(1).toDouble();
        int w1 = query.value(2).toInt();
        QVariant w2Var = query.value(3);
        bool isShared = !w2Var.isNull();
        bool isMain = (userId == w1);
        double share = isShared ? amount / 2.0 : amount;

        out << date << "," << QString::number(amount, 'f', 2) << ",";
        out << (isShared ? "Tak" : "Nie") << ",";
        out << QString::number(share, 'f', 2) << "\n";
    }

    file.close();
    return true;
}
