#ifndef SOFTWARE_STRUCTURES_H
#define SOFTWARE_STRUCTURES_H

typedef struct _STARTUP_APP_
{
    QString Name;
    QString Location;
    QString CommandLine;
}StartupApplication, *PStartupApplication;

typedef struct _APPLICATION
{
    QString Name;
    QString Version;
    QString Publisher;
    QString InstallDate;
    QString UninstallString;
}Application, *PApplication;

typedef enum USER_SCOPE
{
    ALL_USERS,
    CURRENT_USER
}UserScope;

#endif
