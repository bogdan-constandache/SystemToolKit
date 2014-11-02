#include "system_users_information.h"

CSystemUsersInformation::CSystemUsersInformation(): m_qlUsers(), m_pDataModel(NULL)
{
    m_pDataModel = new QStandardItemModel();
    m_pDataModel->setHorizontalHeaderLabels(QStringList() << "Users:" << "");

    int nStatus = Initialize();
    CHECK_OPERATION_STATUS(nStatus);

    QList<QStandardItem*> qList;

    foreach(UserInformation *pUser, m_qlUsers)
    {
        qList << new QStandardItem("User: ") << new QStandardItem(pUser->UserName == "" ? "N/A" : pUser->UserName);
        m_pDataModel->appendRow(qList); qList.clear();

        qList << new QStandardItem("Full name: ") << new QStandardItem(pUser->FullUserName == "" ? "N/A" : pUser->FullUserName);
        m_pDataModel->appendRow(qList); qList.clear();

        qList << new QStandardItem("Privilege: ") << new QStandardItem(pUser->Privilege == "" ? "N/A" : pUser->Privilege);
        m_pDataModel->appendRow(qList); qList.clear();

        qList << new QStandardItem("Home directory: ") << new QStandardItem(pUser->HomeDir == "" ? "N/A" : pUser->HomeDir);
        m_pDataModel->appendRow(qList); qList.clear();

        qList << new QStandardItem("Last logon: ") << new QStandardItem(pUser->LastLogon == "" ? "N/A" : pUser->LastLogon);
        m_pDataModel->appendRow(qList); qList.clear();

        qList << new QStandardItem("Last logoff: ") << new QStandardItem(pUser->LastLogoff == "" ? "N/A" : pUser->LastLogoff);
        m_pDataModel->appendRow(qList); qList.clear();

        qList << new QStandardItem("Country code: ") << new QStandardItem(pUser->CountryCode == "" ? "N/A" : pUser->CountryCode);
        m_pDataModel->appendRow(qList); qList.clear();

        qList << new QStandardItem("Logon server: ") << new QStandardItem(pUser->LogonServer == "" ? "N/A" : pUser->LogonServer);
        m_pDataModel->appendRow(qList); qList.clear();

        qList << new QStandardItem("") << new QStandardItem("");
        m_pDataModel->appendRow(qList); qList.clear();
    }

    m_pDataModel->removeRow(m_pDataModel->rowCount() - 1);
}

CSystemUsersInformation::~CSystemUsersInformation()
{
    foreach(UserInformation* pUser, m_qlUsers)
        SAFE_DELETE(pUser);
    m_qlUsers.clear();

    SAFE_DELETE(m_pDataModel);
}

QStandardItemModel *CSystemUsersInformation::GetUserInformations()
{
    return m_pDataModel;
}

int CSystemUsersInformation::Initialize(void)
{
    // CLEAR LIST
    foreach(UserInformation* pUser, m_qlUsers)
        SAFE_DELETE(pUser);
    m_qlUsers.clear();

    int nStatus = -1;
    UserInformation *pUserInf = 0;
    LPUSER_INFO_11 pBuffer = NULL;
    DWORD dwMaxLength = MAX_PREFERRED_LENGTH;
    DWORD dwEntriesRead = 0, dwTotalEntries = 0, dwResumeHandle = 0;
    QDateTime qdLastLogon;
    QDate qdDate;
    QTime qtTime;

    do
    {
        nStatus = NetUserEnum(NULL,
                              11,
                              FILTER_NORMAL_ACCOUNT,
                              (LPBYTE*)&pBuffer,
                              dwMaxLength,
                              &dwEntriesRead,
                              &dwTotalEntries,
                              &dwResumeHandle);

        if (ERROR_SUCCESS == nStatus || ERROR_MORE_DATA == nStatus)
        {
            if (NULL != pBuffer)
            {
                LPUSER_INFO_11 pTempBuffer = pBuffer;

                for (unsigned int i = 0; i < dwEntriesRead; i++)
                {
                    if (pTempBuffer == NULL)
                        break;

                    pUserInf = new UserInformation;

                    pUserInf->UserName = QString::fromWCharArray(pTempBuffer->usri11_name);
                    pUserInf->FullUserName = QString::fromWCharArray(pTempBuffer->usri11_full_name);

                    switch(pTempBuffer->usri11_priv)
                    {
                    case USER_PRIV_GUEST:
                        pUserInf->Privilege = QString("Guest");
                        break;
                    case USER_PRIV_USER:
                        pUserInf->Privilege = QString("User");
                        break;
                    case USER_PRIV_ADMIN:
                        pUserInf->Privilege = QString("Administrator");
                        break;
                    }

                    pUserInf->HomeDir = QString::fromWCharArray(pTempBuffer->usri11_home_dir);

                    qdDate.setDate(1970, 01, 01);
                    qtTime.setHMS(0,0,0,0);
                    qdLastLogon.setDate(qdDate);
                    qdLastLogon.setTime(qtTime);

                    QDateTime qdNewTime = qdLastLogon.addSecs(pTempBuffer->usri11_last_logon);

                    pUserInf->LastLogon = qdNewTime.toString();

                    qdDate.setDate(1970, 01, 01);
                    qtTime.setHMS(0,0,0,0);
                    qdLastLogon.setDate(qdDate);
                    qdLastLogon.setTime(qtTime);

                    qdNewTime = qdLastLogon.addSecs(pTempBuffer->usri11_last_logoff);

                    pUserInf->LastLogoff = qdNewTime.toString();

                    pUserInf->LogonServer = QString::fromWCharArray(pTempBuffer->usri11_logon_server);

                    pUserInf->CountryCode = QString::number(pTempBuffer->usri11_country_code);

                    m_qlUsers.append(pUserInf);

                    pTempBuffer++;
                }
            }
        }
    }while (nStatus == ERROR_MORE_DATA);
    return 0;
}
