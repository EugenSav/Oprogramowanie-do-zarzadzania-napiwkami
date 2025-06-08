#include <QtTest>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDate>
#include "TipManager.h"

class TipManagerTest : public QObject {
    Q_OBJECT

private:
    TipManager manager;

    QSqlQuery selectLastTip() {
        QSqlQuery q;
        q.prepare("SELECT waiter1_id, amount, date, waiter2_id FROM tips ORDER BY id DESC LIMIT 1");
        q.exec();
        q.next();
        return q;
    }

    void insertTip(int waiter1Id, double amount) {
        QSqlQuery q;
        QVERIFY(q.prepare("INSERT INTO tips (waiter1_id, amount, date) VALUES (?, ?, ?)"));
        q.addBindValue(waiter1Id);
        q.addBindValue(amount);
        q.addBindValue(QDate::currentDate().toString("yyyy-MM-dd"));
        QVERIFY(q.exec());
    }

    double getLastTipAmount() {
        QSqlQuery q("SELECT amount FROM tips ORDER BY id DESC LIMIT 1");

        if (!q.exec()) {
            throw std::runtime_error(("Request execution error: " + q.lastError().text()).toStdString());
        }
        if (!q.next()) {
            throw std::runtime_error("There is no tip in the table.");
        }
        return q.value(0).toDouble();
    }

private slots:
    void initTestCase() {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(":memory:");
        QVERIFY(db.open());

        QSqlQuery query;
        QVERIFY(query.exec(R"(
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
        QSqlQuery().exec("DROP TABLE IF EXISTS tips");

        QSqlQuery q;
        q.exec(R"(
        CREATE TABLE tips (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            waiter1_id INTEGER,
            waiter2_id INTEGER,
            amount REAL,
            date TEXT
        )
    )");
    }

    // --- addTip tests ---

    void testAddTip_waiterOnly() {
        QVERIFY(manager.addTip(42.5, 1));
        QSqlQuery q = selectLastTip();
        QCOMPARE(q.value(0).toInt(), 1);
        QCOMPARE(q.value(1).toDouble(), 42.5);
        QVERIFY(q.value(3).isNull());
    }

    void testAddTip_withSecondWaiter() {
        QVERIFY(manager.addTip(100.0, 2, 3));
        QSqlQuery q = selectLastTip();
        QCOMPARE(q.value(0).toInt(), 2);
        QCOMPARE(q.value(1).toDouble(), 100.0);
        QCOMPARE(q.value(3).toInt(), 3);
    }

    void testAddTip_zeroAmount() {
        QVERIFY(manager.addTip(0.0, 4));
        QSqlQuery q = selectLastTip();
        QCOMPARE(q.value(1).toDouble(), 0.0);
    }

    void testAddTip_negativeAmount() {
        QVERIFY(manager.addTip(-20.0, 5));
    }

    void testAddTip_largeAmount() {
        double big = 999999999.99;
        QVERIFY(manager.addTip(big, 6));
        QSqlQuery q = selectLastTip();
        QCOMPARE(q.value(1).toDouble(), big);
    }

    void testAddTip_checkDate() {
        manager.addTip(77.0, 7);
        QSqlQuery q = selectLastTip();
        QString today = QDate::currentDate().toString("yyyy-MM-dd");
        QCOMPARE(q.value(2).toString(), today);
    }

    void testAddTip_nullSecondWaiterExplicit() {
        QVERIFY(manager.addTip(25.0, 8, std::nullopt));
        QSqlQuery q = selectLastTip();
        QCOMPARE(q.value(0).toInt(), 8);
        QVERIFY(q.value(3).isNull());
    }

    void testAddTip_multipleTimes() {
        QVERIFY(manager.addTip(10.0, 9));
        QVERIFY(manager.addTip(11.0, 9));
        QVERIFY(manager.addTip(12.0, 9));

        QSqlQuery q;
        QVERIFY(q.exec("SELECT COUNT(*) FROM tips WHERE waiter1_id = 9"));
        QVERIFY(q.next());
        QCOMPARE(q.value(0).toInt(), 3);
    }

    void testAddTip_extremelyLargeAmount() {
        double amount = 1e15;
        QVERIFY(manager.addTip(amount, 10));
        QSqlQuery q = selectLastTip();
        QCOMPARE(q.value(1).toDouble(), amount);
    }

    void testAddTip_floatingPointPrecision() {
        QVERIFY(manager.addTip(12.3456789, 11));
        QSqlQuery q = selectLastTip();
        QCOMPARE(q.value(1).toDouble(), 12.3456789);
    }

    void testAddTip_invalidWaiterId() {
        QVERIFY(manager.addTip(50.0, -1));
        QSqlQuery q = selectLastTip();
        QCOMPARE(q.value(0).toInt(), -1);
    }

    void testAddTip_withSameWaiters() {
        QVERIFY(manager.addTip(70.0, 15, 15));
        QSqlQuery q = selectLastTip();
        QCOMPARE(q.value(0).toInt(), 15);
        QCOMPARE(q.value(3).toInt(), 15);
    }

    void testAddTip_failsOnMissingTable() {
        QVERIFY(QSqlQuery().exec("DROP TABLE tips"));
        QVERIFY(!manager.addTip(20.0, 99));
    }

    // --- correctLastTipAmount tests ---

    void test_NoTips_ReturnsFalse() {
        QVERIFY(!manager.correctLastTipAmount(1, "manager", 123.45));
    }

    void test_ManagerCanCorrectLastTip() {
        insertTip(10, 50.0);
        QVERIFY(manager.correctLastTipAmount(99, "manager", 111.11));
        QCOMPARE(getLastTipAmount(), 111.11);
    }

    void test_WaiterCanCorrectOwnTip() {
        insertTip(20, 66.0);
        QVERIFY(manager.correctLastTipAmount(20, "waiter", 77.77));
        QCOMPARE(getLastTipAmount(), 77.77);
    }

    void test_WaiterCannotCorrectOthersTip() {
        insertTip(30, 88.0);
        QVERIFY(!manager.correctLastTipAmount(99, "waiter", 999.99));
        QCOMPARE(getLastTipAmount(), 88.0);
    }

    void test_InvalidRoleDenied() {
        insertTip(40, 123.0);
        QVERIFY(!manager.correctLastTipAmount(40, "client", 222.0));
        QCOMPARE(getLastTipAmount(), 123.0);
    }

    void test_UpdateFails_ReturnsFalse() {
        insertTip(50, 60.0);
        QVERIFY(QSqlQuery().exec("DROP TABLE tips"));
        QVERIFY(!manager.correctLastTipAmount(50, "manager", 999.0));
    }
};

QTEST_MAIN(TipManagerTest)
#include "TipManagerTest.moc"
