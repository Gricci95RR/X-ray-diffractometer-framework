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

#include <string>
#include <memory>
#include <fstream>
#include <iterator>
#include <sstream>

#include "XRayMachineDevicesFactory.hpp"

#include <spdlog/spdlog.h>

int main() {
    std::shared_ptr<XRayMachineDevicesFactory> clientXRayMachineDevicesFactory =  // Factory
        std::make_shared<XRayMachineDevicesFactory>();
    std::shared_ptr<ISingleStepperDeviceController> client_XRaySource =  // XRaySource
        clientXRayMachineDevicesFactory->createXRaySourceDeviceController();
    std::shared_ptr<ICrystalDeviceController> client_Crystal =  // Crystal
        clientXRayMachineDevicesFactory->createCrystalDeviceController();
    std::shared_ptr<IMultiStepperDeviceController> client_Monochromator =  // Monochromator
        clientXRayMachineDevicesFactory->createMonochromatorDeviceController();
    std::shared_ptr<IMultiStepperDeviceController> client_Slit =  // Slit
        clientXRayMachineDevicesFactory->createSlitDeviceController();
    std::shared_ptr<ISingleStepperDeviceController> client_XraySensor =  // XraySensor
        clientXRayMachineDevicesFactory->createXRaySensorDeviceController();
    spdlog::set_level(spdlog::level::debug);  // Set global log level to debug
    // Start devices
    client_XRaySource->start();
    client_Crystal->start();
    client_Monochromator->start();
    client_Slit->start();
    client_XraySensor->start();
    client_XRaySource->getFsmState();

    // Setup Logger
    // client_Crystal->logger("test_compensate_realign_yaxis.txt");

    // Align Source
    client_Slit->alignSourceWithSensor();
    client_Monochromator->alignSourceWithSensor();
    client_Crystal->alignSourceWithSensor();
    client_XRaySource->alignSourceWithSensor(false);

    // Align monochromator and braggs peak
    // client_Monochromator->alignMonochromator(false);
    client_XRaySource->setupMonochromatorBraggPeakSearch();
    client_Monochromator->moveToPositionStepper1Linear(88.6);
    // client_Monochromator->searchMonochromatorBraggPeak();
    client_Monochromator->moveToPositionStepper2Rotational(7.77);

    // Align Slit
    // client_Monochromator->getFsmState();
    client_Slit->alignSlit(true);

    // Align X-Axis Crystal
    // client_Crystal->xAxisAlignmentCrystal();
    // client_Crystal->yWAxesAlignmentCrystal();
    // client_Crystal->xAxisFineAlignmentCrystal();
    // client_Crystal->braggPeakSearchCrystal();
    // client_Crystal->yAxisFineAlignmentCrystal();
    // client_Crystal->checkAlignmentInFlippedOrientation();
    // client_Crystal->bendingAngleMeasurement();
    //client_Crystal->miscutAngleMeasurement(true);
    // client_Crystal->getFsmState();

    // Disconnect
    client_XRaySource->disconnectStp();
    client_XRaySource->getFsmState();
    client_Crystal->disconnect();
    client_Monochromator->disconnectStp();
    client_Slit->disconnectStp();
    client_XraySensor->disconnectStp();
}
