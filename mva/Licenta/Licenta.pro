QT       += core gui sql
QT       += widgets

TARGET = Licenta
TEMPLATE = app

LIBS += -lAdvapi32
LIBS += -lSetupapi
LIBS += -lOle32
LIBS += -loleaut32
LIBS += -lVersion
LIBS += -lWbemuuid
LIBS += -liphlpapi
LIBS += -lws2_32


SOURCES += main/main.cpp \
        utils/sources/utils.cpp \
        gui/abstract_controller.cpp \
        gui/view_adapter.cpp \
        controller/controller.cpp \
        gui/sources/main_window.cpp \
        modules/computer/power-management/sources/battery_status.cpp \
        modules/software/applications-manager/sources/application_manager.cpp \
        modules/computer/dmi/sources/type17_memory_device_information.cpp \
        modules/computer/dmi/sources/type4_processor_information.cpp \
        modules/computer/dmi/sources/type2_baseboard_information.cpp \
        modules/computer/dmi/sources/type0_bios_information.cpp \
        modules/computer/dmi/sources/smbios_entry_point.cpp \
        modules/storage/common/sources/callbacks.cpp \
        modules/storage/smart/sources/csmartinfo.cpp \
        modules/operating-system/system-drivers/sources/system-drivers.cpp \
        modules/network/active-connections/active_connections.cpp \
        gui/sources/battery_status_widget.cpp \
        gui/sources/application_manager_widget.cpp \
        gui/sources/dmi_widget.cpp \
        gui/sources/smart_widget.cpp \
        gui/sources/ata_widget.cpp \
        gui/sources/system_drivers_widget.cpp \
        gui/sources/active_connections_widget.cpp \
    modules/computer/sensors/sources/stk_driver_wrapper.cpp \
    modules/computer/sensors/sources/it87.cpp

HEADERS  += gui/abstract_controller.h \
        gui/view_adapter.h \
        controller/controller.h \
        gui/headers/main_window.h \
        controller/enums.h \
        modules/computer/power-management/headers/battery_status.h \
        modules/software/applications-manager/headers/software_structures.h \
        modules/software/applications-manager/headers/application_manager.h \
        modules/computer/dmi/headers/type17_memory_device_information.h \
        modules/computer/dmi/headers/type4_processor_information.h \
        modules/computer/dmi/headers/type2_baseboard_information.h \
        modules/computer/dmi/headers/type0_bios_information.h \
        modules/computer/dmi/headers/smbios_structures.h \
        modules/computer/dmi/headers/smbios_internal_structures.h \
        modules/computer/dmi/headers/smbios_entry_point.h \
        modules/computer/dmi/headers/smbios_api_classes.h \
        modules/computer/dmi/interfaces/smbios_generic_structure.h \
        modules/storage/smart/headers/smart_structs.h \
        modules/storage/smart/headers/smart_defines.h \
        modules/storage/smart/headers/csmartinfo.h \
        modules/storage/common/headers/callbacks.h \
        modules/operating-system/system-drivers/headers/system_drivers.h \
        modules/network/active-connections/active_connections.h \
        modules/network/active-connections/network_structures.h \
        gui/headers/battery_status_widget.h \
        gui/headers/application_manager_widget.h \
        gui/headers/dmi_widget.h \
        gui/headers/smart_widget.h \
        gui/headers/ata_widget.h \
        gui/headers/system_drivers_widget.h \
        gui/headers/active_connections_widget.h \
    modules/computer/sensors/driver/driver_defines.h \
    modules/computer/sensors/headers/stk_driver_wrapper.h \
    modules/computer/sensors/interfaces/isensor.h \
    modules/computer/sensors/headers/it87.h \
    modules/computer/sensors/headers/sensor_defines.h

FORMS    += \
        gui/forms/mainwindow.ui \
        gui/forms/battery_status_widget.ui \
        gui/forms/application_manager_widget.ui \
        gui/forms/dmi_widget.ui \
        gui/forms/smart_widget.ui \
        gui/forms/ata_widget.ui \
        gui/forms/system_drivers_widget.ui \
        gui/forms/active_connections_widget.ui
