#ifndef SYSTEM_USERS_INFORMATION_H
#define SYSTEM_USERS_INFORMATION_H

// WinAPI includes
#include <Windows.h>
#include <LMaccess.h>

// Qt includes
#include <QString>
#include <QList>
#include <QDateTime>
#include <QStandardItemModel>

// My includes
#include "../../../utils/headers/utils.h"
#include "system_users_information_structs.h"

class CSystemUsersInformation
{
private:
    QList<UserInformation*> m_qlUsers;
    QStandardItemModel      *m_pDataModel;

    int Initialize(void);

public:
    CSystemUsersInformation();
    ~CSystemUsersInformation();
    QStandardItemModel *GetUserInformations();
};

#endif // SYSTEM_USERS_INFORMATION_H
