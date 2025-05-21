#ifndef PERFORMANCESERVICE_H
#define PERFORMANCESERVICE_H

#include "ITipMetricsRepository.h"
#include <QString>
#include <QDate>

struct WaiterMetrics {
    int targetWaiterId = -1;
    int totalTipsCount = 0;
    double totalAmountAttributed = 0.0;
    double averageAmountAttributed = 0.0;
    double sharedTipsPercentage = 0.0;
    bool success = false;
};

class PerformanceService {
public:
    explicit PerformanceService(ITipMetricsRepository* repository);

    WaiterMetrics getWaiterPerformanceMetrics(int requesterId, const QString& role, int targetWaiterId);

private:
    ITipMetricsRepository* m_repository;
};

#endif // PERFORMANCESERVICE_H
