#ifndef API_H
#define API_H

#include "computer/device_manager/device_manager.h"
#include "computer/dmi/headers/smbios_entry_point.h"
#include "computer/power-management/headers/battery_status.h"
#include "computer/sensors/headers/sensor_module.h"

#include "motherboard/cpu/headers/intel_cpuid.h"
#include "motherboard/memory/memory_data.h"
#include "motherboard/memory/spd.h"
#include "motherboard/video-card/nvidia_card.h"

#include "network/active-connections/active_connections.h"
#include "network/network-devices/headers/network_devices.h"

#include "operating-system/summary/headers/operating_system.h"
#include "operating-system/processes/headers/processes.h"
#include "operating-system/system-drivers/headers/system_drivers.h"
#include "operating-system/system-users/system_users_information.h"

#include "software/applications-manager/headers/application_manager.h"
#include "software/applications-manager/headers/process_wrapper.h"
#include "software/startup-applications/headers/startup_application.h"

#include "storage/smart/headers/csmartinfo.h"

#endif
