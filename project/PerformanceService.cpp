#include "PerformanceService.h"
#include <QDebug>

PerformanceService::PerformanceService(ITipMetricsRepository* repository)
    : m_repository(repository) {}

WaiterMetrics PerformanceService::getWaiterPerformanceMetrics(int requesterId, const QString& role, int targetWaiterId) {
    WaiterMetrics metrics;
    metrics.targetWaiterId = targetWaiterId;

    if (role != "manager" && (role != "waiter" || requesterId != targetWaiterId)) {
        qDebug() << "Odmowa dostępu do metryk wydajności";
        return metrics;
    }

    QDate today = QDate::currentDate();
    QDate threeMonthsAgo = today.addMonths(-3);

    return m_repository->fetchWaiterMetrics(targetWaiterId, threeMonthsAgo, today);
}
