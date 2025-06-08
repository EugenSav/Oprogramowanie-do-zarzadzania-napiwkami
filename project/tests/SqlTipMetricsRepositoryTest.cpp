#include <QtTest>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDate>
#include "SqlTipMetricsRepository.h"
#include "PerformanceService.h"

class SqlTipMetricsRepositoryTest : public QObject {
    Q_OBJECT

private:
    SqlTipMetricsRepository repo;
    QSqlDatabase db;

    void insertTip(int waiter1, double amount, const QDate& date, std::optional<int> waiter2 = std::nullopt) {
        QSqlQuery q(db);
        QVERIFY(q.prepare("INSERT INTO tips (waiter1_id, waiter2_id, amount, date) VALUES (?, ?, ?, ?)"));
        q.addBindValue(waiter1);
        q.addBindValue(waiter2.has_value() ? QVariant(waiter2.value()) : QVariant(QVariant::Int));
        q.addBindValue(amount);
        q.addBindValue(date.toString("yyyy-MM-dd"));
        QVERIFY(q.exec());
    }

private slots:
    void initTestCase() {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(":memory:");
        QVERIFY(db.open());

        QSqlQuery q(db);
        QVERIFY(q.exec(R"(
            CREATE TABLE tips (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                waiter1_id INTEGER,
                waiter2_id INTEGER,
                amount REAL,
                date TEXT
            )
        )"));
    }

    void cleanup() {
        QSqlQuery(db).exec("DELETE FROM tips");
    }

    void test_SingleTip_FullAmountToWaiter1() {
        QDate date = QDate::currentDate();
        insertTip(1, 100.0, date); // waiter1 only

        WaiterMetrics m = repo.fetchWaiterMetrics(1, date, date);
        QCOMPARE(m.totalTipsCount, 1);
        QCOMPARE(m.totalAmountAttributed, 100.0);
        QCOMPARE(m.averageAmountAttributed, 100.0);
        QCOMPARE(m.sharedTipsPercentage, 0.0);
        QVERIFY(m.success);
    }

    void test_SingleTip_SplitBetweenWaiters() {
        QDate date = QDate::currentDate();
        insertTip(1, 100.0, date, 2); // shared tip

        WaiterMetrics m1 = repo.fetchWaiterMetrics(1, date, date);
        QCOMPARE(m1.totalTipsCount, 1);
        QCOMPARE(m1.totalAmountAttributed, 50.0);
        QCOMPARE(m1.averageAmountAttributed, 50.0);
        QCOMPARE(m1.sharedTipsPercentage, 100.0);
        QVERIFY(m1.success);

        WaiterMetrics m2 = repo.fetchWaiterMetrics(2, date, date);
        QCOMPARE(m2.totalTipsCount, 1);
        QCOMPARE(m2.totalAmountAttributed, 50.0);
        QCOMPARE(m2.sharedTipsPercentage, 100.0);
        QVERIFY(m2.success);
    }

    void test_MixedTips_MultipleCalculations() {
        QDate d1 = QDate(2024, 1, 1);
        QDate d2 = QDate(2024, 1, 2);
        insertTip(1, 60, d1);          // 60 full
        insertTip(1, 40, d2, 2);       // 20 to each
        insertTip(3, 100, d2, 1);      // 50 to waiter1

        WaiterMetrics m = repo.fetchWaiterMetrics(1, d1, d2);
        QCOMPARE(m.totalTipsCount, 3);
        QCOMPARE(m.totalAmountAttributed, 60.0 + 20.0 + 50.0);
        QCOMPARE(m.averageAmountAttributed, (60.0 + 20.0 + 50.0) / 3.0);
        QCOMPARE(m.sharedTipsPercentage, 66.6666666667); // 2/3 shared
        QVERIFY(m.success);
    }

    void test_NoTips_ReturnsZero() {
        QDate date = QDate(2024, 1, 1);
        WaiterMetrics m = repo.fetchWaiterMetrics(99, date, date);
        QCOMPARE(m.totalTipsCount, 0);
        QCOMPARE(m.totalAmountAttributed, 0.0);
        QCOMPARE(m.averageAmountAttributed, 0.0);
        QCOMPARE(m.sharedTipsPercentage, 0.0);
        QVERIFY(m.success);
    }

    void test_TipAssignedOnlyAsWaiter2() {
        QDate d = QDate::currentDate();
        insertTip(5, 100.0, d, 1); // waiter1 = 5, waiter2 = 1

        WaiterMetrics m = repo.fetchWaiterMetrics(1, d, d);
        QCOMPARE(m.totalTipsCount, 1);
        QCOMPARE(m.totalAmountAttributed, 50.0);
        QCOMPARE(m.sharedTipsPercentage, 100.0);
        QVERIFY(m.success);
    }

    void test_TipsOutsideDateRange_Ignored() {
        QDate dPast = QDate(2020, 1, 1);
        QDate dRange = QDate(2024, 1, 1);
        insertTip(1, 99.0, dPast); // should be ignored
        insertTip(1, 50.0, dRange); // should count

        WaiterMetrics m = repo.fetchWaiterMetrics(1, dRange, dRange);
        QCOMPARE(m.totalTipsCount, 1);
        QCOMPARE(m.totalAmountAttributed, 50.0);
    }

    void test_NullSecondWaiterCountedCorrectly() {
        QDate d = QDate::currentDate();
        insertTip(1, 80.0, d); // waiter2 = null

        WaiterMetrics m = repo.fetchWaiterMetrics(1, d, d);
        QCOMPARE(m.totalTipsCount, 1);
        QCOMPARE(m.totalAmountAttributed, 80.0);
        QCOMPARE(m.sharedTipsPercentage, 0.0);
    }

    void test_NonParticipantWaiter_ZeroMetrics() {
        QDate d = QDate::currentDate();
        insertTip(1, 100.0, d, 2);

        WaiterMetrics m = repo.fetchWaiterMetrics(3, d, d);
        QCOMPARE(m.totalTipsCount, 0);
        QCOMPARE(m.totalAmountAttributed, 0.0);
        QVERIFY(m.success);
    }

    void test_WaiterAsBothWaiters() {
        QDate d = QDate::currentDate();
        insertTip(1, 100.0, d, 1); // waiter1_id == waiter2_id == 1

        WaiterMetrics m = repo.fetchWaiterMetrics(1, d, d);
        // зависит от логики: тут он получит 50.0
        QCOMPARE(m.totalTipsCount, 1);
        QCOMPARE(m.totalAmountAttributed, 50.0); // может быть 100.0 по логике
    }

    void test_MassiveTipsCount() {
        QDate d = QDate::currentDate();
        for (int i = 0; i < 1000; ++i)
            insertTip(1, 1.0, d);

        WaiterMetrics m = repo.fetchWaiterMetrics(1, d, d);
        QCOMPARE(m.totalTipsCount, 1000);
        QCOMPARE(m.totalAmountAttributed, 1000.0);
    }

    void test_AverageAmountCalculationPrecision() {
        QDate d = QDate::currentDate();
        insertTip(1, 10, d);
        insertTip(1, 30, d);
        insertTip(1, 20, d);

        WaiterMetrics m = repo.fetchWaiterMetrics(1, d, d);
        QCOMPARE(m.totalTipsCount, 3);
        QVERIFY(std::abs(m.averageAmountAttributed - 20.0) < 0.0001);
    }

    void test_BrokenTable_ReturnsUnsuccessful() {
        QSqlQuery().exec("DROP TABLE tips");

        QDate d = QDate::currentDate();
        WaiterMetrics m = repo.fetchWaiterMetrics(1, d, d);
        QVERIFY(!m.success);
    }
};

QTEST_MAIN(SqlTipMetricsRepositoryTest)
#include "SqlTipMetricsRepositoryTest.moc"
