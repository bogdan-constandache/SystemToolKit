#ifndef STARTUP_MANAGER_H
#define STARTUP_MANAGER_H

#include <Windows.h>
#include <WinReg.h>
#include <tchar.h>
#include <objidl.h>
#include <shlobj.h>
#include <WinVer.h>

#include <QMap>
#include <QStringList>
#include <QList>
#include <QDir>
#include <QSysInfo>
#include <QStandardItemModel>

#include "../../../../utils/headers/utils.h"
#include "../../applications-manager/headers/software_structures.h"

#define STARTUP_LOCATION_1      "Software\\Microsoft\\Windows\\CurrentVersion\\Run"
#define STARTUP_LOCATION_2      "Software\\Microsoft\\Windows\\CurrentVersion\\RunOnce"

#define STARTUP_PATH_1          "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\StartupApproved"

typedef struct _Startup_Entry_
{
    QString qsName;                     // DONE
    QString qsPublisher;                // DONE
    QString qsLocation;                 // DONE
    QString qsCommandLine;              // DONE
    QString qsState;                    // DONE
    QString qsImpact;                   // WONT DO
    QString qsRunKeyPath;               // DONE
    QString qsRunHive;                  // DONE
    QString qsStartupApprovedKeyPath;   // DONE
    QString qsStartupHive;              // DONE
    bool    bX64;
}StartupEntry;

struct LANGANDCODEPAGE {
    WORD wLanguage;
    WORD wCodePage;
};

enum UserScope {
    ALL_USERS,
    CURRENT_USER
};

class CStartupManager : public QObject
{
    Q_OBJECT

private:
    QMap<QString, StartupEntry*>    m_qStartupEntries;

    QStandardItemModel              *m_pDataModel;

    void FillGenericApplications(HKEY hKey, QString qsKeyPath, bool bX64Hive);
    void FillApplicationsFromFolder(UserScope eScope);
    void CompleteGenericApplications(HKEY hKey, QString qsKeyPath, bool bX64Hive);

    QString GetCompanyNameOfExe(QString qsExePath);
    QString GetExePathOfShortcut(QString qsShortcutPath);

    void OnPopulateData();
    void OnClearInternalMap();

public:
    CStartupManager(QObject *pParent = 0);
    ~CStartupManager();

    QStandardItemModel *GetDataModel();
    void OnChangeApplicationState(QString qsApplication);
    void OnRemoveApplication(QString qsApplication);
    void OnAddApplication(QString qsAppName, QString qsPath);
    void OnRefreshData();
};

#endif // STARTUP_MANAGER_H
