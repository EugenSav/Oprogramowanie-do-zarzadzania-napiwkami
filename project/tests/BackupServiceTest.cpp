#include <QtTest>
#include <QDir>
#include <QFile>
#include "BackupService.h"

class BackupServiceTest : public QObject {
    Q_OBJECT

private:
    QString testDbPath;
    QString backupDir;

    void createDummyDatabase() {
        QFile db(testDbPath);
        if (db.exists()) db.remove();
        QVERIFY(db.open(QIODevice::WriteOnly));
        db.write("Dummy database content");
        db.close();
    }

private slots:
    void initTestCase() {
        QDir().mkpath("test_backups");
        backupDir = QDir::current().absoluteFilePath("test_backups");
        testDbPath = QDir::current().absoluteFilePath("test_database.db");
        createDummyDatabase();
    }

    void cleanup() {
        QDir dir(backupDir);
        for (const QFileInfo& f : dir.entryInfoList(QDir::Files)) {
            QFile::remove(f.absoluteFilePath());
        }
    }

    void testCreateBackup_successful() {
        QString backupPath;
        BackupService service(testDbPath);
        QVERIFY(service.createBackup(backupDir, backupPath));
        QVERIFY(QFile::exists(backupPath));
    }

    void testCreateBackup_emptyPath() {
        QString backupPath;
        BackupService service(testDbPath);
        QVERIFY(!service.createBackup("", backupPath));
    }

    void testCreateBackup_generatesUniqueFile() {
        QString path1, path2;
        BackupService service(testDbPath);

        QVERIFY(service.createBackup(backupDir, path1));
        QTest::qWait(1000);
        QVERIFY(service.createBackup(backupDir, path2));

        QVERIFY(path1 != path2);
        QVERIFY(QFile::exists(path1));
        QVERIFY(QFile::exists(path2));
    }

    void testBackupContentMatchesOriginal() {
        QString backupPath;
        BackupService service(testDbPath);
        QVERIFY(service.createBackup(backupDir, backupPath));

        QFile orig(testDbPath);
        QFile copy(backupPath);
        QVERIFY(orig.open(QIODevice::ReadOnly));
        QVERIFY(copy.open(QIODevice::ReadOnly));
        QCOMPARE(orig.readAll(), copy.readAll());
    }

    void testCreateBackup_invalidDestination() {
        QString backupPath;
        QString invalidDir = "C:/Windows/System32";
        BackupService service(testDbPath);
        QVERIFY(!service.createBackup(invalidDir, backupPath));
    }

    void testCreateBackup_filenameFormat() {
        QString backupPath;
        BackupService service(testDbPath);
        QVERIFY(service.createBackup(backupDir, backupPath));

        QFileInfo info(backupPath);
        QString name = info.fileName();

        QRegularExpression regex(R"(backup_\d{4}-\d{2}-\d{2}_\d{2}-\d{2}-\d{2}\.db)");
        QVERIFY(regex.match(name).hasMatch());
    }

    void testCreateBackup_notEmptyFile() {
        QString path;
        BackupService service(testDbPath);
        QVERIFY(service.createBackup(backupDir, path));

        QFileInfo info(path);
        QVERIFY(info.size() > 0);
    }
};

QTEST_MAIN(BackupServiceTest)
#include "BackupServiceTest.moc"
