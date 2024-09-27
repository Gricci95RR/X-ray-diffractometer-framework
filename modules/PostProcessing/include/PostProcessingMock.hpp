/**
 * @file PostProcessingMock.hpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2022
 * @brief Implementation Class of 'IPostProcessing' used to mock methods of the parent class.
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

#include "IPostProcessing.hpp"

using testing::_;
using testing::Return;
using testing::NiceMock;
using testing::Invoke;

/**
 * @class PostProcessingMock
 * @brief Implementation Class of 'IPostProcessing' used to mock methods of the parent class.
 * 
 * The class 'PostProcessingMock' inherits from 'IPostProcessing' and overrides all of its methods
 * using the MOCK_METHOD macro provided by the Google Test framework.
 * Each overridden method is prefixed with MOCK_METHOD, which takes as arguments the method name,
 * the return type, and the method parameters.
 * 
 */
class PostProcessingMock : public IPostProcessing {
 public:
    MOCK_METHOD2(executeScript1, void(std::string pathToPythonScript, 
                                      std::string argument1));
    MOCK_METHOD3(executeScript2, void(std::string pathToPythonScript, 
                                      std::string argument1,
                                      std::string argument2));
    MOCK_METHOD6(executeScript5, void(std::string pathToPythonScript, 
                                      std::string argument1,
                                      std::string argument2,
                                      std::string argument3,
                                      std::string argument4,
                                      std::string argument5));
    MOCK_METHOD7(executeScript6, void(std::string pathToPythonScript, 
                                      std::string argument1,
                                      std::string argument2,
                                      std::string argument3,
                                      std::string argument4,
                                      std::string argument5,
                                      std::string argument6));
    MOCK_METHOD8(executeScript7, void(std::string pathToPythonScript, 
                                      std::string argument1,
                                      std::string argument2,
                                      std::string argument3,
                                      std::string argument4,
                                      std::string argument5,
                                      std::string argument6,
                                      std::string argument7));
};
