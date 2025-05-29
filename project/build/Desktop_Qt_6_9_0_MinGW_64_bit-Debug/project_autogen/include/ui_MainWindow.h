/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QLabel *labelLoggedUser;
    QPushButton *btnAddTip;
    QPushButton *btnEditLastTip;
    QPushButton *btnCheckBalance;
    QPushButton *btnManagerTools;
    QPushButton *btnAdminPanel;
    QPushButton *btnPerformanceMetrics;
    QPushButton *btnBackup;
    QPushButton *btnGenerateReports;
    QPushButton *btnLogout;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayoutWidget = new QWidget(centralwidget);
        verticalLayoutWidget->setObjectName("verticalLayoutWidget");
        verticalLayoutWidget->setGeometry(QRect(200, 80, 311, 332));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        labelLoggedUser = new QLabel(verticalLayoutWidget);
        labelLoggedUser->setObjectName("labelLoggedUser");

        verticalLayout->addWidget(labelLoggedUser);

        btnAddTip = new QPushButton(verticalLayoutWidget);
        btnAddTip->setObjectName("btnAddTip");

        verticalLayout->addWidget(btnAddTip);

        btnEditLastTip = new QPushButton(verticalLayoutWidget);
        btnEditLastTip->setObjectName("btnEditLastTip");

        verticalLayout->addWidget(btnEditLastTip);

        btnCheckBalance = new QPushButton(verticalLayoutWidget);
        btnCheckBalance->setObjectName("btnCheckBalance");

        verticalLayout->addWidget(btnCheckBalance);

        btnManagerTools = new QPushButton(verticalLayoutWidget);
        btnManagerTools->setObjectName("btnManagerTools");

        verticalLayout->addWidget(btnManagerTools);

        btnAdminPanel = new QPushButton(verticalLayoutWidget);
        btnAdminPanel->setObjectName("btnAdminPanel");

        verticalLayout->addWidget(btnAdminPanel);

        btnPerformanceMetrics = new QPushButton(verticalLayoutWidget);
        btnPerformanceMetrics->setObjectName("btnPerformanceMetrics");

        verticalLayout->addWidget(btnPerformanceMetrics);

        btnBackup = new QPushButton(verticalLayoutWidget);
        btnBackup->setObjectName("btnBackup");

        verticalLayout->addWidget(btnBackup);

        btnGenerateReports = new QPushButton(verticalLayoutWidget);
        btnGenerateReports->setObjectName("btnGenerateReports");

        verticalLayout->addWidget(btnGenerateReports);

        btnLogout = new QPushButton(verticalLayoutWidget);
        btnLogout->setObjectName("btnLogout");

        verticalLayout->addWidget(btnLogout);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 24));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        labelLoggedUser->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:14pt; font-weight:700;\">Zalogowano jako: </span></p></body></html>", nullptr));
        btnAddTip->setText(QCoreApplication::translate("MainWindow", "Dodaj napiwek", nullptr));
        btnEditLastTip->setText(QCoreApplication::translate("MainWindow", "Edytuj ostatni napiwek", nullptr));
        btnCheckBalance->setText(QCoreApplication::translate("MainWindow", "Sprawdz saldo", nullptr));
        btnManagerTools->setText(QCoreApplication::translate("MainWindow", "Sprawdz saldo innych kelner\303\263w", nullptr));
        btnAdminPanel->setText(QCoreApplication::translate("MainWindow", "Zarz\304\205dzanie pracownikami", nullptr));
        btnPerformanceMetrics->setText(QCoreApplication::translate("MainWindow", "Sprawd\305\272 metryki kelener\303\263w", nullptr));
        btnBackup->setText(QCoreApplication::translate("MainWindow", "Utw\303\263rz kopi\304\231 zapasow\304\205", nullptr));
        btnGenerateReports->setText(QCoreApplication::translate("MainWindow", "Generuj raporty miesi\304\231czne", nullptr));
        btnLogout->setText(QCoreApplication::translate("MainWindow", "Wyloguj", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
