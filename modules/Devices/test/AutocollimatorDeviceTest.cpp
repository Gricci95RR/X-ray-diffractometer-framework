/**
 * @file AutocollimatorDeviceTest.cpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2022
 * @brief Test the device Autocollimator.
 * @version 0.1
 * @date 2022
 * 
 * @copyright © Copyright CERN 2018. All rights reserved. This software is released under a CERN proprietary software license.
 * Any permission to use it shall be granted in writing. Requests shall be addressed to CERN through mail-KT@cern.ch
 * 
 */

#include <filesystem>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "XRayMachineDevicesFactory.hpp"
#include "StepperMockConfiguration.hpp"
#include "ScanningStepperMockConfiguration.hpp"

using testing::_;
using testing::Return;
using testing::NiceMock;
using testing::An;

using namespace autocollimator;  // NOLINT
using namespace scanning;  // NOLINT

/**
 * @struct AutocollimatorTest
 * @brief Test fixture called 'TestConfiguration', which inherits from 'testing::Test'.
 * 
 */
struct AutocollimatorTest
    : public ::testing::Test {
        /**
         * @brief Set the Up object.
         * 
         */
        void SetUp() override {
            std::filesystem::path logFileName = "AutocollimatorDeviceController_logFile_test.txt";
            std::filesystem::path p1 ="C:\\Users\\giricci\\Desktop\\XRay_Machine\\LogFiles\\";
            std::filesystem::path logFilePath = p1 /= logFileName;
            Stepper1MockConfig_.configureStepperMock();
            ScanningStepperMockConfig_.configureScanningStepperMock();
            sut_.reset(new autocollimator::AutocollimatorDeviceController(
                logFilePath,
                Stepper1MockConfig_.getMock(),
                ScanningStepperMockConfig_.getMock()));
            }
        void TearDown() override {
            }
        std::unique_ptr<autocollimator::AutocollimatorDeviceController> sut_;  /**< System Under Test (SUT) - unique pointer to AutocollimatorDeviceController Class. */
        StepperMockConfiguration Stepper1MockConfig_;
        scanning::ScanningStepperMockConfiguration ScanningStepperMockConfig_;
};

/**
 * @brief Test case that checks the executions of AutocollimatorDeviceController methods.
 * 
 */
TEST_F(AutocollimatorTest, AutocollimatorScan) {
    spdlog::set_level(spdlog::level::debug);  // Set global log level to debug
    ASSERT_TRUE(sut_->start());
    sut_->getFsmState();
    ASSERT_TRUE(sut_->goHome());
    sut_->getFsmState();
    ASSERT_TRUE(sut_->moveToPositionStepper(1));
    sut_->getFsmState();
    ASSERT_TRUE(sut_->scanStepper(0.1, 100, 1, "TestScan.csv", true, false));
    sut_->getFsmState();
    ASSERT_TRUE(sut_->disconnectStp());
    sut_->getFsmState();
}
