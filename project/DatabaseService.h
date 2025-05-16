#ifndef DATABASESERVICE_H
#define DATABASESERVICE_H

#include <QString>

class DatabaseService {
public:
    static bool connect(const QString& dbPath = "tips.db");
};

#endif // DATABASESERVICE_H
