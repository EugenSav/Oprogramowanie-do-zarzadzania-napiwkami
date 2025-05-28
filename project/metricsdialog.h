#include "PerformanceService.h"
#include "ui_metricsdialog.h"

class MetricsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MetricsDialog(int userId, const QString& role, PerformanceService* service, QWidget *parent = nullptr);
    ~MetricsDialog();

private slots:
    void on_btnLoadMetrics_clicked();

private:
    void fillTable(const WaiterMetrics& m);

    Ui::MetricsDialog *ui;
    int m_userId;
    QString m_role;
    PerformanceService* m_service;
};
