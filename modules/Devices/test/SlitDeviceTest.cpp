/**
 * @file SlitrDeviceTest.cpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2022
 * @brief Test the device Slit.
 * @version 0.1
 * @date 2022
 * 
 * @copyright © Copyright CERN 2018. All rights reserved. This software is released under a CERN proprietary software license.
 * Any permission to use it shall be granted in writing. Requests shall be addressed to CERN through mail-KT@cern.ch
 * 
 */

#include <filesystem>

#include <gtest/gtest.h>

#include "XRayMachineDevicesFactory.hpp"
#include "HXPMockConfiguration.hpp"
#include "StepperMockConfiguration.hpp"
#include "ScanningStepperMockConfiguration.hpp"
#include "SensorsMockConfiguration.hpp"
#include "ConfigurationMockConfig.hpp"
#include "PostProcessingMockConfiguration.hpp"

using testing::_;
using testing::Return;
using testing::NiceMock;
using testing::An;

using namespace slit;  // NOLINT
using namespace scanning;  // NOLINT
using namespace sensors;  // NOLINT

/**
 * @class SlitTests
 * @brief Test fixture called 'SlitTests', which inherits from 'testing::Test'.
 * 
 */
struct SlitTests
    : public ::testing::Test {
        void SetUp() override {
            std::filesystem::path logFileName = "slitDeviceController_logFile_test.txt";
            std::filesystem::path p1 ="C:\\Users\\giricci\\Desktop\\XRay_Machine\\LogFiles\\";
            std::filesystem::path logFilePath = p1 /= logFileName;
            Stepper1MockConfig_.configureStepperMock();
            Stepper2MockConfig_.configureStepperMock();
            ScanningStepper1MockConfig_.configureScanningStepperMock();
            ScanningStepper2MockConfig_.configureScanningStepperMock();
            SensorsMockConfigurationConfig_.configureSensorsMock();
            ConfigurationMockConfig_.configureConfigurationMock();
            PostProcessingMockConfig_.configurePostProcessingMock();
            sut_.reset(new slit::SlitDeviceController(
                logFilePath,
                Stepper1MockConfig_.getMock(),
                Stepper2MockConfig_.getMock(),
                ScanningStepper1MockConfig_.getMock(),
                ScanningStepper2MockConfig_.getMock(),
                SensorsMockConfigurationConfig_.getMock(),
                ConfigurationMockConfig_.getMock(),
                PostProcessingMockConfig_.getMock()
                ));
            }
        void TearDown() override {
            }
        std::unique_ptr<slit::SlitDeviceController> sut_;  /**< System Under Test (SUT) - unique pointer to SlitDeviceController Class. */
        StepperMockConfiguration Stepper1MockConfig_;
        StepperMockConfiguration Stepper2MockConfig_;
        scanning::ScanningStepperMockConfiguration ScanningStepper1MockConfig_;
        scanning::ScanningStepperMockConfiguration ScanningStepper2MockConfig_;
        sensors::SensorsMockConfiguration SensorsMockConfigurationConfig_;
        ConfigurationMockConfig ConfigurationMockConfig_;
        PostProcessingMockConfiguration PostProcessingMockConfig_;
};


TEST_F(SlitTests, ConnectDevicesSlit){
    spdlog::set_level(spdlog::level::debug); // Set global log level to debug
    ASSERT_TRUE(sut_->start());
}

TEST_F(SlitTests, ConnectAndDisconnectDevicesSlit){
    spdlog::set_level(spdlog::level::debug); // Set global log level to debug
    sut_->getFsmState();
    ASSERT_TRUE(sut_->start());
    sut_->getFsmState();
    ASSERT_TRUE(sut_->disconnectStp());
    sut_->getFsmState();
}

TEST_F(SlitTests, ConnectAndGoHomeDevicesSlit){
    spdlog::set_level(spdlog::level::debug); // Set global log level to debug
    sut_->getFsmState();
    ASSERT_TRUE(sut_->start());
    sut_->getFsmState();
    ASSERT_TRUE(sut_->goHome());
    sut_->getFsmState();
    ASSERT_TRUE(sut_->disconnectStp());
    sut_->getFsmState();
}

TEST_F(SlitTests, ConnectAndMoveStepperSlit){
    spdlog::set_level(spdlog::level::debug); // Set global log level to debug
    sut_->getFsmState();
    ASSERT_TRUE(sut_->start());
    sut_->getFsmState();
    ASSERT_TRUE(sut_->goHome());
    sut_->getFsmState();
    ASSERT_TRUE(sut_->moveToPositionStepper1Linear(0.1));
    sut_->getFsmState();
    ASSERT_TRUE(sut_->moveToPositionStepper2Rotational(0.1));
    sut_->getFsmState();
    ASSERT_TRUE(sut_->disconnectStp());
    sut_->getFsmState();
}

TEST_F(SlitTests, SlitTestsDeviceController_Alignment) {
    spdlog::set_level(spdlog::level::debug);  // Set global log level to debug
    ASSERT_TRUE(sut_->start());
    sut_->getFsmState();
    ASSERT_TRUE(sut_->goHome());
    ASSERT_EQ("Home", sut_->getFsmState());
    ASSERT_TRUE(sut_->alignSlit(false));
    ASSERT_EQ("Connected", sut_->getFsmState());
    sut_->getFsmState();
    ASSERT_TRUE(sut_->disconnectStp());
    sut_->getFsmState();
}
