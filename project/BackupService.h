#ifndef BACKUPSERVICE_H
#define BACKUPSERVICE_H

#include <QString>

class BackupService
{
public:
    explicit BackupService(const QString& databasePath);
    bool createBackup(const QString& destinationDirectory, QString& outBackupPath);

private:
    QString m_databasePath;
};

#endif // BACKUPSERVICE_H
