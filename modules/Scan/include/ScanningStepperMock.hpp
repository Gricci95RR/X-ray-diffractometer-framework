/**
 * @file ScanningStepperMock.hpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2022
 * @brief Implementation Class of 'IScanning' used to mock methods of the parent class.
 * @version 0.1
 * @date 2022
 * 
 * @copyright © Copyright CERN 2018. All rights reserved. This software is released under a CERN proprietary software license.
 * Any permission to use it shall be granted in writing. Requests shall be addressed to CERN through mail-KT@cern.ch
 * 
 */
#pragma once

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <string>

#include "ScanningStepper.hpp"

using testing::_;
using testing::Return;
using testing::NiceMock;
using testing::Invoke;

namespace scanning {

/**
 * @class ScanningStepperMock
 * @brief Implementation Class of 'IScanning' used to mock methods of the parent class.
 * @details The class 'ConfigurationMock' inherits from 'IConfiguration'
 * and overrides all of its methods using the MOCK_METHOD macro provided
 * by the Google Test framework. Each overridden method is prefixed with MOCK_METHOD,
 * which takes as arguments the method name, the return type, and the method parameters.
 * 
 */
class ScanningStepperMock : public IScanning {
 public:
  MOCK_METHOD0(scan, bool());
  MOCK_METHOD0(scanRelative, bool());
  MOCK_METHOD0(getStepSize, float());
  MOCK_METHOD1(setStepSize, void(float stepSize));
  MOCK_METHOD0(getDurationAcquisition, int());
  MOCK_METHOD1(setDurationAcquisition, void(int durationAcquisition));
  MOCK_METHOD0(getFilename, std::string());
  MOCK_METHOD1(setFilename, void(std::string filename));
  MOCK_METHOD0(getEraseCsvContent, bool());
  MOCK_METHOD1(setEraseCsvContent, void(bool eraseCsvContent));
  MOCK_METHOD0(getRange, float());
  MOCK_METHOD1(setRange, void(float range));
  MOCK_METHOD0(stop, bool());
  MOCK_METHOD0(getShowPlot, bool());
  MOCK_METHOD1(setShowPlot, void(bool showPlot));
  MOCK_METHOD6(setupAlignmentParameters, void(float stepSize,
                                              float range,
                                              int durationAcquisition,
                                              std::string filename,
                                              bool eraseCsvContent,
                                              bool showPlot));
  MOCK_METHOD2(checkReachingPosition, bool(float currentPosition,
                                           float targetPosition));
};

}  // namespace scanning
