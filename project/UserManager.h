#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <QString>

class UserManager {
public:
    friend class UserManagerTest;
    bool login(const QString& username, const QString& password);
    QString getRole(const QString& username);
    int getUserId(const QString& username);

private:
    QString hashPassword(const QString& password);
};

#endif // USERMANAGER_H
