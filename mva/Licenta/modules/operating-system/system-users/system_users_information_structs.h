#ifndef SYSTEM_USERS_INFORMATION_STRUCTS_H
#define SYSTEM_USERS_INFORMATION_STRUCTS_H

typedef struct _USER_INFORMATION_
{
    QString UserName;
    QString FullUserName;
    QString Privilege;
    QString HomeDir;
    QString LastLogon;
    QString LastLogoff;
    QString LogonServer;
    QString CountryCode;
}UserInformation;

#endif // SYSTEM_USERS_INFORMATION_STRUCTS_H
