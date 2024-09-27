/**
 * @file CrystalDeviceTests.cpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2022
 * @brief Test the device Crystal.
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
#include "SensorsMockConfiguration.hpp"
#include "ScanningStepperMockConfiguration.hpp"
#include "ScanningHXPMockConfiguration.hpp"
#include "ConfigurationMockConfig.hpp"
#include "PostProcessingMockConfiguration.hpp"

using testing::_;
using testing::Return;
using testing::NiceMock;
using testing::An;

using namespace crystal;  // NOLINT
using namespace sensors;  // NOLINT
using namespace scanning;  // NOLINT

/**
 * @struct CrystalDeviceTests
 * @brief Test fixture called 'CrystalDeviceTests', which inherits from 'testing::Test'.
 * 
 */
struct CrystalDeviceTests
    : public ::testing::Test {
        void SetUp() override {
            std::filesystem::path logFileName = "crystalDeviceController_logFile_test.txt";
            std::filesystem::path p1 ="C:\\Users\\giricci\\Desktop\\XRay_Machine\\LogFiles\\";
            std::filesystem::path logFilePath = p1 /= logFileName;
            HXPMockConfig_.configureHXPMock();
            StepperMockConfig_.configureStepperMock();
            ScanningStepperMockConfig_.configureScanningStepperMock();
            ScanningHXPMockConfig_.configureScanningHXPMock();
            ConfigurationMockConfig_.configureConfigurationMock();
            PostProcessingMockConfig_.configurePostProcessingMock();
            sut_.reset(new crystal::CrystalDeviceController(
                logFilePath,
                HXPMockConfig_.getMock(),
                StepperMockConfig_.getMock(),
                ScanningStepperMockConfig_.getMock(),
                ScanningHXPMockConfig_.getMock(),
                SensorsMockConfig_.getMock(),
                ConfigurationMockConfig_.getMock(),
                PostProcessingMockConfig_.getMock()
                ));
            }
        void TearDown() override {
            }
        std::unique_ptr<crystal::CrystalDeviceController> sut_;  /**< System Under Test (SUT) - unique pointer to CrystalDeviceController Class. */
        HXPMockConfiguration HXPMockConfig_;
        StepperMockConfiguration StepperMockConfig_;
        sensors::SensorsMockConfiguration SensorsMockConfig_;
        scanning::ScanningStepperMockConfiguration ScanningStepperMockConfig_;
        scanning::ScanningHXPMockConfiguration ScanningHXPMockConfig_;
        ConfigurationMockConfig ConfigurationMockConfig_;
        PostProcessingMockConfiguration PostProcessingMockConfig_;
};

TEST_F(CrystalDeviceTests, ConnectDevices){
    spdlog::set_level(spdlog::level::debug);  // Set global log level to debug
    ASSERT_TRUE(sut_->start());
}


TEST_F(CrystalDeviceTests, ConnectAndDisconnectDevices){
    spdlog::set_level(spdlog::level::debug);  // Set global log level to debug
    sut_->getFsmState();
    ASSERT_TRUE(sut_->start());
    sut_->getFsmState();
    ASSERT_TRUE(sut_->disconnect());
    sut_->getFsmState();
}

TEST_F(CrystalDeviceTests, ConnectAndGoHomeDevices){
    spdlog::set_level(spdlog::level::debug); // Set global log level to debug
    sut_->getFsmState();
    ASSERT_TRUE(sut_->start());
    sut_->getFsmState();
    ASSERT_TRUE(sut_->goHome());
    sut_->getFsmState();
    ASSERT_TRUE(sut_->disconnect());
    sut_->getFsmState();
}

TEST_F(CrystalDeviceTests, ConnectAndMoveStepper){
    spdlog::set_level(spdlog::level::debug); // Set global log level to debug
    sut_->getFsmState();
    ASSERT_TRUE(sut_->start());
    sut_->getFsmState();
    ASSERT_TRUE(sut_->goHome());
    sut_->getFsmState();
    ASSERT_TRUE(sut_->moveToPositionStepper(0.1));
    sut_->getFsmState();
    ASSERT_TRUE(sut_->disconnect());
    sut_->getFsmState();
}

TEST_F(CrystalDeviceTests, ConnectAndMoveHXP){
    spdlog::set_level(spdlog::level::debug); // Set global log level to debug
    sut_->getFsmState();
    ASSERT_TRUE(sut_->start());
    sut_->getFsmState();
    ASSERT_TRUE(sut_->goHome());
    sut_->getFsmState();
    ASSERT_TRUE(sut_->moveToAbsPosition(0.1, 0.1, 0.1, 0.1, 0.1, 0.1));
    sut_->getFsmState();
    ASSERT_TRUE(sut_->disconnect());
    sut_->getFsmState();
}

TEST_F(CrystalDeviceTests, CrystalDeviceController_X_Alignment) {
    spdlog::set_level(spdlog::level::debug);  // Set global log level to debug
    ASSERT_TRUE(sut_->start());
    sut_->getFsmState();
    ASSERT_TRUE(sut_->goHome());
    sut_->getFsmState();
    ASSERT_TRUE(sut_->xAxisAlignmentCrystal());
    ASSERT_EQ("Connected", sut_->getFsmState());
    sut_->getFsmState();
    ASSERT_TRUE(sut_->disconnect());
    sut_->getFsmState();
}

TEST_F(CrystalDeviceTests, CrystalDeviceController_Y_Alignment) {
    spdlog::set_level(spdlog::level::debug);  // Set global log level to debug
    ASSERT_TRUE(sut_->start());
    sut_->getFsmState();
    ASSERT_TRUE(sut_->goHome());
    sut_->getFsmState();
    ASSERT_TRUE(sut_->yWAxesAlignmentCrystal());
    ASSERT_EQ("Connected", sut_->getFsmState());
    sut_->getFsmState();
    ASSERT_TRUE(sut_->yAxisFineAlignmentCrystal());
    ASSERT_EQ("Connected", sut_->getFsmState());
    sut_->getFsmState();
    ASSERT_TRUE(sut_->disconnect());
    sut_->getFsmState();
}

TEST_F(CrystalDeviceTests, CrystalDeviceController_Z_Alignment) {
    spdlog::set_level(spdlog::level::debug);  // Set global log level to debug
    ASSERT_TRUE(sut_->start());
    sut_->getFsmState();
    ASSERT_TRUE(sut_->goHome());
    sut_->getFsmState();
    ASSERT_TRUE(sut_->zAxisAlignmentCrystal());
    ASSERT_EQ("Connected", sut_->getFsmState());
    sut_->getFsmState();
    ASSERT_TRUE(sut_->disconnect());
    sut_->getFsmState();
}

TEST_F(CrystalDeviceTests, CrystalDeviceController_flipCheck_Alignment) {
    spdlog::set_level(spdlog::level::debug);  // Set global log level to debug
    ASSERT_TRUE(sut_->start());
    sut_->getFsmState();
    ASSERT_TRUE(sut_->goHome());
    sut_->getFsmState();
    ASSERT_TRUE(sut_->checkAlignmentInFlippedOrientation());
    ASSERT_EQ("Connected", sut_->getFsmState());
    sut_->getFsmState();
    ASSERT_TRUE(sut_->disconnect());
    sut_->getFsmState();
}

TEST_F(CrystalDeviceTests, CrystalDeviceController_Bending_Measurement) {
    spdlog::set_level(spdlog::level::debug);  // Set global log level to debug
    ASSERT_TRUE(sut_->start());
    sut_->getFsmState();
    ASSERT_TRUE(sut_->goHome());
    sut_->getFsmState();
    ASSERT_TRUE(sut_->bendingAngleMeasurement());
    ASSERT_EQ("Connected", sut_->getFsmState());
    sut_->getFsmState();
    ASSERT_TRUE(sut_->disconnect());
    sut_->getFsmState();
}

TEST_F(CrystalDeviceTests, CrystalDeviceController_Torsion_Measurement) {
    spdlog::set_level(spdlog::level::debug);  // Set global log level to debug
    ASSERT_TRUE(sut_->start());
    sut_->getFsmState();
    ASSERT_TRUE(sut_->goHome());
    ASSERT_EQ("Home", sut_->getFsmState());
    ASSERT_TRUE(sut_->torsionAngleMeasurement());
    ASSERT_EQ("Connected", sut_->getFsmState());
    sut_->getFsmState();
    ASSERT_TRUE(sut_->disconnect());
    sut_->getFsmState();
}