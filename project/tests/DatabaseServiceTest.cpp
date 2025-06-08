#include <QtTest>
#include <QSqlDatabase>
#include <QFile>
#include <QSqlQuery>
#include "DatabaseService.h"

class DatabaseServiceTest : public QObject {
    Q_OBJECT

private slots:
    void cleanup() {
        QSqlDatabase::removeDatabase("test_connection");
        QFile::remove("test_test.db");
    }

    void test_ConnectToNewFile_ShouldCreateDb() {
        QVERIFY(DatabaseService::connect("test_test.db"));
        QVERIFY(QFile::exists("test_test.db"));

        QSqlDatabase db = QSqlDatabase::database();
        QVERIFY(db.isOpen());
    }

    void test_ConnectToMemoryDatabase() {
        QSqlDatabase::removeDatabase("test_memory");
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "test_memory");
        db.setDatabaseName(":memory:");
        QVERIFY(db.open());
        QVERIFY(db.isOpen());
    }

    void test_ConnectToInvalidPath_ShouldFail() {
        QString path = "C:/Windows/System32/forbidden.db";
        bool connected = DatabaseService::connect(path);
        QVERIFY(!connected);
    }

    void test_ConnectTwice_SameConnectionUsed() {
        QVERIFY(DatabaseService::connect("test_test.db"));
        QVERIFY(QSqlDatabase::database().isOpen());

        QVERIFY(DatabaseService::connect("test_test.db"));
        QVERIFY(QSqlDatabase::database().isOpen());
    }

    void test_ConnectionOpensDatabase() {
        QVERIFY(DatabaseService::connect("test_test.db"));

        QSqlQuery q("CREATE TABLE IF NOT EXISTS test_table (id INTEGER PRIMARY KEY)");
        QVERIFY(q.exec());

        QVERIFY(QSqlQuery("SELECT * FROM test_table").exec());
    }

    void test_ConnectToReadOnlyLocation_ShouldFail() {
        QString systemPath = "C:/Windows/test.db";
        bool ok = DatabaseService::connect(systemPath);
        QVERIFY(!ok);
    }

    void test_FileActuallyCreated() {
        QString file = "new_db.db";
        QFile::remove(file);
        QVERIFY(DatabaseService::connect(file));
        QVERIFY(QFile::exists(file));
    }

    void test_ReconnectDoesNotDropTables() {
        QString file = "persistent.db";
        QVERIFY(DatabaseService::connect(file));
        QSqlQuery().exec("CREATE TABLE mydata (id INTEGER)");

        QVERIFY(DatabaseService::connect(file)); 
        QVERIFY(QSqlQuery("SELECT * FROM mydata").exec()); 
    }

    void test_DefaultPathUsedIfNoneProvided() {
        QFile::remove("tips.db");
        QVERIFY(DatabaseService::connect());
        QVERIFY(QFile::exists("tips.db"));
    }

    void test_DatabaseConnectionIsValid() {
        QVERIFY(DatabaseService::connect("validcheck.db"));
        QSqlDatabase db = QSqlDatabase::database();
        QVERIFY(db.isValid());
        QVERIFY(db.isOpen());
    }
};

QTEST_MAIN(DatabaseServiceTest)
#include "DatabaseServiceTest.moc"
