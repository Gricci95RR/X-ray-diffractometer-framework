/**
 * @file XRaySensorMock.hpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2022
 * @brief Implementation Class of @ref IXRaySensor used to mock methods of the parent class.
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

#include "IXRaySensor.hpp"

using testing::_;
using testing::Return;
using testing::NiceMock;
using testing::Invoke;

/**
 * @class XRaySensorMock
 * @brief Implementation Class of @ref IXRaySensor used to mock methods of the parent class.
 * 
 * The class 'XRaySensorMock' inherits from 'IXRaySensor' and overrides all of its methods
 * using the MOCK_METHOD macro provided by the Google Test framework.
 * Each overridden method is prefixed with MOCK_METHOD, which takes as arguments the method name,
 * the return type, and the method parameters.
 * 
 */
class XRaySensorMock : public IXRaySensor {
 public:
  MOCK_METHOD0(read,
  std::string());
  MOCK_METHOD1(write,
  void(std::string data));
  MOCK_METHOD0(deinitialize,
  void());
};
