#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "LoginDialog.h"
#include <QDebug>
#include "PerformanceService.h"
#include <QInputDialog>
#include "MetricsDialog.h"
#include "AddTipDialog.h"
#include "TipManager.h"
#include <QMessageBox>
#include "editlasttipdialog.h"
#include "BalanceDialog.h"
#include "UserManagementDialog.h"
#include "BackupService.h"
#include <QFileDialog>
#include <QDate>
#include "ReportGenerator.h"
#include "SqlTipMetricsRepository.h"


MainWindow::MainWindow(int userId, const QString& role, QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_userId(userId),
    m_role(role)
{
    ui->setupUi(this);
    setupForRole();
    connect(ui->btnLogout, &QPushButton::clicked, this, &MainWindow::logout);
    connect(ui->btnPerformanceMetrics, &QPushButton::clicked, this, &MainWindow::onPerformanceMetricsClicked);
    connect(ui->btnAddTip, &QPushButton::clicked, this, &MainWindow::onAddTipClicked);
    connect(ui->btnEditLastTip, &QPushButton::clicked, this, &MainWindow::onEditLastTipClicked);
    connect(ui->btnCheckBalance, &QPushButton::clicked, this, &MainWindow::onCheckBalanceClicked);
    connect(ui->btnBackup, &QPushButton::clicked, this, &MainWindow::createBackup);
    connect(ui->btnGenerateReports, &QPushButton::clicked, this, &MainWindow::onGenerateReportsClicked);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupForRole()
{
    ui->labelLoggedUser->setText("Zalogowano jako: " + m_role);

    if (m_role == "waiter") {
        ui->btnAddTip->show();
        ui->btnEditLastTip->show();
        ui->btnCheckBalance->show();
        ui->btnCheckBalance->setText("Sprawdź swoje saldo");
        ui->btnPerformanceMetrics->show();
        ui->btnPerformanceMetrics->setText("Sprawdź swoje metryki");

        ui->btnManagerTools->hide();
        ui->btnAdminPanel->hide();
        ui->btnBackup->hide();
        ui->btnGenerateReports->hide();

    }
    else if (m_role == "manager") {
        ui->btnAddTip->show();
        ui->btnEditLastTip->show();
        ui->btnCheckBalance->show();
        ui->btnCheckBalance->setText("Sprawdź swoje saldo lub innych kelnerów");
        ui->btnPerformanceMetrics->show();
        ui->btnPerformanceMetrics->setText("Sprawdź metryki kelnerów");


        ui->btnManagerTools->hide();
        ui->btnAdminPanel->hide();
        ui->btnBackup->hide();
        ui->btnGenerateReports->hide();
    }
    else if (m_role == "admin") {
        ui->btnAddTip->hide();
        ui->btnEditLastTip->hide();
        ui->btnCheckBalance->hide();
        ui->btnManagerTools->hide();
        ui->btnPerformanceMetrics->hide();
        ui->btnAdminPanel->show();
        ui->btnBackup->show();
        ui->btnGenerateReports->show();
        connect(ui->btnAdminPanel, &QPushButton::clicked, this, [this]() {
            UserManagementDialog dialog(this);
            dialog.exec();
        });

    }
}

void MainWindow::logout()
{
    this->close();

    LoginDialog login;
    if (login.exec() == QDialog::Accepted) {
        MainWindow *newWindow = new MainWindow(login.userId, login.role);
        newWindow->show();
    }
}

void MainWindow::onPerformanceMetricsClicked()
{
    auto* repository = new SqlTipMetricsRepository();
    auto* service = new PerformanceService(repository);
    auto* dialog = new MetricsDialog(m_userId, m_role, service, this);

    dialog->exec();
}

void MainWindow::onAddTipClicked()
{
    AddTipDialog dialog(m_userId, this);
    if (dialog.exec() == QDialog::Accepted) {
        TipManager tm;
        double amount = dialog.amount();
        int secondId = dialog.secondWaiterId();

        bool success;
        if (secondId == 0 || secondId == m_userId) {
            success = tm.addTip(amount, m_userId, std::nullopt);
        } else {
            success = tm.addTip(amount, m_userId, secondId);
        }

        if (success) {
            QMessageBox::information(this, "Sukces", "Napiwek został dodany.");
        } else {
            QMessageBox::warning(this, "Błąd", "Nie udało się dodać napiwku.");
        }
    }
}

void MainWindow::onEditLastTipClicked()
{
    EditLastTipDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        double newAmount = dialog.newAmount();

        TipManager tm;
        bool success = tm.correctLastTipAmount(m_userId, m_role, newAmount);

        if (success) {
            QMessageBox::information(this, "Sukces", "Napiwek został zaktualizowany.");
        } else {
            QMessageBox::warning(this, "Błąd", "Nie udało się zaktualizować napiwku.\nUpewnij się, że masz do niego dostęp.");
        }
    }
}

void MainWindow::onCheckBalanceClicked()
{
    BalanceDialog *dialog = new BalanceDialog(m_userId, m_role, this);
    dialog->exec();
}

void MainWindow::createBackup()
{
    QString dbPath = "/Users/maciejbajer/Documents/sem4/io/tips.db";

    QString destDir = QFileDialog::getExistingDirectory(this, "Wybierz folder do zapisu kopii");
    if (destDir.isEmpty())
        return;

    BackupService backupService(dbPath);
    QString backupPath;

    if (backupService.createBackup(destDir, backupPath)) {
        QMessageBox::information(this, "Sukces", "Kopia zapasowa została utworzona:\n" + backupPath);
    } else {
        QMessageBox::warning(this, "Błąd", "Nie udało się utworzyć kopii zapasowej.");
    }
}

void MainWindow::generateMonthlyReports()
{
    QString destDir = QFileDialog::getExistingDirectory(this, "Wybierz folder na raporty");
    if (destDir.isEmpty()) return;

    QDate current = QDate::currentDate();
    int year = current.year();
    int month = current.month();

    ReportGenerator generator("/Users/maciejbajer/project/tips.db");

    if (generator.generateMonthlyReports(destDir, year, month)) {
        QMessageBox::information(this, "Sukces", "Raporty wygenerowane.");
    } else {
        QMessageBox::warning(this, "Błąd", "Nie udało się wygenerować wszystkich raportów.");
    }
}

void MainWindow::onGenerateReportsClicked()
{
    QDate current = QDate::currentDate();

    bool ok;
    int year = QInputDialog::getInt(this, "Wybierz rok", "Rok:", current.year(), 2000, 2100, 1, &ok);
    if (!ok) return;

    int month = QInputDialog::getInt(this, "Wybierz miesiąc", "Miesiąc (1-12):", current.month(), 1, 12, 1, &ok);
    if (!ok) return;

    QString destDir = QFileDialog::getExistingDirectory(this, "Wybierz folder na raporty");
    if (destDir.isEmpty()) return;

    QString dbPath = "/Users/maciejbajer/Documents/sem4/io/tips.db";
    ReportGenerator generator(dbPath);

    if (generator.generateMonthlyReports(destDir, year, month)) {
        QMessageBox::information(this, "Sukces", "Raporty CSV zostały wygenerowane.");
    } else {
        QMessageBox::warning(this, "Błąd", "Nie udało się wygenerować wszystkich raportów.");
    }
}
