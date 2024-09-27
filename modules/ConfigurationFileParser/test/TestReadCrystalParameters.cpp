/**
 * @file TestReadCrystalParameters.cpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2022
 * @brief Test that checks if the configuration file 'AlignmentSettings.ini' contains all the required keys to perform the crystal alignments or measurements.
 * @version 0.1
 * @date 2022
 * 
 * @copyright © Copyright CERN 2018. All rights reserved. This software is released under a CERN proprietary software license.
 * Any permission to use it shall be granted in writing. Requests shall be addressed to CERN through mail-KT@cern.ch
 * 
 */

#include <gtest/gtest.h>
#include <filesystem>

#include "Configuration.hpp"

/**
 * @brief Test fixture called 'TestReadCrystalParameters', which inherits from 'testing::Test'.
 * 
 */
struct TestReadCrystalParameters : public ::testing::Test {
    std::filesystem::path currentPath = std::filesystem::current_path();
    std::filesystem::path projectName = "XRay_Machine";
    std::filesystem::path directoryName = "ConfigurationFiles";
    std::filesystem::path filePath;

    std::shared_ptr<IConfiguration> conf1;
    std::filesystem::path pathToDir;

    void SetUp() override {
        conf1 = std::make_shared<Configuration>();
        pathToDir = conf1->getPartialPathUntilKeyName(currentPath, projectName) / directoryName;
    }
};

/**
 * @brief Test case that checks if the configuration file 'config.ini' contains the keys 'IpAdress' and 'nPort'.
 * 
 */
TEST_F(TestReadCrystalParameters, ReadHxpIP_nPort) {
    EXPECT_EQ(1, conf1->hasKey("Hexapod", "IpAdress", conf1->getConfigFilename(), pathToDir));
    EXPECT_EQ(1, conf1->hasKey("Hexapod", "nPort", conf1->getConfigFilename(), pathToDir));
}

/**
 * @brief Test case that checks if the configuration file 'AlignmentSettins.ini' contains the keys to align the crystal on the x-axis.
 * 
 */
TEST_F(TestReadCrystalParameters, read_xAxis_Alignment_CRYSTAL_STAGE_parameters) {
    EXPECT_EQ(1, conf1->hasKey("xAxis_Alignment_CRYSTAL_STAGE", "SCRIPT_NAME", conf1->getAlignmentSettingsConfigFilename(), pathToDir));
    EXPECT_EQ(1, conf1->hasKey("xAxis_Alignment_CRYSTAL_STAGE", "FILENAME_TO_X_ALIGNMENT_POSITION", conf1->getAlignmentSettingsConfigFilename(), pathToDir));
    EXPECT_EQ(1, conf1->hasKey("xAxis_Alignment_CRYSTAL_STAGE", "START_POSITION_STEPPER", conf1->getAlignmentSettingsConfigFilename(), pathToDir));
    EXPECT_EQ(1, conf1->hasKey("xAxis_Alignment_CRYSTAL_STAGE", "START_POSITION_HXP_X", conf1->getAlignmentSettingsConfigFilename(), pathToDir));
    EXPECT_EQ(1, conf1->hasKey("xAxis_Alignment_CRYSTAL_STAGE", "START_POSITION_HXP_Y", conf1->getAlignmentSettingsConfigFilename(), pathToDir));
    EXPECT_EQ(1, conf1->hasKey("xAxis_Alignment_CRYSTAL_STAGE", "START_POSITION_HXP_Z", conf1->getAlignmentSettingsConfigFilename(), pathToDir));
    EXPECT_EQ(1, conf1->hasKey("xAxis_Alignment_CRYSTAL_STAGE", "START_POSITION_HXP_U", conf1->getAlignmentSettingsConfigFilename(), pathToDir));
    EXPECT_EQ(1, conf1->hasKey("xAxis_Alignment_CRYSTAL_STAGE", "START_POSITION_HXP_V", conf1->getAlignmentSettingsConfigFilename(), pathToDir));
    EXPECT_EQ(1, conf1->hasKey("xAxis_Alignment_CRYSTAL_STAGE", "START_POSITION_HXP_W", conf1->getAlignmentSettingsConfigFilename(), pathToDir));
    EXPECT_EQ(1, conf1->hasKey("xAxis_Alignment_CRYSTAL_STAGE", "STEP_SIZE", conf1->getAlignmentSettingsConfigFilename(), pathToDir));
    EXPECT_EQ(1, conf1->hasKey("xAxis_Alignment_CRYSTAL_STAGE", "RANGE", conf1->getAlignmentSettingsConfigFilename(), pathToDir));
    EXPECT_EQ(1, conf1->hasKey("xAxis_Alignment_CRYSTAL_STAGE", "DURATION_ACQUISITION", conf1->getAlignmentSettingsConfigFilename(), pathToDir));
    EXPECT_EQ(1, conf1->hasKey("xAxis_Alignment_CRYSTAL_STAGE", "DATA_LOG_FILENAME", conf1->getAlignmentSettingsConfigFilename(), pathToDir));
    EXPECT_EQ(1, conf1->hasKey("xAxis_Alignment_CRYSTAL_STAGE", "ERASE_CSV_CONTENT", conf1->getAlignmentSettingsConfigFilename(), pathToDir));
}

/**
 * @brief Test case that checks if the configuration file 'AlignmentSettins.ini' contains the keys to align the crystal on the z-axis.
 * 
 */
TEST_F(TestReadCrystalParameters, read_zAxis_Alignment_CRYSTAL_STAGE_parameters) {
    EXPECT_EQ(1, conf1->hasKey("zAxis_Alignment_CRYSTAL_STAGE", "SCRIPT_NAME", conf1->getAlignmentSettingsConfigFilename(), pathToDir));
    EXPECT_EQ(1, conf1->hasKey("zAxis_Alignment_CRYSTAL_STAGE", "FILENAME_TO_Z_ALIGNMENT_POSITION ", conf1->getAlignmentSettingsConfigFilename(), pathToDir));
    EXPECT_EQ(1, conf1->hasKey("zAxis_Alignment_CRYSTAL_STAGE", "START_POSITION_STEPPER", conf1->getAlignmentSettingsConfigFilename(), pathToDir));
    EXPECT_EQ(1, conf1->hasKey("zAxis_Alignment_CRYSTAL_STAGE", "START_POSITION_HXP_Z", conf1->getAlignmentSettingsConfigFilename(), pathToDir));
    EXPECT_EQ(1, conf1->hasKey("zAxis_Alignment_CRYSTAL_STAGE", "STEP_SIZE", conf1->getAlignmentSettingsConfigFilename(), pathToDir));
    EXPECT_EQ(1, conf1->hasKey("zAxis_Alignment_CRYSTAL_STAGE", "RANGE", conf1->getAlignmentSettingsConfigFilename(), pathToDir));
    EXPECT_EQ(1, conf1->hasKey("zAxis_Alignment_CRYSTAL_STAGE", "DURATION_ACQUISITION", conf1->getAlignmentSettingsConfigFilename(), pathToDir));
    EXPECT_EQ(1, conf1->hasKey("zAxis_Alignment_CRYSTAL_STAGE", "DATA_LOG_FILENAME", conf1->getAlignmentSettingsConfigFilename(), pathToDir));
    EXPECT_EQ(1, conf1->hasKey("zAxis_Alignment_CRYSTAL_STAGE", "ERASE_CSV_CONTENT", conf1->getAlignmentSettingsConfigFilename(), pathToDir));
}

/**
 * @brief Test case that checks if the configuration file 'AlignmentSettins.ini' contains the keys to align the crystal on the y-axis and w-axis.
 * 
 */
TEST_F(TestReadCrystalParameters, read_yWAxis_Alignment_CRYSTAL_STAGE_parameters) {
    EXPECT_EQ(1, conf1->hasKey("yWAxis_Alignment_CRYSTAL_STAGE", "SCRIPT_NAME", conf1->getAlignmentSettingsConfigFilename(), pathToDir));
    EXPECT_EQ(1, conf1->hasKey("yWAxis_Alignment_CRYSTAL_STAGE", "FILENAME_TO_Y_ALIGNMENT_POSITION  ", conf1->getAlignmentSettingsConfigFilename(), pathToDir));
    EXPECT_EQ(1, conf1->hasKey("yWAxis_Alignment_CRYSTAL_STAGE", "FILENAME_TO_STANDARD_DEVIATION_RESULT", conf1->getAlignmentSettingsConfigFilename(), pathToDir));
    EXPECT_EQ(1, conf1->hasKey("yWAxis_Alignment_CRYSTAL_STAGE", "FILENAME_TO_SLOPESFILE", conf1->getAlignmentSettingsConfigFilename(), pathToDir));
    EXPECT_EQ(1, conf1->hasKey("yWAxis_Alignment_CRYSTAL_STAGE", "THRESHOLD_STD_DEVIATION", conf1->getAlignmentSettingsConfigFilename(), pathToDir));
    EXPECT_EQ(1, conf1->hasKey("yWAxis_Alignment_CRYSTAL_STAGE", "START_POSITION_SCAN_HXP_Y", conf1->getAlignmentSettingsConfigFilename(), pathToDir));
    EXPECT_EQ(1, conf1->hasKey("yWAxis_Alignment_CRYSTAL_STAGE", "STEP_SIZE_SCAN_HXP_Y", conf1->getAlignmentSettingsConfigFilename(), pathToDir));
    EXPECT_EQ(1, conf1->hasKey("yWAxis_Alignment_CRYSTAL_STAGE", "RANGE_SCAN_HXP_Y  ", conf1->getAlignmentSettingsConfigFilename(), pathToDir));
    EXPECT_EQ(1, conf1->hasKey("yWAxis_Alignment_CRYSTAL_STAGE", "START_POSITION_HXP_W   ", conf1->getAlignmentSettingsConfigFilename(), pathToDir));
    EXPECT_EQ(1, conf1->hasKey("yWAxis_Alignment_CRYSTAL_STAGE", "STEP_SIZE", conf1->getAlignmentSettingsConfigFilename(), pathToDir));
    EXPECT_EQ(1, conf1->hasKey("yWAxis_Alignment_CRYSTAL_STAGE", "RANGE", conf1->getAlignmentSettingsConfigFilename(), pathToDir));
    EXPECT_EQ(1, conf1->hasKey("yWAxis_Alignment_CRYSTAL_STAGE", "DURATION_ACQUISITION", conf1->getAlignmentSettingsConfigFilename(), pathToDir));
    EXPECT_EQ(1, conf1->hasKey("yWAxis_Alignment_CRYSTAL_STAGE", "DATA_LOG_FILENAME", conf1->getAlignmentSettingsConfigFilename(), pathToDir));
    EXPECT_EQ(1, conf1->hasKey("yWAxis_Alignment_CRYSTAL_STAGE", "ERASE_CSV_CONTENT", conf1->getAlignmentSettingsConfigFilename(), pathToDir));
}

/**
 * @brief Test case that checks if the configuration file 'AlignmentSettins.ini' contains the keys to search the crystal Bragg's peak.
 * 
 */
TEST_F(TestReadCrystalParameters, read_Braggs_Peak_Search_CRYSTAL_STAGE_parameters) {
    EXPECT_EQ(1, conf1->hasKey("Braggs_Peak_Search_CRYSTAL_STAGE", "SCRIPT_NAME", conf1->getAlignmentSettingsConfigFilename(), pathToDir));
    EXPECT_EQ(1, conf1->hasKey("Braggs_Peak_Search_CRYSTAL_STAGE", "FILENAME_TO_BRAGG_ANGLE   ", conf1->getAlignmentSettingsConfigFilename(), pathToDir));
    EXPECT_EQ(1, conf1->hasKey("Braggs_Peak_Search_CRYSTAL_STAGE", "STEP_SIZE", conf1->getAlignmentSettingsConfigFilename(), pathToDir));
    EXPECT_EQ(1, conf1->hasKey("Braggs_Peak_Search_CRYSTAL_STAGE", "RANGE", conf1->getAlignmentSettingsConfigFilename(), pathToDir));
    EXPECT_EQ(1, conf1->hasKey("Braggs_Peak_Search_CRYSTAL_STAGE", "DURATION_ACQUISITION", conf1->getAlignmentSettingsConfigFilename(), pathToDir));
    EXPECT_EQ(1, conf1->hasKey("Braggs_Peak_Search_CRYSTAL_STAGE", "DATA_LOG_FILENAME", conf1->getAlignmentSettingsConfigFilename(), pathToDir));
    EXPECT_EQ(1, conf1->hasKey("Braggs_Peak_Search_CRYSTAL_STAGE", "ERASE_CSV_CONTENT", conf1->getAlignmentSettingsConfigFilename(), pathToDir));
}

/**
 * @brief Test case that checks if the configuration file 'AlignmentSettins.ini' contains the keys to align the crystal on the y-axis.
 * 
 */
TEST_F(TestReadCrystalParameters, read_yAxis_Fine_Alignment_CRYSTAL_STAGE_parameters) {
    EXPECT_EQ(1, conf1->hasKey("yAxis_Fine_Alignment_CRYSTAL_STAGE", "SCRIPT_NAME_FULLY_OPENED_BEAM", conf1->getAlignmentSettingsConfigFilename(), pathToDir));
    EXPECT_EQ(1, conf1->hasKey("yAxis_Fine_Alignment_CRYSTAL_STAGE", "FILENAME_TO_FULLY_OPENED_BEAM", conf1->getAlignmentSettingsConfigFilename(), pathToDir));
    EXPECT_EQ(1, conf1->hasKey("yAxis_Fine_Alignment_CRYSTAL_STAGE", "SCRIPT_NAME", conf1->getAlignmentSettingsConfigFilename(), pathToDir));
    EXPECT_EQ(1, conf1->hasKey("yAxis_Fine_Alignment_CRYSTAL_STAGE", "FILENAME_TO_Y_ALIGNMENT_POSITION", conf1->getAlignmentSettingsConfigFilename(), pathToDir));
    EXPECT_EQ(1, conf1->hasKey("yAxis_Fine_Alignment_CRYSTAL_STAGE", "FILENAME_TO_Yin", conf1->getAlignmentSettingsConfigFilename(), pathToDir));
    EXPECT_EQ(1, conf1->hasKey("yAxis_Fine_Alignment_CRYSTAL_STAGE", "FILENAME_TO_Yout", conf1->getAlignmentSettingsConfigFilename(), pathToDir));
    EXPECT_EQ(1, conf1->hasKey("yAxis_Fine_Alignment_CRYSTAL_STAGE", "CRYSTAL_WIDTH", conf1->getAlignmentSettingsConfigFilename(), pathToDir));
    EXPECT_EQ(1, conf1->hasKey("yAxis_Fine_Alignment_CRYSTAL_STAGE", "STEP_SIZE_SCAN_HXP_Y", conf1->getAlignmentSettingsConfigFilename(), pathToDir));
    EXPECT_EQ(1, conf1->hasKey("yAxis_Fine_Alignment_CRYSTAL_STAGE", "RANGE_SCAN_HXP_Y", conf1->getAlignmentSettingsConfigFilename(), pathToDir));
    EXPECT_EQ(1, conf1->hasKey("yAxis_Fine_Alignment_CRYSTAL_STAGE", "STEP_SIZE", conf1->getAlignmentSettingsConfigFilename(), pathToDir));
    EXPECT_EQ(1, conf1->hasKey("yAxis_Fine_Alignment_CRYSTAL_STAGE", "RANGE", conf1->getAlignmentSettingsConfigFilename(), pathToDir));
    EXPECT_EQ(1, conf1->hasKey("yAxis_Fine_Alignment_CRYSTAL_STAGE", "DURATION_ACQUISITION", conf1->getAlignmentSettingsConfigFilename(), pathToDir));
    EXPECT_EQ(1, conf1->hasKey("yAxis_Fine_Alignment_CRYSTAL_STAGE", "DATA_LOG_FILENAME", conf1->getAlignmentSettingsConfigFilename(), pathToDir));
    EXPECT_EQ(1, conf1->hasKey("yAxis_Fine_Alignment_CRYSTAL_STAGE", "ERASE_CSV_CONTENT", conf1->getAlignmentSettingsConfigFilename(), pathToDir));
}

/**
 * @brief Test case that checks if the configuration file 'AlignmentSettins.ini' contains the keys to check the alignment in flipped orientation.
 * 
 */
TEST_F(TestReadCrystalParameters, read_checkAlignmentInFlippedOrientation_CRYSTAL_STAGE_parameters) {
    EXPECT_EQ(1, conf1->hasKey("checkAlignmentInFlippedOrientation_CRYSTAL_STAGE", "THRESHOLD ", conf1->getAlignmentSettingsConfigFilename(), pathToDir));
}

/**
 * @brief Test case that checks if the configuration file 'AlignmentSettins.ini' contains the keys to measure the bending angle of the crystal.
 * 
 */
TEST_F(TestReadCrystalParameters, read_Bending_Angle_CRYSTAL_STAGE_parameters) {
    EXPECT_EQ(1, conf1->hasKey("Bending_Angle_CRYSTAL_STAGE", "SCRIPT_NAME", conf1->getAlignmentSettingsConfigFilename(), pathToDir));
    EXPECT_EQ(1, conf1->hasKey("Bending_Angle_CRYSTAL_STAGE", "FILENAME_TO_BENDING_ANGLE", conf1->getAlignmentSettingsConfigFilename(), pathToDir));
    EXPECT_EQ(1, conf1->hasKey("Bending_Angle_CRYSTAL_STAGE", "FILENAME_TO_PEAKS", conf1->getAlignmentSettingsConfigFilename(), pathToDir));
    EXPECT_EQ(1, conf1->hasKey("Bending_Angle_CRYSTAL_STAGE", "STEP_SIZE_SCAN_HXP_Y", conf1->getAlignmentSettingsConfigFilename(), pathToDir));
    EXPECT_EQ(1, conf1->hasKey("Bending_Angle_CRYSTAL_STAGE", "RANGE_SCAN_HXP_Y", conf1->getAlignmentSettingsConfigFilename(), pathToDir));
    EXPECT_EQ(1, conf1->hasKey("Bending_Angle_CRYSTAL_STAGE", "STEP_SIZE", conf1->getAlignmentSettingsConfigFilename(), pathToDir));
    EXPECT_EQ(1, conf1->hasKey("Bending_Angle_CRYSTAL_STAGE", "RANGE", conf1->getAlignmentSettingsConfigFilename(), pathToDir));
    EXPECT_EQ(1, conf1->hasKey("Bending_Angle_CRYSTAL_STAGE", "DURATION_ACQUISITION", conf1->getAlignmentSettingsConfigFilename(), pathToDir));
    EXPECT_EQ(1, conf1->hasKey("Bending_Angle_CRYSTAL_STAGE", "DATA_LOG_FILENAME", conf1->getAlignmentSettingsConfigFilename(), pathToDir));
    EXPECT_EQ(1, conf1->hasKey("Bending_Angle_CRYSTAL_STAGE", "ERASE_CSV_CONTENT", conf1->getAlignmentSettingsConfigFilename(), pathToDir));
}
