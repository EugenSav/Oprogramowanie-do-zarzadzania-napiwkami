#ifndef REPORTGENERATOR_H
#define REPORTGENERATOR_H

#include <QString>

class ReportGenerator
{
public:
    explicit ReportGenerator(const QString& databasePath);
    bool generateMonthlyReports(const QString& outputDir, int year, int month);

private:
    QString m_databasePath;
    bool generateReportForUser(int userId, const QString& username, const QString& outputDir, const QString& periodStart, const QString& periodEnd);
};

#endif // REPORTGENERATOR_H
