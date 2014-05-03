QT       += core gui sql
QT       += widgets

include(others/project-configuration/project-defines.pri)

TARGET = Licenta
TEMPLATE = app

INCLUDEPATH += $$PWD/proto-buffers/includes

LIBS += $$PWD/proto-buffers/lib/$$CROSSPLATFORM/libprotobuf-lite.lib

LIBS += -lAdvapi32
LIBS += -lSetupapi
LIBS += -lOle32
LIBS += -loleaut32
LIBS += -lVersion
LIBS += -lWbemuuid
LIBS += -liphlpapi
LIBS += -lws2_32
LIBS += -lPdh


DEFINES += STK_WINDOWS


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
        modules/computer/sensors/sources/stk_driver_wrapper.cpp \
        modules/computer/sensors/sources/it87.cpp \
        modules/network/network-devices/sources/network_devices.cpp \
        modules/motherboard/cpu/sources/intel_cpuid.cpp \
        modules/computer/sensors/sources/sensor_module.cpp \
        modules/computer/sensors/sources/hardware_usage.cpp \
        gui/sources/battery_status_widget.cpp \
        gui/sources/application_manager_widget.cpp \
        gui/sources/dmi_widget.cpp \
        gui/sources/smart_widget.cpp \
        gui/sources/ata_widget.cpp \
        gui/sources/system_drivers_widget.cpp \
        gui/sources/active_connections_widget.cpp \
        gui/sources/network_devices_widget.cpp \
        gui/sources/cpuid_widget.cpp \
        gui/sources/sensors_widget.cpp \
        gui/sources/processes_widget.cpp \
        proto-buffers/sensors_data.pb.cc \
        modules/computer/sensors/sources/intel_temperature.cpp \
        modules/computer/sensors/sources/factory_board_sensor.cpp \
        modules/computer/sensors/sources/factory_cpu_sensor.cpp \
        modules/computer/sensors/sources/w836xx.cpp \
        modules/operating-system/processes/sources/processes.cpp \
        modules/software/startup-applications/sources/startup_application.cpp \
        gui/sources/startup_apps_widget.cpp \
    modules/computer/sensors/sources/f718xx.cpp \
    modules/computer/sensors/sources/amd0f_temperature.cpp \
    modules/computer/sensors/sources/amd10_temperature.cpp

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
        modules/computer/sensors/driver/driver_defines.h \
        modules/computer/sensors/headers/stk_driver_wrapper.h \
        modules/computer/sensors/interfaces/isensor.h \
        modules/computer/sensors/headers/it87.h \
        modules/computer/sensors/headers/sensor_defines.h \
        modules/network/network-devices/headers/network_devices.h \
        modules/motherboard/cpu/headers/cpuid_defines.h \
        modules/motherboard/cpu/headers/intel_cpuid.h \
        modules/computer/sensors/headers/sensor_module.h \
        modules/computer/sensors/headers/hardware_usage.h \
        gui/headers/battery_status_widget.h \
        gui/headers/application_manager_widget.h \
        gui/headers/dmi_widget.h \
        gui/headers/smart_widget.h \
        gui/headers/ata_widget.h \
        gui/headers/system_drivers_widget.h \
        gui/headers/active_connections_widget.h \
        gui/headers/network_devices_widget.h \
        gui/headers/cpuid_widget.h \
        gui/headers/sensors_widget.h \
        gui/headers/processes_widget.h \
        proto-buffers/sensors_data.pb.h \
        modules/computer/sensors/headers/intel_temperature.h \
        modules/computer/sensors/headers/factory_board_sensor.h \
        modules/computer/sensors/headers/factory_cpu_sensor.h \
        modules/computer/sensors/headers/w836xx.h \
        modules/api.h \
        modules/operating-system/processes/headers/processes.h \
        modules/operating-system/processes/headers/processes_structs.h \
        modules/software/startup-applications/headers/startup_application.h \
        gui/headers/startup_apps_widget.h \
    modules/computer/sensors/headers/f718xx.h \
    modules/computer/sensors/headers/amd0f_temperature.h \
    modules/computer/sensors/headers/amd10_temperature.h

FORMS    += \
        gui/forms/mainwindow.ui \
        gui/forms/battery_status_widget.ui \
        gui/forms/application_manager_widget.ui \
        gui/forms/dmi_widget.ui \
        gui/forms/smart_widget.ui \
        gui/forms/ata_widget.ui \
        gui/forms/system_drivers_widget.ui \
        gui/forms/active_connections_widget.ui \
        gui/forms/network_devices_widget.ui \
        gui/forms/cpuid_widget.ui \
        gui/forms/sensors_widget.ui \
        gui/forms/processes_widget.ui \
        gui/forms/startup_apps_widget.ui
