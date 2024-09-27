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

#include "XRayMachineDevicesFactory.hpp"

#include <spdlog/spdlog.h>

int main() {
    std::shared_ptr<XRayMachineDevicesFactory> clientXRayMachineDevicesFactory =  // Factory
        std::make_shared<XRayMachineDevicesFactory>();

    std::shared_ptr<ICrystalDeviceController> clientCrystalDeviceController =  // creation of obj clientCrystalDeviceController (Autocollimator)
        clientXRayMachineDevicesFactory->createCrystalDeviceController();
    /* FSM Crystal Start*/
    spdlog::set_level(spdlog::level::debug);  // Set global log level to debug
    clientCrystalDeviceController->start();
    clientCrystalDeviceController->getFsmState();
    clientCrystalDeviceController->goHome();
    clientCrystalDeviceController->getFsmState();
    /*
    std::thread t1;
    t1 = std::thread([=](){
        return  clientCrystalDeviceController->scanHxp(clientCrystalDeviceController->hxpAxis::X, 0.5, 2, 100, "scan1.csv", false);
    });
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    clientCrystalDeviceController->stop();
    clientCrystalDeviceController->getFsmState();  // print fsm state
    t1.join();
    
    clientCrystalDeviceController->scanHxp(clientCrystalDeviceController->hxpAxis::Z, 0.5, 2, 100, "scan1.csv", true, false);
    clientCrystalDeviceController->getFsmState();
    clientCrystalDeviceController->scanHxp(clientCrystalDeviceController->hxpAxis::X, 0.5, 2, 100, "scan1.csv", false, false);
    clientCrystalDeviceController->getFsmState();
    */
    clientCrystalDeviceController->disconnect();
    clientCrystalDeviceController->getFsmState();
    /* FSM Crystal End */
}
