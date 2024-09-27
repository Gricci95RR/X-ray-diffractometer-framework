/**
 * @file IDevicesFactory.hpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2022
 * @brief This file defines the Class IDevicesFactory.
 * @version 0.1
 * @date 2022
 * 
 * @copyright © Copyright CERN 2018. All rights reserved. This software is released under a CERN proprietary software license.
 * Any permission to use it shall be granted in writing. Requests shall be addressed to CERN through mail-KT@cern.ch
 * 
 */

#pragma once

#include <iostream>
#include <string>
#include <memory>

#include "ICrystalDeviceController.hpp"
#include "IMultiStepperDeviceController.hpp"
#include "ISingleStepperDeviceController.hpp"
#include "IConfiguration.hpp"
#include "IPostProcessing.hpp"

/**
 * @class IDevicesFactory
 * @brief Interface used to create devices objects.
 * 
 */
class IDevicesFactory {
 public:
  ~IDevicesFactory() = default;
  /**
  * @brief creates CrystalDeviceController object.
  * @return shared pointer to CrystalDeviceController object.
  */
  virtual std::shared_ptr<ICrystalDeviceController> createCrystalDeviceController() = 0;
  /**
  * @brief creates AutocollimatorDeviceController object.
  * @return shared pointer to AutocollimatorDeviceController object.
  */
  virtual std::shared_ptr<ISingleStepperDeviceController> createAutocollimatorDeviceController() = 0;
  /**
  * @brief creates MonochromatorDeviceController object.
  * @return shared pointer to MonochromatorDeviceController object.
  */
  virtual std::shared_ptr<IMultiStepperDeviceController> createMonochromatorDeviceController() = 0;
  /**
  * @brief creates SlitDeviceController object.
  * @return shared pointer to SlitDeviceController object.
  */
  virtual std::shared_ptr<IMultiStepperDeviceController> createSlitDeviceController() = 0;
  /**
  * @brief creates XRaySourceDeviceController object.
  * @return shared pointer to XRaySourceDeviceController object.
  */
  virtual std::shared_ptr<ISingleStepperDeviceController> createXRaySourceDeviceController() = 0;
  /**
  * @brief creates XRaySensorDeviceController object.
  * @return shared pointer to XRaySensorDeviceController object.
  */
  virtual std::shared_ptr<ISingleStepperDeviceController> createXRaySensorDeviceController() = 0;
};
