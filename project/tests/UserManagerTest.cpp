#include <QtTest>
#include "UserManager.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QRegularExpression>

class UserManagerTest : public QObject {
    Q_OBJECT

private:
    UserManager userManager;
    QSqlDatabase db;

private slots:
    void initTestCase() {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(":memory:");
        QVERIFY(db.open());

        QSqlQuery query;
        QVERIFY(query.exec("CREATE TABLE users (id INTEGER PRIMARY KEY, username TEXT, password TEXT, role TEXT)"));

        struct UserInfo {
            QString username;
            QString password;
            QString role;
        };

        QList<UserInfo> users = {
            {"alice",     "secret",     "manager"},
            {"admin",     "adminpass",  "admin"},
            {"kelner1",   "haslo123",   "kelner"},
            {"friend",    "pass_word",   "observer"},
            {"USER",      "123456",     "uppercase"},
            {"emoji🧪",   "test",       "unicode"}
        };

        for (const auto& u : users) {
            QString hash = userManager.hashPassword(u.password);
            query.prepare("INSERT INTO users (username, password, role) VALUES (?, ?, ?)");
            query.addBindValue(u.username);
            query.addBindValue(hash);
            query.addBindValue(u.role);
            QVERIFY2(query.exec(), query.lastError().text().toUtf8().constData());
        }
    }

    void cleanupTestCase() {
        db.close();
    }

    void testHashPassword() {

        QString p1 = "password123";
        QString p2 = "password123";
        QString p3 = "Password123";
        QString p4 = "pss wrd";
        QString p5 = "";
        QString p6 = "??????";
        QString p7 = QString(1000, 'x');

        QString h1 = userManager.hashPassword(p1);
        QString h2 = userManager.hashPassword(p2);
        QString h3 = userManager.hashPassword(p3);
        QString h4 = userManager.hashPassword(p4);
        QString h5 = userManager.hashPassword(p5);
        QString h6 = userManager.hashPassword(p6);
        QString h7 = userManager.hashPassword(p7);

        QCOMPARE(h1, h2);

        QVERIFY(h1 != h3);
        QVERIFY(h1 != h4);
        QVERIFY(h1 != h5);
        QVERIFY(h1 != h6);

        QCOMPARE(h1.length(), 64);
        QCOMPARE(h4.length(), 64);
        QCOMPARE(h5.length(), 64);
        QCOMPARE(h6.length(), 64);
        QCOMPARE(h7.length(), 64);

        QVERIFY(!h1.isEmpty());
        QVERIFY(!h5.isEmpty());
        QVERIFY(!h6.isEmpty());

        QRegularExpression hexRegex("^[0-9a-f]{64}$");
        QVERIFY(hexRegex.match(h1).hasMatch());
        QVERIFY(hexRegex.match(h4).hasMatch());
        QVERIFY(hexRegex.match(h6).hasMatch());
    }

    void testLogin_data() {
        QTest::addColumn<QString>("username");
        QTest::addColumn<QString>("password");
        QTest::addColumn<bool>("expected");

        QTest::newRow("alice correct") << "alice" << "secret" << true;
        QTest::newRow("admin correct") << "admin" << "adminpass" << true;
        QTest::newRow("kelner correct") << "kelner1" << "haslo123" << true;
        QTest::newRow("friend correct") << "friend" << "pass_word" << true;
        QTest::newRow("uppercase correct") << "USER" << "123456" << true;
        QTest::newRow("emoji correct") << "emoji🧪" << "test" << true;

        QTest::newRow("alice wrong password") << "alice" << "wrong" << false;
        QTest::newRow("admin wrong") << "admin" << "pass" << false;

        QTest::newRow("unknown user") << "bob" << "secret" << false;

        QTest::newRow("alise with space") << "alice " << "secret" << false;
        QTest::newRow("space with alice") << " alice" << "secret" << false;

        QTest::newRow("sql inject username") << "' OR 1=1 --" << "secret" << false;
        QTest::newRow("sql inject password") << "alice" << "' OR 1=1 --" << false;
        QTest::newRow("sql inject both") << "' OR 1=1 --" << "' OR 1=1 --" << false;

        QTest::newRow("empty username") << "" << "secret" << false;
        QTest::newRow("empty password") << "alice" << "" << false;
        QTest::newRow("empty both") << "" << "" << false;

        QTest::newRow("case mismatch") << "ALICE" << "secret" << false;
        QTest::newRow("case mismatch password") << "alice" << "SECRET" << false;

        QTest::newRow("long username") << QString(500, 'a') << "secret" << false;
        QTest::newRow("long password") << "alice" << QString(1000, 'p') << false;
        QTest::newRow("long both") << QString(500, 'a') << QString(1000, 'p') << false;
    }

    void testLogin() {
        QFETCH(QString, username);
        QFETCH(QString, password);
        QFETCH(bool, expected);

        bool result = userManager.login(username, password);
        QCOMPARE(result, expected);
    }

    void testGetRole_data() {
        QTest::addColumn<QString>("username");
        QTest::addColumn<QString>("expectedRole");

        QTest::newRow("manager user") << "alice" << "manager";
        QTest::newRow("admin user") << "admin" << "admin";
        QTest::newRow("kelner user") << "kelner1" << "kelner";
        QTest::newRow("friend user") << "friend" << "observer";
        QTest::newRow("emoji user") << "emoji🧪" << "unicode";
        QTest::newRow("uppercase name") << "USER" << "uppercase";

        QTest::newRow("nonexistent") << "bob" << "";
        QTest::newRow("empty login") << "" << "";
        QTest::newRow("wrong case") << "ALICE" << "";
        QTest::newRow("injected input") << "' OR 1=1 --" << "";
        QTest::newRow("long input") << QString(500, 'x') << "";
    }

    void testGetRole() {
        QFETCH(QString, username);
        QFETCH(QString, expectedRole);

        QCOMPARE(userManager.getRole(username), expectedRole);
    }

    void testGetUserId_data() {
        QTest::addColumn<QString>("username");
        QTest::addColumn<bool>("expectValidId");

        QTest::newRow("valid alice") << "alice" << true;
        QTest::newRow("valid admin") << "admin" << true;
        QTest::newRow("valid kelner1") << "kelner1" << true;
        QTest::newRow("unicode name") << "friend" << true;
        QTest::newRow("emoji name") << "emoji🧪" << true;

        QTest::newRow("nonexistent user") << "bob" << false;
        QTest::newRow("nonexistent user with space") << "bob " << false;
        QTest::newRow("nonexistent user with space at start") << " bob" << false;
        QTest::newRow("empty username") << "" << false;
        QTest::newRow("empty username with space") << " " << false;
        QTest::newRow("wrong case") << "ALICE" << false;
        QTest::newRow("sql injection") << "' OR 1=1 --" << false;
        QTest::newRow("long input") << QString(1000, 'x') << false;
    }

    void testGetUserId() {
        QFETCH(QString, username);
        QFETCH(bool, expectValidId);

        int id = userManager.getUserId(username);
        if (expectValidId) {
            QVERIFY(id > 0);
        }
        else {
            QCOMPARE(id, -1);
        }
    }
};

QTEST_MAIN(UserManagerTest)
#include "UserManagerTest.moc"
