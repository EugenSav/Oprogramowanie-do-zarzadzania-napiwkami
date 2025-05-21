#ifndef SQLTIPMETRICSREPOSITORY_H
#define SQLTIPMETRICSREPOSITORY_H

#include "ITipMetricsRepository.h"

class SqlTipMetricsRepository : public ITipMetricsRepository {
public:
    WaiterMetrics fetchWaiterMetrics(int waiterId, const QDate& from, const QDate& to) override;
};

#endif // SQLTIPMETRICSREPOSITORY_H
