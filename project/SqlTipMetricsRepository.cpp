#include "SqlTipMetricsRepository.h"
#include "PerformanceService.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

WaiterMetrics SqlTipMetricsRepository::fetchWaiterMetrics(int waiterId, const QDate& from, const QDate& to) {
    WaiterMetrics metrics;
    metrics.targetWaiterId = waiterId;

    QString startDateStr = from.toString("yyyy-MM-dd");
    QString endDateStr = to.toString("yyyy-MM-dd");

    QSqlQuery query;
    query.prepare(R"(
        SELECT
            COUNT(t.id) AS total_tips_count,
            COALESCE(SUM(CASE
                WHEN t.waiter1_id = ? AND t.waiter2_id IS NULL THEN t.amount
                WHEN t.waiter1_id = ? AND t.waiter2_id IS NOT NULL THEN t.amount / 2.0
                WHEN t.waiter2_id = ? THEN t.amount / 2.0
                ELSE 0
            END), 0.0) AS total_amount_attributed,
            COALESCE(AVG(CASE
                WHEN t.waiter1_id = ? AND t.waiter2_id IS NULL THEN t.amount
                WHEN t.waiter1_id = ? AND t.waiter2_id IS NOT NULL THEN t.amount / 2.0
                WHEN t.waiter2_id = ? THEN t.amount / 2.0
                ELSE NULL
            END), 0.0) AS average_amount_attributed,
            COALESCE(CAST(SUM(CASE WHEN t.waiter2_id IS NOT NULL AND (t.waiter1_id = ? OR t.waiter2_id = ?) THEN 1 ELSE 0 END) AS REAL) * 100.0 / NULLIF(COUNT(t.id), 0), 0.0) AS shared_tips_percentage
        FROM tips t
        WHERE t.date BETWEEN ? AND ?
        AND (t.waiter1_id = ? OR t.waiter2_id = ?)
    )");

    for (int i = 0; i < 8; ++i) query.addBindValue(waiterId);
    query.addBindValue(startDateStr);
    query.addBindValue(endDateStr);
    query.addBindValue(waiterId);
    query.addBindValue(waiterId);

    if (!query.exec()) {
        qDebug() << "Błąd wykonania zapytania:" << query.lastError().text();
        return metrics;
    }

    if (!query.next()) return metrics;

    metrics.totalTipsCount = query.value("total_tips_count").toInt();
    metrics.totalAmountAttributed = query.value("total_amount_attributed").toDouble();
    metrics.averageAmountAttributed = query.value("average_amount_attributed").toDouble();
    metrics.sharedTipsPercentage = query.value("shared_tips_percentage").toDouble();
    metrics.success = true;

    return metrics;
}
