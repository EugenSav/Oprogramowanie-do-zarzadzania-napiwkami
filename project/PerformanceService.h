#ifndef PERFORMANCESERVICE_H
#define PERFORMANCESERVICE_H

#include <QString>
#include <QDate>
#include <QDebug>

struct WaiterMetrics {
    int targetWaiterId = -1;
    int totalTipsCount = 0;
    double totalAmountAttributed = 0.0;
    double averageAmountAttributed = 0.0;
    double sharedTipsPercentage = 0.0;
    bool success = false;

    // void print() const {
    //     if (success) {
    //         qDebug() << "--- Metryki wydajności dla kelnera (ID:" << targetWaiterId << ") ---";
    //         qDebug() << "Okres: Ostatnie 3 miesiące"; // Możesz dodać wyświetlanie zakresu dat, jeśli masz go w strukturze
    //         qDebug() << "Całkowita liczba napiwków:" << totalTipsCount;
    //         qDebug() << "Łączna przypisana kwota:" << QString::number(totalAmountAttributed, 'f', 2); // Formatowanie do 2 miejsc po przecinku
    //         qDebug() << "Średnia przypisana kwota na napiwek:" << QString::number(averageAmountAttributed, 'f', 2);
    //         qDebug() << "Procent napiwków dzielonych:" << QString::number(sharedTipsPercentage, 'f', 2) << "%";
    //         qDebug() << "--------------------------------------------------";
    //     } else {
    //         // Komunikat o niepowodzeniu jest wyświetlany wewnątrz funkcji getWaiterPerformanceMetrics
    //     }
    // }
};

class PerformanceService {
public:
    PerformanceService();

    WaiterMetrics getWaiterPerformanceMetrics(int requesterId, const QString& role, int targetWaiterId);
};
#endif // PERFORMANCESERVICE_H
