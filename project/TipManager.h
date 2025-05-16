#ifndef TIPMANAGER_H
#define TIPMANAGER_H

#include <QString>
#include <optional>
#include <QDate>


class TipManager {
public:
    bool addTip(double amount, int waiter1Id, std::optional<int> waiter2Id = std::nullopt);
    bool correctLastTipAmount(int requesterId, const QString& role, double newAmount);

};
#endif // TIPMANAGER_H
