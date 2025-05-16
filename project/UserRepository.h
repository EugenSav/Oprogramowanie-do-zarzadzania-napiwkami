#ifndef USERREPOSITORY_H
#define USERREPOSITORY_H

#include <QString>
#include <QDebug>
#include <QDate>
#include <QVariant>

class UserRepository {
public:
    bool insertUser(const QString& username, const QString& hashedPassword, const QString& role);

    bool deleteUser(int id);


};
#endif // USERREPOSITORY_H
