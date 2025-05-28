#include "MetricsDialog.h"
#include "ui_MetricsDialog.h"
#include <QDebug>
#include <QMessageBox>

MetricsDialog::MetricsDialog(int userId, const QString& role, PerformanceService* service, QWidget *parent)
    : QDialog(parent),
    ui(new Ui::MetricsDialog),
    m_userId(userId),
    m_role(role),
    m_service(service)
{
    ui->setupUi(this);

    if (m_role == "waiter") {
        ui->waiterIdSpinBox->hide();
        ui->labelWaiterId->hide();
        ui->btnLoadMetrics->hide();

        WaiterMetrics metrics = m_service->getWaiterPerformanceMetrics(m_userId, m_role, m_userId);
        fillTable(metrics);
    } else {
        ui->waiterIdSpinBox->setMinimum(1);
        ui->waiterIdSpinBox->setValue(m_userId);
        connect(ui->btnLoadMetrics, &QPushButton::clicked, this, &MetricsDialog::on_btnLoadMetrics_clicked);
    }
}

MetricsDialog::~MetricsDialog()
{
    delete ui;
}

void MetricsDialog::on_btnLoadMetrics_clicked()
{
    int targetId = ui->waiterIdSpinBox->value();
    WaiterMetrics metrics = m_service->getWaiterPerformanceMetrics(m_userId, m_role, targetId);
    if (metrics.success) {
        fillTable(metrics);
    } else {
        QMessageBox::warning(this, "Błąd", "Nie udało się pobrać metryk.");
    }
}

void MetricsDialog::fillTable(const WaiterMetrics& m)
{
    ui->tableMetrics->setRowCount(4);
    ui->tableMetrics->setColumnCount(2);
    ui->tableMetrics->setHorizontalHeaderLabels(QStringList() << "Metryka" << "Wartość");

    ui->tableMetrics->setItem(0, 0, new QTableWidgetItem("Liczba napiwków"));
    ui->tableMetrics->setItem(0, 1, new QTableWidgetItem(QString::number(m.totalTipsCount)));

    ui->tableMetrics->setItem(1, 0, new QTableWidgetItem("Łączna kwota"));
    ui->tableMetrics->setItem(1, 1, new QTableWidgetItem(QString::number(m.totalAmountAttributed, 'f', 2)));

    ui->tableMetrics->setItem(2, 0, new QTableWidgetItem("Średnia kwota"));
    ui->tableMetrics->setItem(2, 1, new QTableWidgetItem(QString::number(m.averageAmountAttributed, 'f', 2)));

    ui->tableMetrics->setItem(3, 0, new QTableWidgetItem("Procent dzielonych"));
    ui->tableMetrics->setItem(3, 1, new QTableWidgetItem(QString::number(m.sharedTipsPercentage, 'f', 2) + "%"));

    ui->tableMetrics->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->tableMetrics->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableMetrics->verticalHeader()->setVisible(false);
}
