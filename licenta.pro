#=====================================================================================
include ($$PWD/others/project-configuration/project-defines.pri)
#=====================================================================================

#=====================================================================================
#SHADOWBUILD DIRS
DESTDIR = $$PWD/build/$$CROSSPLATFORM/Licenta
#=====================================================================================

#=====================================================================================
QT       += core
QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
#=====================================================================================

#=====================================================================================
TARGET = Licenta
#CONFIG   += console
#CONFIG   -= app_bundle
#=====================================================================================

#=====================================================================================
#DEFINES
#=====================================================================================

#=====================================================================================
LIBS += -L"$$PWD/libs/$$CROSSPLATFORM/" -lsqlite
LIBS += -lAdvapi32
LIBS += -lSetupapi
LIBS += -lOle32
LIBS += -loleaut32
LIBS += -lVersion
#=====================================================================================

#=====================================================================================
TEMPLATE = app
#=====================================================================================

#=====================================================================================
INCLUDEPATH += $$PWD
#=====================================================================================

#=====================================================================================
SOURCES += \
#=====================================================================================
    utils/sources/utils.cpp \
    modules/computer/dmi/sources/type17_memory_device_information.cpp \
    modules/computer/dmi/sources/type4_processor_information.cpp \
    modules/computer/dmi/sources/type2_baseboard_information.cpp \
    modules/computer/dmi/sources/type0_bios_information.cpp \
    modules/computer/dmi/sources/smbios_entry_point.cpp \
    modules/operating-system/processes/sources/processes.cpp \
#    modules/operating-system/summary/sources/operating_system.cpp \
    modules/operating-system/system-drivers/sources/system-drivers.cpp \
    modules/storage/smart/sources/csmartinfo.cpp \
    modules/storage/smart/sources/identify.cpp \
    modules/storage/common/sources/callbacks.cpp \
#    modules/software/applications-manager/sources/application_manager.cpp \
#    modules/software/startup-applications/sources/startup_application.cpp \
    modules/computer/power-management/sources/battery_status.cpp \
    main/main.cpp \
    controller/sources/controller.cpp \
    gui/sources/main_window.cpp

#=====================================================================================
HEADERS += \
#=====================================================================================
    interfaces/module.h \
    interfaces/imain_window.h \
    utils/headers/utils.h \
    modules/computer/dmi/headers/type17_memory_device_information.h \
    modules/computer/dmi/headers/type4_processor_information.h \
    modules/computer/dmi/headers/type2_baseboard_information.h \
    modules/computer/dmi/headers/type0_bios_information.h \
    modules/computer/dmi/headers/smbios_structures.h \
    modules/computer/dmi/headers/smbios_internal_structures.h \
    modules/computer/dmi/headers/smbios_entry_point.h \
    modules/computer/dmi/headers/smbios_api_classes.h \
    modules/computer/dmi/interfaces/smbios_generic_structure.h \
    modules/operating-system/processes/headers/processes_structs.h \
    modules/operating-system/processes/headers/processes.h \
#    modules/operating-system/summary/headers/operating_system.h \
    modules/operating-system/system-drivers/headers/system_drivers.h \
    modules/storage/smart/headers/smart_structs.h \
    modules/storage/smart/headers/smart_defines.h \
    modules/storage/smart/headers/csmartinfo.h \
    modules/storage/smart/headers/identify.h \
    modules/storage/common/headers/callbacks.h \
#    modules/software/applications-manager/headers/software_structures.h \
#    modules/software/applications-manager/headers/application_manager.h \
#    modules/software/startup-applications/headers/startup_application.h \
    modules/computer/power-management/headers/battery_status.h \
    controller/headers/controller.h \
    gui/headers/main_window.h \
    modules/api.h

#=====================================================================================
#Additional files to copy
copyFiles( $$PWD/others/databases/ $$DESTDIR/config/databases )
copyFiles( $$PWD/others/vcredist/$$CROSSPLATFORM/ $$DESTDIR )
#=====================================================================================

FORMS += \
    gui/forms/mainwindow.ui
