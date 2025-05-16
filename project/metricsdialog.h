#ifndef METRICSDIALOG_H
#define METRICSDIALOG_H

#include <QDialog>
#include <QString>
#include "PerformanceService.h"

namespace Ui {
class MetricsDialog;
}

class MetricsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MetricsDialog(int userId, const QString& role, QWidget *parent = nullptr);
    ~MetricsDialog();

private:
    Ui::MetricsDialog *ui;
    int m_userId;
    QString m_role;
    PerformanceService m_service;

    void fillTable(const WaiterMetrics& metrics);

private slots:
    void on_btnLoadMetrics_clicked();
};

#endif // METRICSDIALOG_H
