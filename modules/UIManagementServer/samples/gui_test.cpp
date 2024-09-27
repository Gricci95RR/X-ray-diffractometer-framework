/* © Copyright CERN 2022.  All rights reserved. This software is released under a CERN proprietary
 * software licence. Any permission to use it shall be granted in writing. Requests shall be
 * addressed to CERN through mail-KT@cern.ch
 *
 * Author: Gianmarco Ricci CERN BE/CEM/MRO 2022
 *         
 *  ===============================================================================================
 */

#pragma once

#include <chrono>
#include <iostream>
#include <thread>

#include "UIManagementServer.hpp"
#include "XRayMachineDevicesFactory.hpp"

#include <spdlog/spdlog.h>

int main() {
    spdlog::set_level(spdlog::level::info);  // Set global log level to debug
    spdlog::debug("UI MNGMNT SERVER SAMPLE\n");

    
    std::shared_ptr<IDevicesFactory> clientXRayMachineDevicesFactory =  // Factory
        std::make_shared<XRayMachineDevicesFactory>();
    std::shared_ptr<IUIManagementServer> clientUIManagementServer =
        std::make_shared<UIManagementServer>(1337, clientXRayMachineDevicesFactory);

    clientUIManagementServer->initServer();

}
