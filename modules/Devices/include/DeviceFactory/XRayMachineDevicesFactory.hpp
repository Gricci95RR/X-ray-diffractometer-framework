/**
 * @file XRayMachineDevicesFactory.hpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2022
 * @brief This file defines the Class DevicesFactory.
 * @version 0.1
 * @date 2022
 * 
 * @copyright © Copyright CERN 2018. All rights reserved. This software is released under a CERN proprietary software license.
 * Any permission to use it shall be granted in writing. Requests shall be addressed to CERN through mail-KT@cern.ch
 * 
 */

#pragma once

#include <spdlog/spdlog.h>

#include <string>
#include <cassert>
#include <memory>

#include "IDevicesFactory.hpp"
#include "HXP.hpp"
#include "XIMC.hpp"
#include "Configuration.hpp"
#include "PostProcessing.hpp"
#include "ScanningHXP.hpp"
#include "ScanningStepper.hpp"
#include "Sensors.hpp"
#include "Slit/SlitDeviceController.hpp"
#include "Autocollimator/AutocollimatorDeviceController.hpp"
#include "Crystal/CrystalDeviceController.hpp"
#include "Monochromator/MonochromatorDeviceController.hpp"
#include "XRaySensor/XRaySensorDeviceController.hpp"
#include "XRaySource/XRaySourceDeviceController.hpp"

using namespace autocollimator;  // NOLINT
using namespace crystal;  // NOLINT
using namespace slit;  // NOLINT
using namespace scanning;  // NOLINT
using namespace sensors;  // NOLINT
using namespace monochromator;  // NOLINT
using namespace xRaySource;  // NOLINT
using namespace xRaySensor;  // NOLINT

/**
 * @class XRayMachineDevicesFactory
 * @brief Implementation of the IDevicesFactory interface.
 * 
 */
class XRayMachineDevicesFactory : public IDevicesFactory {
 public:
  /**
   * @brief Construct a new XRayMachineDevicesFactory object.
   * 
   * Creates the objects of the classes Sensors, Configuration and PostProcessing.
   * 
   */
  XRayMachineDevicesFactory();
  /**
   * @brief Destroy the XRayMachineDevicesFactory object.
   * 
   */
  ~XRayMachineDevicesFactory();
  std::shared_ptr<ICrystalDeviceController> createCrystalDeviceController() override;
  std::shared_ptr<ISingleStepperDeviceController> createAutocollimatorDeviceController() override;
  std::shared_ptr<IMultiStepperDeviceController> createMonochromatorDeviceController() override;
  std::shared_ptr<IMultiStepperDeviceController> createSlitDeviceController() override;
  std::shared_ptr<ISingleStepperDeviceController> createXRaySensorDeviceController() override;
  std::shared_ptr<ISingleStepperDeviceController> createXRaySourceDeviceController() override;
  std::shared_ptr<ISensors> clientSensors_;  /**< Shared pointer to ISensor Class. */
  std::shared_ptr<IConfiguration> clientConfiguration_;  /**< Shared pointer to IConfiguration Class. */
  std::shared_ptr<IPostProcessing> clientPostProcessing_;  /**< Shared pointer to IPostProcessing Class. */
  std::filesystem::path currentPath_;
  std::filesystem::path projectName_;  /**< Name of the project. */
};
