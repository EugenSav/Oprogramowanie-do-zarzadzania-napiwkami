#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "TipManager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(int userId, const QString& role, QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    int m_userId;
    QString m_role;

    void setupForRole();

private slots:
    void logout();
    void onPerformanceMetricsClicked();
    void onAddTipClicked();
    void onEditLastTipClicked();
    void onCheckBalanceClicked();
    void createBackup();
    void onGenerateReportsClicked();
    void generateMonthlyReports();
};

#endif // MAINWINDOW_H
