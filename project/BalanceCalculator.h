#ifndef BALANCECALCULATOR_H
#define BALANCECALCULATOR_H

#include <QString>
#include <QDate>
#include <QDebug>

class BalanceCalculator {
public:
    BalanceCalculator();
    double getBalanceInRange(int requesterId, const QString& role, int targetWaiterId, const QDate& from, const QDate& to);
    double getTodayBalance(int requesterId, const QString& role, int targetWaiterId);
    double getMonthlyBalance(int requesterId, const QString& role, int targetWaiterId);
};

#endif // BALANCECALCULATOR_H
