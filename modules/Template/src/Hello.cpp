/* © Copyright CERN 2021.  All rights reserved. This software is released under a CERN proprietary
 * software licence. Any permission to use it shall be granted in writing. Requests shall be
 * addressed to CERN through mail-KT@cern.ch
 *
 * Author: Yulia Venichenko CERN BE/CEM/MRO 2021
 *         Alejandro Diaz Rosales CERN BE/CEM/MRO 2021
 *
 *  ===============================================================================================
 */

#include "Template/Hello.hpp"
#include <hxp_drivers.h>
#include <ximc.h>
#include <string>

std::string generateHelloString(const std::string & personName) {
    std::string xmicversion = createXMICVersion();
    std::string libversion = createHXPVersion();
    fprintf(stderr, "Hello, Test in error pipeline!\n");
    std::string result = "Hello "
                        + personName + ". "
                        + libversion
                        + ". Using XIMC version: " + xmicversion;
    return result;
}

std::string createXMICVersion() {
    std::string result;
    char version_value[32];
    ximc_version(version_value);
    result = version_value;
    return result;
}

std::string createHXPVersion() {
    std::string result = GetLibraryVersion();
    return result;
}
