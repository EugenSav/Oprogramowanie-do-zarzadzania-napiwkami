#include "BackupService.h"
#include <QFile>
#include <QDateTime>

BackupService::BackupService(const QString& databasePath)
    : m_databasePath(databasePath)
{}

bool BackupService::createBackup(const QString& destinationDirectory, QString& outBackupPath)
{
    if (destinationDirectory.isEmpty())
        return false;

    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd_HH-mm-ss");
    outBackupPath = destinationDirectory + "/backup_" + timestamp + ".db";

    return QFile::copy(m_databasePath, outBackupPath);
}
