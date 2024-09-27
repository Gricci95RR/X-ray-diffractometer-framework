/**
 * @file XRayMachineDevicesFactory.cpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2022
 * @brief This file defines the Class DevicesFactory.
 * @version 0.1
 * @date 2022
 * 
 * @copyright © Copyright CERN 2018. All rights reserved. This software is released under a CERN proprietary software license.
 * Any permission to use it shall be granted in writing. Requests shall be addressed to CERN through mail-KT@cern.ch
 * 
 */

#include "XRayMachineDevicesFactory.hpp"

XRayMachineDevicesFactory::XRayMachineDevicesFactory() {
    spdlog::info("cTor XRayMachineDevicesFactory\n");
    currentPath_ = std::filesystem::current_path();
    projectName_ = "XRay_Machine";
    clientConfiguration_ = std::make_shared<Configuration>();
    clientPostProcessing_ = std::make_shared<PostProcessing>();
    std::filesystem::path pathToProjDirectory = clientConfiguration_->getPartialPathUntilKeyName(currentPath_, projectName_);
    clientSensors_ = std::make_shared<Sensors>(pathToProjDirectory);
}

XRayMachineDevicesFactory::~XRayMachineDevicesFactory() {
    spdlog::info("dTor XRayMachineDevicesFactory\n");
}

std::shared_ptr<ICrystalDeviceController> XRayMachineDevicesFactory::createCrystalDeviceController() {
    spdlog::info("Method createCrystalDeviceController of Class  XRayMachineDevicesFactory\n");
    std::shared_ptr<IHXP> clientHXP =
        std::make_shared<HXP>(clientConfiguration_->getDefaultIpAddressHxp(), clientConfiguration_->getDefaultNPortHxp());
    std::shared_ptr<IMotor> clientStepper =
        std::make_shared<XIMC>(clientConfiguration_->getDefaultIpAddressStepper(), clientConfiguration_->readStepperMotorIndexAxis(3), clientConfiguration_->readStepperMotorCONST(3));  // Motor rotational crystal axis = 3
    // Scanning
    std::shared_ptr<IScanning> clientScanningHXP =
        std::make_shared<ScanningHXP>(clientHXP, clientSensors_, clientPostProcessing_);
    std::shared_ptr<IScanning> clientScanningStepper =
        std::make_shared<ScanningStepper>(clientStepper, clientSensors_, clientPostProcessing_);
    // Logger
    std::filesystem::path logFileDirectoryName = "LogFiles";
    std::filesystem::path logFileName = "crystalDeviceController_logFile.txt";
    std::filesystem::path logFilePath = clientConfiguration_->getPartialPathUntilKeyName(currentPath_, projectName_) / logFileDirectoryName / logFileName;
    std::shared_ptr<ICrystalDeviceController> clientCryDeviceController =
        std::make_shared<CrystalDeviceController>(logFilePath,
                                                  clientHXP,
                                                  clientStepper,
                                                  clientScanningHXP,
                                                  clientScanningStepper,
                                                  clientSensors_,
                                                  clientConfiguration_,
                                                  clientPostProcessing_);
    return clientCryDeviceController;
}

std::shared_ptr<ISingleStepperDeviceController> XRayMachineDevicesFactory::createAutocollimatorDeviceController() {
    spdlog::info("Method createAutocollimatorDeviceController of Class  XRayMachineDevicesFactory\n");
    std::shared_ptr<XIMC> clientStepper =
        std::make_shared<XIMC>(clientConfiguration_->getDefaultIpAddressStepper(), clientConfiguration_->readStepperMotorIndexAxis(6), clientConfiguration_->readStepperMotorCONST(6));  // Motor autocollimator axis = 6
    // Scanning
    std::shared_ptr<IScanning> clientScanning =
        std::make_shared<ScanningStepper>(clientStepper, clientSensors_, clientPostProcessing_);
    // Logger
    std::filesystem::path logFileDirectoryName = "LogFiles";
    std::filesystem::path logFileName = "AutocollimatorDeviceController_logFile.txt";
    std::filesystem::path logFilePath = clientConfiguration_->getPartialPathUntilKeyName(currentPath_, projectName_) /= logFileDirectoryName /= logFileName;
    std::shared_ptr<ISingleStepperDeviceController> clientAutocollimatorDeviceController =
        std::make_shared<AutocollimatorDeviceController>(logFilePath,
                                                         clientStepper,
                                                         clientScanning);
    return clientAutocollimatorDeviceController;
}

std::shared_ptr<IMultiStepperDeviceController> XRayMachineDevicesFactory::createMonochromatorDeviceController() {
    spdlog::info("Method createMonochromatorDeviceController of Class  XRayMachineDevicesFactory\n");
    // Stepper
    std::shared_ptr<XIMC> clientStepperLinear =
        std::make_shared<XIMC>(clientConfiguration_->getDefaultIpAddressStepper(), clientConfiguration_->readStepperMotorIndexAxis(0), clientConfiguration_->readStepperMotorCONST(0));  // Motor Monochromator axis Linear = 0
    std::shared_ptr<XIMC> clientStepperRotational =
        std::make_shared<XIMC>(clientConfiguration_->getDefaultIpAddressStepper(), clientConfiguration_->readStepperMotorIndexAxis(1), clientConfiguration_->readStepperMotorCONST(1));  // Motor Monochromator axis Rotational = 1
    // Scanning
    std::shared_ptr<IScanning> clientScanningLinear =
        std::make_shared<ScanningStepper>(clientStepperLinear, clientSensors_, clientPostProcessing_);
    std::shared_ptr<IScanning> clientScanningRotational =
        std::make_shared<ScanningStepper>(clientStepperRotational, clientSensors_, clientPostProcessing_);
    // Logger
    std::filesystem::path logFileDirectoryName = "LogFiles";
    std::filesystem::path logFileName = "MonochromatorDeviceController_logFile.txt";
    std::filesystem::path logFilePath = clientConfiguration_->getPartialPathUntilKeyName(currentPath_, projectName_) /= logFileDirectoryName /= logFileName;
    std::shared_ptr<IMultiStepperDeviceController> clientMonochromatorDeviceController =
        std::make_shared<MonochromatorDeviceController>(logFilePath,
                                                        clientStepperLinear,
                                                        clientStepperRotational,
                                                        clientScanningLinear,
                                                        clientScanningRotational,
                                                        clientSensors_,
                                                        clientConfiguration_,
                                                        clientPostProcessing_);
    return clientMonochromatorDeviceController;
}

std::shared_ptr<IMultiStepperDeviceController> XRayMachineDevicesFactory::createSlitDeviceController() {
    spdlog::info("Method createSlitDeviceController of Class  XRayMachineDevicesFactory\n");
    std::shared_ptr<XIMC> clientStepperLinear =
        std::make_shared<XIMC>(clientConfiguration_->getDefaultIpAddressStepper(), clientConfiguration_->readStepperMotorIndexAxis(2), clientConfiguration_->readStepperMotorCONST(2));  // Motor Slit axis Linear = 2
    std::shared_ptr<XIMC> clientStepperRotational =
        std::make_shared<XIMC>(clientConfiguration_->getDefaultIpAddressStepper(), clientConfiguration_->readStepperMotorIndexAxis(7), clientConfiguration_->readStepperMotorCONST(7));  // Motor Slit axis Rotational = 7
    // Scanning
    std::shared_ptr<IScanning> clientScanningLinear =
        std::make_shared<ScanningStepper>(clientStepperLinear, clientSensors_, clientPostProcessing_);
    std::shared_ptr<IScanning> clientScanningRotational =
        std::make_shared<ScanningStepper>(clientStepperRotational, clientSensors_, clientPostProcessing_);
    // Logger
    std::filesystem::path logFileDirectoryName = "LogFiles";
    std::filesystem::path logFileName = "SlitDeviceController_logFile.txt";
    std::filesystem::path logFilePath = clientConfiguration_->getPartialPathUntilKeyName(currentPath_, projectName_) /= logFileDirectoryName /= logFileName;
    std::shared_ptr<IMultiStepperDeviceController> clientSlitDeviceController =
        std::make_shared<SlitDeviceController>(logFilePath,
                                               clientStepperLinear,
                                               clientStepperRotational,
                                               clientScanningLinear,
                                               clientScanningRotational,
                                               clientSensors_,
                                               clientConfiguration_,
                                               clientPostProcessing_);
    return clientSlitDeviceController;
}

std::shared_ptr<ISingleStepperDeviceController> XRayMachineDevicesFactory::createXRaySensorDeviceController() {
    spdlog::info("Method createXRaySensorDeviceController of Class  XRayMachineDevicesFactory\n");
    std::shared_ptr<XIMC> clientStepper =
        std::make_shared<XIMC>(clientConfiguration_->getDefaultIpAddressStepper(), clientConfiguration_->readStepperMotorIndexAxis(5), clientConfiguration_->readStepperMotorCONST(5));  // Motor XRaySensor axis = 5
    // Scanning
    std::shared_ptr<IScanning> clientScanning =
        std::make_shared<ScanningStepper>(clientStepper, clientSensors_, clientPostProcessing_);
    // Logger
    std::filesystem::path logFileDirectoryName = "LogFiles";
    std::filesystem::path logFileName = "XRaySensorDeviceController_logFile.txt";
    std::filesystem::path logFilePath = clientConfiguration_->getPartialPathUntilKeyName(currentPath_, projectName_) /= logFileDirectoryName /= logFileName;
    std::shared_ptr<ISingleStepperDeviceController> clientXRaySensorDeviceController =
        std::make_shared<XRaySensorDeviceController>(logFilePath,
                                                     clientStepper,
                                                     clientScanning,
                                                     clientConfiguration_);
    return clientXRaySensorDeviceController;
}

std::shared_ptr<ISingleStepperDeviceController> XRayMachineDevicesFactory::createXRaySourceDeviceController() {
    spdlog::info("Method createXRaySourceDeviceController of Class  XRayMachineDevicesFactory\n");
    std::shared_ptr<XIMC> clientStepper =
        std::make_shared<XIMC>(clientConfiguration_->getDefaultIpAddressStepper(), clientConfiguration_->readStepperMotorIndexAxis(4), clientConfiguration_->readStepperMotorCONST(4));  // Motor XRaySource axis = 4
    // Scanning
    std::shared_ptr<IScanning> clientScanning =
        std::make_shared<ScanningStepper>(clientStepper, clientSensors_, clientPostProcessing_);
    // Logger
    std::filesystem::path logFileDirectoryName = "LogFiles";
    std::filesystem::path logFileName = "XRaySourceDeviceController_logFile.txt";
    std::filesystem::path logFilePath = clientConfiguration_->getPartialPathUntilKeyName(currentPath_, projectName_) /= logFileDirectoryName /= logFileName;
    std::shared_ptr<ISingleStepperDeviceController> clientXRaySourceDeviceController =
        std::make_shared<XRaySourceDeviceController>(logFilePath,
                                                     clientStepper,
                                                     clientScanning,
                                                     clientSensors_,
                                                     clientConfiguration_,
                                                     clientPostProcessing_);
    return clientXRaySourceDeviceController;
}
