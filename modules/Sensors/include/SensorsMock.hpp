/**
 * @file SensorsMock.hpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2022
 * @brief Implementation Class of 'ISensors' used to mock methods of the parent class.
 * @version 0.1
 * @date 2022-03-01
 * 
 * @copyright © Copyright CERN 2018. All rights reserved. This software is released under a CERN proprietary software license.
 * Any permission to use it shall be granted in writing. Requests shall be addressed to CERN through mail-KT@cern.ch
 * 
 */



#pragma once

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <string>

#include "ISensors.hpp"

using testing::_;
using testing::Return;
using testing::NiceMock;
using testing::Invoke;

namespace sensors {

/**
 * @class SensorMock
 * @brief Implementation Class of 'ISensors' used to mock methods of the parent class.
 * 
 * The class 'SensorsMock' inherits from 'ISensors' and overrides all of its methods
 * using the MOCK_METHOD macro provided by the Google Test framework.
 * Each overridden method is prefixed with MOCK_METHOD, which takes as arguments the method name,
 * the return type, and the method parameters.
 * 
 */
class SensorsMock : public ISensors {
 public:
  MOCK_METHOD2(startAcquisitionSingleStepper, void(std::string filename, bool eraseCsvContent));
  MOCK_METHOD2(startAcquisitionCrystal, void(std::string filename, bool eraseCsvContent));
  MOCK_METHOD2(readXRaySensor, std::string(int durationAcquisition, float position));
  MOCK_METHOD7(readXRaySensor, std::string(int durationAcquisition, float positionX, float positionY, float positionZ, float positionU, float positionV, float positionW));
  MOCK_METHOD0(deinitializeXRaySensor, void());
  MOCK_METHOD1(motionStabilizationTimer, void(int timerLength));
  MOCK_METHOD1(flushCsv, void(std::string pathToCsv));
  MOCK_METHOD1(readCsvResult, float(std::string filename));
  MOCK_METHOD0(getPathToProjDirectory, std::filesystem::path());
};

}  // namespace sensors
