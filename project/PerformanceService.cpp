#include "PerformanceService.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDate>
#include <QVariant>
#include <QtMath>
#include <QDebug>
#include <QStringBuilder>

PerformanceService::PerformanceService() {};

WaiterMetrics PerformanceService::getWaiterPerformanceMetrics(int requesterId, const QString& role, int targetWaiterId) {
    WaiterMetrics metrics;
    metrics.targetWaiterId = targetWaiterId;

    if (role != "manager" && (role != "waiter" || requesterId != targetWaiterId)) {
        qDebug() << "Odmowa dostępu do metryk wydajności: Tylko Manager może przeglądać metryki innych kelnerów. Kelner może zobaczyć tylko własne.";
        return metrics;
    }

    QDate today = QDate::currentDate();
    QDate threeMonthsAgo = today.addMonths(-3);

    QString startDateStr = threeMonthsAgo.toString("yyyy-MM-dd");
    QString endDateStr = today.toString("yyyy-MM-dd");

    qDebug() << "Pobieranie metryk dla kelnera ID:" << targetWaiterId << "z okresu od" << startDateStr << "do" << endDateStr;

    QSqlQuery query;
    if (!query.prepare(R"(
        SELECT
            COUNT(t.id) AS total_tips_count,
            COALESCE(SUM(CASE
                WHEN t.waiter1_id = ? AND t.waiter2_id IS NULL THEN t.amount -- Cała kwota, jeśli targetWaiterId był pierwszy i jedyny
                WHEN t.waiter1_id = ? AND t.waiter2_id IS NOT NULL THEN t.amount / 2.0 -- Połowa, jeśli targetWaiterId był pierwszy i był drugi
                WHEN t.waiter2_id = ? THEN t.amount / 2.0 -- Połowa, jeśli targetWaiterId był drugi
                ELSE 0 -- Wiersze, które nie dotyczą targetWaiterId (powinny być odfiltrowane przez WHERE, ale zabezpieczenie)
            END), 0.0) AS total_amount_attributed,
            COALESCE(AVG(CASE
                WHEN t.waiter1_id = ? AND t.waiter2_id IS NULL THEN t.amount
                WHEN t.waiter1_id = ? AND t.waiter2_id IS NOT NULL THEN t.amount / 2.0
                WHEN t.waiter2_id = ? THEN t.amount / 2.0
                ELSE NULL -- AVG ignoruje NULL-e, co jest pożądane, jeśli nie było pasujących wpisów
            END), 0.0) AS average_amount_attributed,
            -- Obliczenie procentu napiwków dzielonych: (Liczba napiwków DZIELONYCH, w których uczestniczył target) / (Liczba wszystkich napiwków, w których uczestniczył target) * 100
            COALESCE(CAST(SUM(CASE WHEN t.waiter2_id IS NOT NULL AND (t.waiter1_id = ? OR t.waiter2_id = ?) THEN 1 ELSE 0 END) AS REAL) * 100.0 / NULLIF(COUNT(t.id), 0), 0.0) AS shared_tips_percentage
        FROM tips t
        WHERE t.date BETWEEN ? AND ? -- Filtr daty
        AND (t.waiter1_id = ? OR t.waiter2_id = ?) -- Filtr: tylko napiwki, w których uczestniczył targetWaiterId
    )")) {
        qDebug() << "Błąd PRZYGOTOWANIA zapytania GET METRICS:";
        qDebug() << query.lastError().text();
        return metrics;
    }

    int paramIndex = 0;
    query.addBindValue(targetWaiterId); paramIndex++; // SUM CASE 1 (waiter1_id)
    query.addBindValue(targetWaiterId); paramIndex++; // SUM CASE 2 (waiter1_id)
    query.addBindValue(targetWaiterId); paramIndex++; // SUM CASE 3 (waiter2_id)
    query.addBindValue(targetWaiterId); paramIndex++; // AVG CASE 1 (waiter1_id)
    query.addBindValue(targetWaiterId); paramIndex++; // AVG CASE 2 (waiter1_id)
    query.addBindValue(targetWaiterId); paramIndex++; // AVG CASE 3 (waiter2_id)
    query.addBindValue(targetWaiterId); paramIndex++; // PERCENTAGE SUM CASE 1 (waiter1_id)
    query.addBindValue(targetWaiterId); paramIndex++; // PERCENTAGE SUM CASE 2 (waiter2_id)

    query.addBindValue(startDateStr);   paramIndex++; // WHERE date BETWEEN ?
    query.addBindValue(endDateStr);     paramIndex++; // AND ?

    query.addBindValue(targetWaiterId); paramIndex++; // WHERE (waiter1_id = ?
    query.addBindValue(targetWaiterId); paramIndex++; // OR waiter2_id = ?)

    if (!query.exec()) {
        qDebug() << "Błąd WYKONANIA zapytania GET METRICS:";
        qDebug() << query.lastError().text();
        return metrics;
    }

    if (query.next()) {
        if (query.value("total_tips_count").toInt() > 0) {
            metrics.totalTipsCount = query.value("total_tips_count").toInt();
            metrics.totalAmountAttributed = query.value("total_amount_attributed").toDouble();
            metrics.averageAmountAttributed = query.value("average_amount_attributed").toDouble();
            metrics.sharedTipsPercentage = query.value("shared_tips_percentage").toDouble();

            if (qIsNaN(metrics.averageAmountAttributed) || qIsInf(metrics.averageAmountAttributed)) {
                metrics.averageAmountAttributed = 0.0;
            }
            if (qIsNaN(metrics.sharedTipsPercentage) || qIsInf(metrics.sharedTipsPercentage)) {
                metrics.sharedTipsPercentage = 0.0;
            }

            metrics.success = true;

        } else {
            qDebug() << "ℹ️ Nie znaleziono napiwków dla kelnera ID:" << targetWaiterId << "w ciągu ostatnich 3 miesięcy. Metryki zerowe.";
            metrics.success = true;
        }

    } else {
        qDebug() << "Błąd: Zapytanie GET METRICS wykonane, ale next() zwróciło false bez danych.";
    }
    return metrics;
}
