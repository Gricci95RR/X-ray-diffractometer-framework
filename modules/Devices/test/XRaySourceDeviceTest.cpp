/**
 * @file XRaySourceDeviceTest.cpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2022
 * @brief Test the device XRaySource.
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
#include "SensorsMockConfiguration.hpp"
#include "ConfigurationMockConfig.hpp"
#include "PostProcessingMockConfiguration.hpp"

using testing::_;
using testing::Return;
using testing::NiceMock;
using testing::An;

using namespace xRaySource;  // NOLINT
using namespace scanning;  // NOLINT
using namespace sensors;  // NOLINT

/**
 * @class XRaySourceDeviceControllerTest
 * @brief Test fixture called 'XRaySourceDeviceControllerTest', which inherits from 'testing::Test'.
 * 
 */
struct XRaySourceDeviceControllerTest
    : public ::testing::Test {
        void SetUp() override {
            std::filesystem::path logFileName = "XRaySourceDeviceController_logFile_test.txt";
            std::filesystem::path p1 ="C:\\Users\\giricci\\Desktop\\XRay_Machine\\LogFiles\\";
            std::filesystem::path logFilePath = p1 / logFileName;
            Stepper1MockConfig_.configureStepperMock();
            ScanningStepperMockConfig_.configureScanningStepperMock();
            SensorsMockConfigurationConfig_.configureSensorsMock();
            ConfigurationMockConfig_.configureConfigurationMock();
            PostProcessingMockConfig_.configurePostProcessingMock();
            sut_.reset(new xRaySource::XRaySourceDeviceController(
                logFilePath,
                Stepper1MockConfig_.getMock(),
                ScanningStepperMockConfig_.getMock(),
                SensorsMockConfigurationConfig_.getMock(),
                ConfigurationMockConfig_.getMock(),
                PostProcessingMockConfig_.getMock()
                ));
            }

        void TearDown() override {
            }
        std::unique_ptr<xRaySource::XRaySourceDeviceController> sut_;  /**< System Under Test (SUT) - unique pointer to XRaySourceDeviceController Class. */
        StepperMockConfiguration Stepper1MockConfig_;
        scanning::ScanningStepperMockConfiguration ScanningStepperMockConfig_;
        sensors::SensorsMockConfiguration SensorsMockConfigurationConfig_;
        ConfigurationMockConfig ConfigurationMockConfig_;
        PostProcessingMockConfiguration PostProcessingMockConfig_;
};

TEST_F(XRaySourceDeviceControllerTest, XRaySourceDeviceController_Scan) {
    spdlog::set_level(spdlog::level::debug);  // Set global log level to debug
    ASSERT_TRUE(sut_->start());
    sut_->getFsmState();
    ASSERT_TRUE(sut_->goHome());
    sut_->getFsmState();
    ASSERT_TRUE(sut_->moveToPositionStepper(1));
    sut_->getFsmState();
    ASSERT_TRUE(sut_->scanStepper(0.1, 100, 1, "TestScan.csv", true, false));
    ASSERT_EQ("Connected", sut_->getFsmState());
    sut_->getFsmState();
    ASSERT_TRUE(sut_->disconnectStp());
    sut_->getFsmState();
}

TEST_F(XRaySourceDeviceControllerTest, XRaySourceDeviceController_Alignment) {
    spdlog::set_level(spdlog::level::debug);  // Set global log level to debug
    ASSERT_TRUE(sut_->start());
    sut_->getFsmState();
    ASSERT_TRUE(sut_->goHome());
    sut_->getFsmState();
    ASSERT_TRUE(sut_->alignSourceWithSensor(false));
    ASSERT_EQ("Connected", sut_->getFsmState());
    sut_->getFsmState();
    ASSERT_TRUE(sut_->disconnectStp());
    sut_->getFsmState();
}
