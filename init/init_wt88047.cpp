/*
   Copyright (c) 2016, The CyanogenMod Project

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are
   met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.
    * Neither the name of The Linux Foundation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
   WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
   ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
   BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
   BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
   OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
   IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <cstdlib>
#include <fcntl.h>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>
#include <sys/sysinfo.h>

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/properties.h>
#include <android-base/strings.h>

#include "property_service.h"
#include "vendor_init.h"

using android::base::GetProperty;
using android::base::ReadFileToString;
using android::base::Trim;
using android::init::property_set;

void property_override(char const prop[], char const value[])
{
    prop_info *pi;

    pi = (prop_info*) __system_property_find(prop);
    if (pi)
        __system_property_update(pi, value, strlen(value));
    else
        __system_property_add(prop, strlen(prop), value, strlen(value));
}

void property_override_dual(char const system_prop[],
        char const vendor_prop[], char const value[])
{
    property_override(system_prop, value);
    property_override(vendor_prop, value);
}

int is2GB()
{
    struct sysinfo sys;
    sysinfo(&sys);
    return sys.totalram > 1024ull * 1024 * 1024;
}

void init_target_properties()
{
    std::ifstream fin;
    std::string buf;
    std::string product;

    fin.open("/proc/cmdline");
    while (std::getline(fin, buf, ' '))
        if (buf.find("board_id") != std::string::npos)
            break;
    fin.close();

    /* S88047E1 */
    if (buf.find("S88047E1") != std::string::npos) {
        property_override("ro.build.product", "HM2014817");
        property_override("ro.product.device", "HM2014817");
        property_override("ro.product.model", "2014817");
        property_override("ro.product.name", "2014817");
        property_override("ro.telephony.default_network", "9,1");
    }
    /* S88047D1 */
    else if (buf.find("S88047D1") != std::string::npos) {
        property_override("ro.build.product", "HM2014819");
        property_override("ro.product.device", "HM2014819");
        property_override("ro.product.model", "2014819");
        property_override("ro.product.name", "2014819");
        property_set("ro.telephony.default_network", "9,1");
    }
    /* S88047C1 */
    else if (buf.find("S88047C1") != std::string::npos) {
        property_override("ro.build.product", "HM2014818");
        property_override("ro.product.device", "HM2014818");
        property_override("ro.product.model", "2014818");
        property_override("ro.product.name", "2014818");
        property_set("ro.telephony.default_network", "9,1");
        property_set("persist.dbg.volte_avail_ovr", "1");
        property_set("persist.dbg.vt_avail_ovr", "1");
    }
    /* S88047B2 */
    else if (buf.find("S88047B2") != std::string::npos) {
        property_override("ro.build.product", "HM2014821");
        property_override("ro.product.device", "HM2014821");
        property_override("ro.product.model", "2014821");
        property_override("ro.product.name", "2014821");
        property_set("ro.telephony.default_network", "22,1");
        property_set("telephony.lteOnCdmaDevice", "1");
        property_set("persist.radio.sglte.eons_domain", "ps");
    }
    /* S88047B1 */
    else if (buf.find("S88047B1") != std::string::npos) {
        property_override("ro.build.product", "HM2014812");
        property_override("ro.product.device", "HM2014812");
        property_override("ro.product.model", "2014812");
        property_override("ro.product.name", "2014812");
        property_set("ro.telephony.default_network", "22,1");
        property_set("telephony.lteOnCdmaDevice", "1");
        property_set("persist.radio.sglte.eons_domain", "ps");
    }
    /* S86047A1 and S86047A1_CD */
    else if (buf.find("S86047A1") != std::string::npos) {
        property_override("ro.build.product", "HM2014813");
        property_override("ro.product.device", "HM2014813");
        property_override("ro.product.model", "2014813");
        property_override("ro.product.name", "2014813");
        property_set("ro.telephony.default_network", "9,1");
    }
    /* S86047A2 and S86047A2_CD */
    else if (buf.find("S86047A2") != std::string::npos) {
        property_override("ro.build.product", "HM2014112");
        property_override("ro.product.device", "HM2014112");
        property_override("ro.product.model", "2014112");
        property_override("ro.product.name", "2014112");
        property_set("ro.telephony.default_network", "9,1");
    }
    /* S88047A2 and S88047A1 */
    else {
        property_override("ro.build.product", "HM2014811");
        property_override("ro.product.device", "HM2014811");
        property_override("ro.product.model", "2014811");
        property_override("ro.product.name", "2014811");
        property_set("ro.telephony.default_network", "9,1");
    }

    property_set("dalvik.vm.heapstartsize", "8m");
    property_set("dalvik.vm.heapgrowthlimit", is2GB() ? "192m" : "96m");
    property_set("dalvik.vm.heapsize", is2GB() ? "512m" : "256m");
    property_set("dalvik.vm.heaptargetutilization", "0.75");
    property_set("dalvik.vm.heapminfree", "512k");
    property_set("dalvik.vm.heapmaxfree", "8m");

    property_override_dual("ro.product.device", "ro.vendor.product.device", "wt88047");
    property_override_dual("ro.product.name", "ro.vendor.product.name", "wt88047");
    property_override_dual("ro.build.fingerprint", "ro.vendor.build.fingerprint", "Xiaomi/wt88047/wt88047:5.1.1/LMY47V/V9.2.5.0.LHJMIEK:user/release-keys");
}

static void init_alarm_boot_properties()
{
    char const *boot_reason_file = "/proc/sys/kernel/boot_reason";
    std::string boot_reason;
    std::string tmp = GetProperty("ro.boot.alarmboot","");

    if (ReadFileToString(boot_reason_file, &boot_reason)) {
        /*
         * Setup ro.alarm_boot value to true when it is RTC triggered boot up
         * For existing PMIC chips, the following mapping applies
         * for the value of boot_reason:
         *
         * 0 -> unknown
         * 1 -> hard reset
         * 2 -> sudden momentary power loss (SMPL)
         * 3 -> real time clock (RTC)
         * 4 -> DC charger inserted
         * 5 -> USB charger insertd
         * 6 -> PON1 pin toggled (for secondary PMICs)
         * 7 -> CBLPWR_N pin toggled (for external power supply)
         * 8 -> KPDPWR_N pin toggled (power key pressed)
         */
        if (Trim(boot_reason) == "3" || tmp == "true")
            property_set("ro.alarm_boot", "true");
        else
            property_set("ro.alarm_boot", "false");
    }
}

void vendor_load_properties()
{
    // Init a dummy BT MAC address, will be overwritten later
    property_set("ro.boot.btmacaddr", "00:00:00:00:00:00");
    init_target_properties();
    init_alarm_boot_properties();
}
