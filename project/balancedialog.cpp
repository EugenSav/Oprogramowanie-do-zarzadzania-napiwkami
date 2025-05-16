#include "BalanceDialog.h"
#include "ui_BalanceDialog.h"
#include "BalanceCalculator.h"
#include <QDate>
#include <QDebug>

BalanceDialog::BalanceDialog(int userId, const QString& role, QWidget *parent)
    : QDialog(parent),
    ui(new Ui::BalanceDialog),
    m_userId(userId),
    m_role(role)
{
    ui->setupUi(this);

    if (m_role == "waiter") {
        // Kelner nie może wybierać ID — widzi tylko własne saldo
        ui->waiterIdSpinBox->hide();
        ui->labelWaiterId->hide();
        ui->btnLoadBalance->hide();
        loadBalance(m_userId);
    } else {
        // Manager — domyślnie własne ID wpisane, ale może zmienić
        ui->waiterIdSpinBox->setMinimum(1);
        ui->waiterIdSpinBox->setValue(m_userId);

        connect(ui->btnLoadBalance, &QPushButton::clicked, this, &BalanceDialog::on_btnLoadBalance_clicked);
    }
}

BalanceDialog::~BalanceDialog()
{
    delete ui;
}

void BalanceDialog::on_btnLoadBalance_clicked()
{
    int targetId = ui->waiterIdSpinBox->value();
    loadBalance(targetId);
}

void BalanceDialog::loadBalance(int targetWaiterId)
{
    BalanceCalculator calculator;

    double daily = calculator.getTodayBalance(m_userId, m_role, targetWaiterId);
    double monthly = calculator.getMonthlyBalance(m_userId, m_role, targetWaiterId);

    if (daily < 0 || monthly < 0) {
        ui->labelDailyBalance->setText("Saldo dzienne: Błąd dostępu");
        ui->labelMonthlyBalance->setText("Saldo miesięczne: Błąd dostępu");
        return;
    }

    ui->labelDailyBalance->setText("Saldo dzienne: " + QString::number(daily, 'f', 2));
    ui->labelMonthlyBalance->setText("Saldo miesięczne: " + QString::number(monthly, 'f', 2));
}
