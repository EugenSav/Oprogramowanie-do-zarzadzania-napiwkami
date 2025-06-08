#include <QtTest>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include "ReportGenerator.h"

class ReportGeneratorTest : public QObject {
    Q_OBJECT

private:
    QSqlDatabase db;
    QString tempDirPath;

private slots:
    void initTestCase() {
        // Create test directory
        QDir().mkdir("test_reports");
        tempDirPath = QDir::currentPath() + "/test_reports";

        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(":memory:");
        QVERIFY(db.open());

        QSqlQuery query;
        QVERIFY(query.exec(R"(
            CREATE TABLE users (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                username TEXT NOT NULL,
                role TEXT NOT NULL
            )
        )"));
        QVERIFY(query.exec(R"(
            CREATE TABLE tips (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                date TEXT NOT NULL,
                amount REAL NOT NULL,
                waiter1_id INTEGER,
                waiter2_id INTEGER
            )
        )"));

        // Add multiple users
        query.exec("INSERT INTO users (username, role) VALUES ('waiter1', 'waiter')");
        query.exec("INSERT INTO users (username, role) VALUES ('waiter2', 'waiter')");
        query.exec("INSERT INTO users (username, role) VALUES ('manager1', 'manager')");
        query.exec("INSERT INTO users (username, role) VALUES ('odd_char_user!@#', 'waiter')");

        // Add tips
        query.exec(R"(INSERT INTO tips (date, amount, waiter1_id, waiter2_id) VALUES ('2025-06-01', 100.0, 1, NULL))");
        query.exec(R"(INSERT INTO tips (date, amount, waiter1_id, waiter2_id) VALUES ('2025-06-02', 80.0, 1, 2))");
        query.exec(R"(INSERT INTO tips (date, amount, waiter1_id, waiter2_id) VALUES ('2025-05-30', 60.0, 2, NULL))");  // outside range
        query.exec(R"(INSERT INTO tips (date, amount, waiter1_id, waiter2_id) VALUES ('2025-06-03', 200.0, 3, NULL))"); // manager
        query.exec(R"(INSERT INTO tips (date, amount, waiter1_id, waiter2_id) VALUES ('2025-06-04', 300.0, 4, 1))"); // shared with special-char user
    }

    void cleanupTestCase() {
        QDir dir(tempDirPath);
        for (const QString& fileName : dir.entryList(QDir::Files)) {
            QFile::remove(tempDirPath + "/" + fileName);
        }
        dir.rmdir(tempDirPath);
        db.close();
    }

    void test_generateReports_validData() {
        ReportGenerator generator(":memory:");
        QVERIFY(generator.generateMonthlyReports(tempDirPath, 2025, 6));

        QFile f1(tempDirPath + "/raport_waiter1_2025-06.csv");
        QVERIFY(f1.exists());
        QVERIFY(f1.open(QIODevice::ReadOnly));
        QTextStream in1(&f1);
        QStringList lines1 = in1.readAll().split("\n", Qt::SkipEmptyParts);
        QCOMPARE(lines1.size(), 4); // 3 tips + header   !!!
        f1.close();

        QFile f2(tempDirPath + "/raport_waiter2_2025-06.csv");
        QVERIFY(f2.exists());
        QVERIFY(f2.open(QIODevice::ReadOnly));
        QTextStream in2(&f2);
        QStringList lines2 = in2.readAll().split("\n", Qt::SkipEmptyParts);
        QCOMPARE(lines2.size(), 2); // 1 shared tip with waiter1
        f2.close();

        QFile f3(tempDirPath + "/raport_manager1_2025-06.csv");
        QVERIFY(f3.exists());
        QVERIFY(f3.open(QIODevice::ReadOnly));
        QTextStream in3(&f3);
        QStringList lines3 = in3.readAll().split("\n", Qt::SkipEmptyParts);
        QCOMPARE(lines3.size(), 2); // 1 manager tip
        f3.close();

        QFile f4(tempDirPath + "/raport_odd_char_user!@#_2025-06.csv");
        QVERIFY(f4.exists());
        QVERIFY(f4.open(QIODevice::ReadOnly));
        QTextStream in4(&f4);
        QStringList lines4 = in4.readAll().split("\n", Qt::SkipEmptyParts);
        QCOMPARE(lines4.size(), 2); // 1 shared tip with waiter1
        f4.close();
    }

    void test_generateReports_noTipsInRange() {
        ReportGenerator generator(":memory:");
        QVERIFY(generator.generateMonthlyReports(tempDirPath, 2024, 1)); // No tips in this month

        QStringList expectedFiles = {
            "raport_waiter1_2024-01.csv",
            "raport_waiter2_2024-01.csv",
            "raport_manager1_2024-01.csv",
            "raport_odd_char_user!@#_2024-01.csv"
        };

        for (const QString& fname : expectedFiles) {
            QFile f(tempDirPath + "/" + fname);
            QVERIFY(f.exists());
            QVERIFY(f.open(QIODevice::ReadOnly));
            QTextStream in(&f);
            QStringList lines = in.readAll().split("\n", Qt::SkipEmptyParts);
            QCOMPARE(lines.size(), 1); // Only header
            f.close();
        }
    }

    void test_generateReports_invalidOutputDir() {
        ReportGenerator generator(":memory:");
        QString badDir = "Z:/invalid_path_that_does_not_exist";
        QVERIFY(!generator.generateMonthlyReports(badDir, 2025, 6));
    }
};

QTEST_MAIN(ReportGeneratorTest)
#include "ReportGeneratorTest.moc"
