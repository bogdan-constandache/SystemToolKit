#ifndef NETWORK_DEVICES_WIDGET_H
#define NETWORK_DEVICES_WIDGET_H

#include <QWidget>

#include "../abstract_controller.h"

namespace Ui {
class CNetworkDevicesWidget;
}

class CNetworkDevicesWidget : public QWidget
{
    Q_OBJECT

private:
    Ui::CNetworkDevicesWidget *ui;
    AbstractController *m_pController;

public:
    explicit CNetworkDevicesWidget(QWidget *parent = 0, AbstractController *pController = 0);
    ~CNetworkDevicesWidget();

private slots:
    void OnNetworkNamesItemClickedSlot(QModelIndex Index);
    void OnSetNetworkNamesTreeModel(QStandardItemModel *pModel);
    void OnSetNetworkDeviceInformationsTreeModel(QStandardItemModel *pModel);
    void OnAdaptersNamesDataChangedSlot();
    void OnAdaptersPropertiesDataChangedSlot();

signals:
    void OnShowWidget(QWidget*);
    void OnRequestNetworkDeviceInformationSignal(QString);
};

#endif // NETWORK_DEVICES_WIDGET_H
