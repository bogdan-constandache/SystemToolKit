#include "controller.h"

Controller::Controller(): m_pBatteryStatus(NULL), m_pApplicationManager(NULL),
    m_pDMIManager(NULL), m_pSmartManager(NULL)
{
}

Controller::~Controller()
{
    if (m_pBatteryStatus)
        delete m_pBatteryStatus;
    if (m_pApplicationManager)
        delete m_pApplicationManager;
    if (m_pDMIManager)
        delete m_pDMIManager;
    if (m_pSmartManager)
        delete m_pSmartManager;
}

void Controller::StartController()
{
    // Create main window
    emit OnCreateMainWindowSignal();

    // Populate left menu tree
    QStandardItemModel *pModel = new QStandardItemModel();
    QStandardItem *pDataRoot = pModel->invisibleRootItem();
    QStandardItem *pDataItem = 0;
    QStandardItem *pChildItem = 0;
    QList<QStandardItem*> qList;
    QList<QStandardItem*> qChildList;

    pDataItem = new QStandardItem(QString("Computer"));
    qList.append(pDataItem);

    pChildItem = new QStandardItem(QString("DMI"));
    qChildList.append(pChildItem);
    pChildItem = new QStandardItem(QString("Power management"));
    qChildList.append(pChildItem);

    pDataItem->appendRows(qChildList);
    qChildList.clear();

    pDataItem = new QStandardItem(QString("Operating system"));
    qList.append(pDataItem);

    pChildItem = new QStandardItem(QString("Operating system"));
    qChildList.append(pChildItem);
    pChildItem = new QStandardItem(QString("Processes"));
    qChildList.append(pChildItem);
    pChildItem = new QStandardItem(QString("System drivers"));
    qChildList.append(pChildItem);

    pDataItem->appendRows(qChildList);
    qChildList.clear();

    pDataItem = new QStandardItem(QString("Storage"));
    qList.append(pDataItem);

    pChildItem = new QStandardItem(QString("SMART"));
    qChildList.append(pChildItem);

    pDataItem->appendRows(qChildList);
    qChildList.clear();

    pDataItem = new QStandardItem(QString("Software"));
    qList.append(pDataItem);

    pChildItem = new QStandardItem(QString("Startup applications"));
    qChildList.append(pChildItem);
    pChildItem = new QStandardItem(QString("Application manager"));
    qChildList.append(pChildItem);

    pDataItem->appendRows(qChildList);
    qChildList.clear();

    pDataRoot->appendRows(qList);
    qList.clear();

    emit OnPopulateMenuTreeSignal(pModel);

    emit OnShowMainWindowSignal();
}

void Controller::OnComputerDMIOptClickedSlot()
{
    m_pDMIManager = new CSMBiosEntryPoint();
    QStandardItemModel *pModel = m_pDMIManager->GetItemsModel();

    if (0 != pModel)
        emit OnSetDMIItemsInformation(pModel);
}

void Controller::OnComputerPowerManagementOptClickedSlot()
{
    m_pBatteryStatus = new BatteryStatus();
    QStandardItemModel *pModel = m_pBatteryStatus->GetBatteryInformation();

    if (0 != pModel)
        emit OnSetPowerManagementInformation(pModel);
}

void Controller::OnHddInformationOptClickedSlot()
{
    qDebug() << __FUNCTION__;
}

void Controller::OnOperatingSystemOptClickedSlot()
{
    qDebug() << __FUNCTION__;
}

void Controller::OnProcessesOptClickedSlot()
{
    qDebug() << __FUNCTION__;
}

void Controller::OnSystemDriversOptClickedSlot()
{
    qDebug() << __FUNCTION__;
}

void Controller::OnStorageSmartOptClickedSlot()
{
    m_pSmartManager = new CSmartInfo();

    QStandardItemModel *pModel = m_pSmartManager->GetAvailableHDD();

    if (0 != pModel)
        emit OnSetSMARTHDDItemsInformation(pModel);
}

void Controller::OnSmbiosOptClickedSlot()
{
    qDebug() << __FUNCTION__;
}

void Controller::OnApplicationManagerOptClickedSlot()
{
    m_pApplicationManager = new CApplicationManager();

    QStandardItemModel *pModel = m_pApplicationManager->GetApplicationList();

    if (0 != pModel)
        emit OnSetApplicationManagerInformation(pModel);
}

void Controller::OnStartupApplicationsOptClickedSlot()
{
    qDebug() << __FUNCTION__;
}

void Controller::OnRequestDMIItemProperties(DMIModuleType ItemType)
{
    QStandardItemModel *pModel = m_pDMIManager->GetItemPropertiesModel(ItemType);

    if (0 != pModel)
        emit OnSetDMIPropertiesInfomation(pModel);
}

void Controller::OnRequestSMARTProperties(QString qzModel)
{
    QStandardItemModel *pModel = m_pSmartManager->GetSMARTPropertiesForHDD(qzModel);

    if (0 != pModel)
        emit OnSetSMARTItemPropertiesInformation(pModel);
}

void Controller::OnUninstallApplicationSlot()
{
    qDebug() << __FUNCTION__;
}
