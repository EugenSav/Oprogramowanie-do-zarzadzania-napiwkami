#ifndef ITIPMETRICSREPOSITORY_H
#define ITIPMETRICSREPOSITORY_H

#include <QString>
#include <QDate>

struct WaiterMetrics;

class ITipMetricsRepository {
public:
    virtual ~ITipMetricsRepository() = default;

    virtual WaiterMetrics fetchWaiterMetrics(int waiterId, const QDate& from, const QDate& to) = 0;
};

#endif // ITIPMETRICSREPOSITORY_H
