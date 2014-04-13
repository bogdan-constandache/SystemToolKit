#ifndef PROCESSES_STRUCTS_H
#define PROCESSES_STRUCTS_H

//My includes

//WinAPI includes

//QT includes
#include <QString>
#include <QList>

typedef struct _Module_
{
    qint32 qnPID;
    QString qszModuleName;
    QString qszModuleExePath;
    qint32 qnBaseSize;
}Module;

typedef struct _Process_
{
    QString qszProcessName;
    QString qszProcessFileName;
    QString qszProcessCommandLine;
    qint32 qnPID;
    qint32 qnNumberOfThreads;
    bool bType;
    qint32 qnMemoryUsed;
    qint32 qnPageFileUsage;
}Process;

#endif
