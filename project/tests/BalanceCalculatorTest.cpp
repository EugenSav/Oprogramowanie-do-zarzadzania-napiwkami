#include <QtTest>
#include "BalanceCalculator.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

class BalanceCalculatorTest : public QObject {
    Q_OBJECT

private:
    BalanceCalculator calculator;
    QSqlDatabase db;
    int waiter1Id = 1;
    int waiter2Id = 2;
    int managerId = 99;

private slots:
    void initTestCase() {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(":memory:");
        QVERIFY(db.open());

        QSqlQuery query;
        QVERIFY(query.exec("CREATE TABLE tips (id INTEGER PRIMARY KEY, waiter1_id INTEGER, waiter2_id INTEGER, amount REAL, date TEXT)"));

        struct TipEntry {
            int w1, w2;
            double amount;
            QString date;
        };

        QList<TipEntry> tips = {
            {1, QVariant().toInt(),  20.0, "2025-06-01"},
            {1, 2,                  30.0, "2025-06-02"},
            {2, QVariant().toInt(), 15.0, "2025-06-03"},
            {2, QVariant().toInt(), 10.0, "2025-05-28"},
            {1, 2,                  25.0, "2025-05-30"},
            {1, QVariant().toInt(), 5.0,  "2025-06-06"},
        };

        for (const auto& t : tips) {
            query.prepare("INSERT INTO tips (waiter1_id, waiter2_id, amount, date) VALUES (?, ?, ?, ?)");
            query.addBindValue(t.w1);
            if (t.w2 == 0) query.addBindValue(QVariant(QVariant::Int));
            else query.addBindValue(t.w2);
            query.addBindValue(t.amount);
            query.addBindValue(t.date);
            QVERIFY2(query.exec(), query.lastError().text().toUtf8().constData());
        }
    }

    void cleanupTestCase() {
        db.close();
    }

    void testGetBalanceInRange_data() {
        QTest::addColumn<int>("requesterId");
        QTest::addColumn<QString>("role");
        QTest::addColumn<int>("targetWaiterId");
        QTest::addColumn<QDate>("from");
        QTest::addColumn<QDate>("to");
        QTest::addColumn<double>("expectedBalance");

        // Waiter requesting himself
        QTest::newRow("waiter1_full_access") << 1 << "waiter" << 1 << QDate(2025, 6, 1) << QDate(2025, 6, 6) << (20.0 + 30.0 / 2 + 5.0);
        QTest::newRow("waiter2_full_access") << 2 << "waiter" << 2 << QDate(2025, 6, 1) << QDate(2025, 6, 6) << (30.0 / 2 + 15.0);

        // Waiter requesting someone else
        QTest::newRow("waiter1_blocked_from_waiter2") << 1 << "waiter" << 2 << QDate(2025, 6, 1) << QDate(2025, 6, 6) << -1.0;

        // Manager requesting any data
        QTest::newRow("manager_for_waiter1") << 99 << "manager" << 1 << QDate(2025, 6, 1) << QDate(2025, 6, 6) << (20.0 + 30.0 / 2 + 5.0);
        QTest::newRow("manager_for_waiter2") << 99 << "manager" << 2 << QDate(2025, 5, 1) << QDate(2025, 5, 31) << (25.0 / 2 + 10.0);

        // No tips in range
        QTest::newRow("empty_date_range") << 99 << "manager" << 1 << QDate(2024, 1, 1) << QDate(2024, 1, 10) << 0.0;
    }

    void testGetBalanceInRange() {
        QFETCH(int, requesterId);
        QFETCH(QString, role);
        QFETCH(int, targetWaiterId);
        QFETCH(QDate, from);
        QFETCH(QDate, to);
        QFETCH(double, expectedBalance);

        double balance = calculator.getBalanceInRange(requesterId, role, targetWaiterId, from, to);
        QCOMPARE(balance, expectedBalance);
    }

    void testGetTodayBalance_accessControl() {
        QDate today = QDate::currentDate();

        // Forcing an insert with today's date
        QSqlQuery query;
        query.prepare("INSERT INTO tips (waiter1_id, waiter2_id, amount, date) VALUES (?, ?, ?, ?)");
        query.addBindValue(1);
        query.addBindValue(QVariant(QVariant::Int)); // No second waiter
        query.addBindValue(99.99);
        query.addBindValue(today.toString("yyyy-MM-dd"));
        QVERIFY(query.exec());

        // Waiter accessing their own balance today
        double balance = calculator.getTodayBalance(1, "waiter", 1);
        QVERIFY(balance >= 99.99);

        // Waiter accessing another waiter's balance today
        double blocked = calculator.getTodayBalance(2, "waiter", 1);
        QCOMPARE(blocked, -1.0);

        // Manager can access
        double managerView = calculator.getTodayBalance(99, "manager", 1);
        QVERIFY(managerView >= 99.99);
    }

    void testGetMonthlyBalance() {
        QDate current = QDate::currentDate();
        int year = current.year();
        int month = current.month();

        // Add a tip entry just for this month and test monthly sum
        QSqlQuery query;
        query.prepare("INSERT INTO tips (waiter1_id, waiter2_id, amount, date) VALUES (?, ?, ?, ?)");
        query.addBindValue(2);
        query.addBindValue(QVariant(QVariant::Int));
        query.addBindValue(88.88);
        query.addBindValue(current.toString("yyyy-MM-dd"));
        QVERIFY(query.exec());

        double balance = calculator.getMonthlyBalance(2, "waiter", 2);
        QVERIFY(balance >= 88.88);

        double blocked = calculator.getMonthlyBalance(1, "waiter", 2);
        QCOMPARE(blocked, -1.0);
    }
};

QTEST_MAIN(BalanceCalculatorTest)
#include "BalanceCalculatorTest.moc"
