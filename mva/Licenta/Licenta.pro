QT       += core gui sql
QT       += widgets
QT       += winextras

include(others/project-configuration/project-defines.pri)

TARGET = Licenta
TEMPLATE = app

INCLUDEPATH += $$PWD/proto-buffers/includes

QMAKE_CFLAGS_RELEASE += -Zi

QMAKE_LFLAGS_RELEASE += /DEBUG

QMAKE_LFLAGS += /MANIFESTUAC:\"level=\'requireAdministrator\' uiAccess=\'false\'\"

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
LIBS += -lNetapi32
LIBS += -lShell32
LIBS += -lPsapi

contains(QMAKE_TARGET.arch, x86_64) {
LIBS += -l$$PWD/modules/motherboard/video-card/api/nvidia-api/amd64/nvapi64
} else {
LIBS += -l$$PWD/modules/motherboard/video-card/api/nvidia-api/x86/nvapi
}

DEFINES += NOMINMAX
DEFINES += STK_WINDOWS

Release:DESTDIR = $$PWD/../build
Release:OBJECTS_DIR = $$PWD/../build/.obj
Release:MOC_DIR = $$PWD/../build/.moc
Release:RCC_DIR = $$PWD/../build/.rcc
Release:UI_DIR = $$PWD/../build/.ui

Debug:DESTDIR = $$PWD/../build
Debug:OBJECTS_DIR = $$PWD/../build/.obj
Debug:MOC_DIR = $$PWD/../build/.moc
Debug:RCC_DIR = $$PWD/../build/.rcc
Debug:UI_DIR = $$PWD/../build/.ui


SOURCES += main/main.cpp \
        utils/sources/utils.cpp \
        gui/abstract_controller.cpp \
        gui/view_adapter.cpp \
        controller/controller.cpp \
        controller/controller_callbacks.cpp \
        proto-buffers/sensors_data.pb.cc \
        proto-buffers/configuration_data.pb.cc \
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
        modules/computer/sensors/sources/intel_temperature.cpp \
        modules/computer/sensors/sources/factory_board_sensor.cpp \
        modules/computer/sensors/sources/factory_cpu_sensor.cpp \
        modules/computer/sensors/sources/w836xx.cpp \
        modules/operating-system/processes/sources/processes.cpp \
        modules/software/startup-applications/sources/startup_application.cpp \
        modules/computer/sensors/sources/f718xx.cpp \
        modules/computer/sensors/sources/amd0f_temperature.cpp \
        modules/computer/sensors/sources/amd10_temperature.cpp \
        modules/computer/device_manager/device_manager.cpp \
        modules/motherboard/video-card/nvidia_card.cpp \
        modules/motherboard/memory/memory_data.cpp \
        modules/operating-system/system-users/system_users_information.cpp \
        modules/software/applications-manager/sources/process_wrapper.cpp \
        modules/computer/dmi/sources/type3_enclosure_information.cpp \
        modules/computer/dmi/sources/type7_cache_information.cpp \
        modules/motherboard/memory/spd.cpp \
        modules/operating-system/summary/sources/operating_system.cpp \
        gui/sources/main_window.cpp \
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
        gui/sources/about_dialog.cpp \
        gui/sources/startup_apps_widget.cpp \
        gui/sources/device_manager_widget.cpp \
        gui/sources/user_information_widget.cpp \
        gui/sources/spd_widget.cpp \
        gui/sources/video_card_widget.cpp \
        gui/sources/operating_system_widget.cpp \
        modules/motherboard/cpu/sources/cpuid_manager.cpp \
        modules/motherboard/cpu/sources/amd_cpuid.cpp \
        gui/sources/waiting_widget.cpp \
    modules/motherboard/video-card/amd_card.cpp \
    modules/motherboard/video-card/coverdrive5.cpp \
    modules/motherboard/video-card/coverdrive6.cpp

HEADERS  += gui/abstract_controller.h \
        gui/view_adapter.h \
        controller/controller.h \
        gui/headers/main_window.h \
        controller/enums.h \
        controller/controller_callbacks.h \
        proto-buffers/sensors_data.pb.h \
        proto-buffers/configuration_data.pb.h \
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
        modules/computer/sensors/headers/intel_temperature.h \
        modules/computer/sensors/headers/factory_board_sensor.h \
        modules/computer/sensors/headers/factory_cpu_sensor.h \
        modules/computer/sensors/headers/w836xx.h \
        modules/api.h \
        modules/operating-system/processes/headers/processes.h \
        modules/operating-system/processes/headers/processes_structs.h \
        modules/software/startup-applications/headers/startup_application.h \
        modules/computer/sensors/headers/f718xx.h \
        modules/computer/sensors/headers/amd0f_temperature.h \
        modules/computer/sensors/headers/amd10_temperature.h \
        modules/computer/device_manager/device_manager.h \
        modules/motherboard/video-card/video_structures.h \
        modules/motherboard/video-card/nvidia_card.h \
        modules/motherboard/memory/memory_data.h \
        modules/operating-system/system-users/system_users_information.h \
        modules/operating-system/system-users/system_users_information_structs.h \
        modules/software/applications-manager/headers/process_wrapper.h \
        modules/computer/dmi/headers/type3_enclosure_information.h \
        modules/computer/dmi/headers/type7_cache_information.h \
        modules/motherboard/memory/spd.h \
        modules/motherboard/memory/spd_defines.h \
        modules/operating-system/summary/headers/operating_system.h \
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
        gui/headers/about_dialog.h \
        gui/headers/startup_apps_widget.h \
        gui/headers/device_manager_widget.h \
        gui/headers/user_information_widget.h \
        gui/headers/spd_widget.h \
        gui/headers/video_card_widget.h \
        gui/headers/operating_system_widget.h \
        modules/motherboard/cpu/headers/cpuid_manager.h \
        modules/motherboard/cpu/headers/amd_cpuid.h \
        gui/headers/waiting_widget.h \
    modules/motherboard/video-card/amd_card.h \
    modules/motherboard/video-card/coverdrive5.h \
    modules/motherboard/video-card/coverdrive6.h \
    modules/motherboard/video-card/ioverdrive.h

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
        gui/forms/startup_apps_widget.ui \
        gui/forms/device_manager_widget.ui \
        gui/forms/about_dialog.ui \
        gui/forms/user_information_widget.ui \
        gui/forms/spd_widget.ui \
        gui/forms/video_card_widget.ui \
        gui/forms/operating_system_widget.ui \
        gui/forms/waiting_widget.ui

OTHER_FILES += \
        resources/qss/main_dialog.qss \
        resources/qss/about_dialog.qss

RESOURCES += \
        resources/resources.qrc

QMAKE_POST_LINK += rcc $$PWD/resources/resources.qrc -o $$PWD/resources.rcc

RC_FILE += res.rc

configurationFile.path = $$PWD/../build
configurationFile.files += $$PWD/others/configuration.cfg

INSTALLS += configurationFile

dbFile.path = $$PWD/../build/config/databases
dbFile.files += $$PWD/others/databases/config.db

INSTALLS += dbFile

vistaDrivers.path = $$PWD/../build/config/drivers/vista
vistaDrivers.files += $$PWD/others/driver/vista/stk_driver.sys
vistaDrivers.files += $$PWD/others/driver/vista/stk_driver64.sys

INSTALLS += vistaDrivers

win7Driver.path = $$PWD/../build/config/drivers/win7
win7Driver.files += $$PWD/others/driver/win7/stk_driver.sys
win7Driver.files += $$PWD/others/driver/win7/stk_driver64.sys

INSTALLS += win7Driver

win8Driver.path = $$PWD/../build/config/drivers/win8
win8Driver.files += $$PWD/others/driver/win8/stk_driver.sys
win8Driver.files += $$PWD/others/driver/win8/stk_driver64.sys

INSTALLS += win8Driver

resourceFile.path = $$PWD/../build
resourceFile.files += $$PWD/resources.rcc

INSTALLS += resourceFile
