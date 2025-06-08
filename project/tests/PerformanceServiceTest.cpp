#include <QtTest>
#include "PerformanceService.h"

// Fake implementation of ITipMetricsRepository
class FakeTipMetricsRepository : public ITipMetricsRepository {
public:
    int lastCalledWaiterId = -1;
    QDate lastFrom;
    QDate lastTo;

    WaiterMetrics metricsToReturn;

    WaiterMetrics fetchWaiterMetrics(int waiterId, const QDate& from, const QDate& to) override {
        lastCalledWaiterId = waiterId;
        lastFrom = from;
        lastTo = to;
        return metricsToReturn;
    }
};

class PerformanceServiceTest : public QObject {
    Q_OBJECT

private:
    FakeTipMetricsRepository* fakeRepo;
    PerformanceService* service;

private slots:
    void init() {
        fakeRepo = new FakeTipMetricsRepository();
        service = new PerformanceService(fakeRepo);

        // setup expected return data
        fakeRepo->metricsToReturn = WaiterMetrics{ 42, 10, 200.0, 20.0, 50.0, true };
    }

    void cleanup() {
        delete service;
        delete fakeRepo;
    }

    void testManagerCanAccessAnyWaiter() {
        WaiterMetrics result = service->getWaiterPerformanceMetrics(99, "manager", 42);

        QCOMPARE(result.success, true);
        QCOMPARE(result.targetWaiterId, 42);
        QCOMPARE(result.totalTipsCount, 10);
        QCOMPARE(result.totalAmountAttributed, 200.0);
        QCOMPARE(result.averageAmountAttributed, 20.0);
        QCOMPARE(result.sharedTipsPercentage, 50.0);
        QCOMPARE(fakeRepo->lastCalledWaiterId, 42);
    }

    void testWaiterCanAccessSelf() {
        WaiterMetrics result = service->getWaiterPerformanceMetrics(7, "waiter", 7);

        QCOMPARE(result.success, true);
        QCOMPARE(result.targetWaiterId, 42);
    }

    void testWaiterCannotAccessOthers() {
        WaiterMetrics result = service->getWaiterPerformanceMetrics(1, "waiter", 2);

        QCOMPARE(result.success, false);
        QCOMPARE(result.totalTipsCount, 0);
        QCOMPARE(result.totalAmountAttributed, 0.0);
        QCOMPARE(result.averageAmountAttributed, 0.0);
        QCOMPARE(result.sharedTipsPercentage, 0.0);
        QCOMPARE(result.targetWaiterId, 2);
        QCOMPARE(fakeRepo->lastCalledWaiterId, -1);  // should not call the repo
    }

    void testUnauthorizedRoleDenied() {
        WaiterMetrics result = service->getWaiterPerformanceMetrics(1, "customer", 1);

        QCOMPARE(result.success, false);
        QCOMPARE(result.targetWaiterId, 1);
        QCOMPARE(fakeRepo->lastCalledWaiterId, -1);  // should not call the repo
    }

    void testRepositoryCalledWithCorrectDates() {
        QDate today = QDate::currentDate();
        QDate threeMonthsAgo = today.addMonths(-3);

        service->getWaiterPerformanceMetrics(99, "manager", 5);

        QCOMPARE(fakeRepo->lastFrom, threeMonthsAgo);
        QCOMPARE(fakeRepo->lastTo, today);
    }
};

QTEST_MAIN(PerformanceServiceTest)
#include "PerformanceServiceTest.moc"
