#include <QtTest>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include "UserRepository.h"

class UserRepositoryTest : public QObject {
    Q_OBJECT

private:
    QSqlDatabase db;

private slots:
    void initTestCase() {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(":memory:");
        QVERIFY(db.open());

        QSqlQuery query;
        QVERIFY(query.exec(R"(
            CREATE TABLE users (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                username TEXT NOT NULL UNIQUE,
                password TEXT NOT NULL,
                role TEXT NOT NULL
            )
        )"));
    }

    void cleanupTestCase() {
        db.close();
    }

    void cleanup() {
        QSqlQuery cleanup;
        cleanup.exec("DELETE FROM users");
    }

    void test_insertUser_validRoles() {
        UserRepository repo;
        QVERIFY(repo.insertUser("janek", "haslo123", "waiter"));
        QVERIFY(repo.insertUser("zosia", "sekret", "manager"));
        QVERIFY(repo.insertUser("admin_arek", "adminpass", "admin"));

        QSqlQuery q("SELECT COUNT(*) FROM users");
        QVERIFY(q.next());
        QCOMPARE(q.value(0).toInt(), 3);
    }

    void test_insertUser_missingFields() {
        UserRepository repo;
        QVERIFY(!repo.insertUser("", "tajne", "waiter"));           // brak loginu
        QVERIFY(!repo.insertUser("basia", "", "manager"));          // brak hasła
        QVERIFY(!repo.insertUser("marek", "haslo", ""));            // brak roli

        QSqlQuery q("SELECT COUNT(*) FROM users");
        QVERIFY(q.next());
        QCOMPARE(q.value(0).toInt(), 0);
    }

    void test_insertUser_invalidRoles() {
        UserRepository repo;
        QVERIFY(!repo.insertUser("wanda", "haslo", "kucharz"));     // nieznana rola
        QVERIFY(!repo.insertUser("tomek", "haslo", "admin2"));      // nieznana rola
        QVERIFY(!repo.insertUser("ola", "haslo", "ADMIN"));         // case-sensitive

        QSqlQuery q("SELECT COUNT(*) FROM users");
        QVERIFY(q.next());
        QCOMPARE(q.value(0).toInt(), 0);
    }

    void test_insertUser_duplicateUsername() {
        UserRepository repo;
        QVERIFY(repo.insertUser("alicja", "pass1", "waiter"));
        QVERIFY(!repo.insertUser("alicja", "pass2", "manager")); // duplikat

        QSqlQuery q("SELECT COUNT(*) FROM users WHERE username = 'alicja'");
        QVERIFY(q.next());
        QCOMPARE(q.value(0).toInt(), 1);
    }

    void test_deleteUser_valid() {
        UserRepository repo;
        QVERIFY(repo.insertUser("stefan", "haslo", "waiter"));

        QSqlQuery q("SELECT id FROM users WHERE username = 'stefan'");
        QVERIFY(q.next());
        int id = q.value(0).toInt();

        QVERIFY(repo.deleteUser(id));

        QSqlQuery check("SELECT COUNT(*) FROM users WHERE id = " + QString::number(id));
        QVERIFY(check.next());
        QCOMPARE(check.value(0).toInt(), 0);
    }

    void test_deleteUser_nonExistent() {
        UserRepository repo;
        QVERIFY(!repo.deleteUser(9999)); // użytkownik nie istnieje
    }

    void test_deleteUser_invalidId() {
        UserRepository repo;
        QVERIFY(!repo.deleteUser(-5));
        QVERIFY(!repo.deleteUser(0));
    }
};

QTEST_MAIN(UserRepositoryTest)
#include "UserRepositoryTest.moc"
