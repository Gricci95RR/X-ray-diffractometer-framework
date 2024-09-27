/**
 * @file TestReadXRaySourceParameters.cpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2022
 * @brief Test that checks if the configuration file 'AlignmentSettings.ini' contains all the required keys to perform the X-Ray Source alignment.
 * @version 0.1
 * @date 2022
 * 
 * @copyright © Copyright CERN 2018. All rights reserved. This software is released under a CERN proprietary software license.
 * Any permission to use it shall be granted in writing. Requests shall be addressed to CERN through mail-KT@cern.ch
 * 
 */

#include <gtest/gtest.h>

#include "Configuration.hpp"

struct TestReadXRaySourceParameters : public ::testing::Test {
    std::filesystem::path currentPath = std::filesystem::current_path();
    std::filesystem::path projectName = "XRay_Machine";
    std::filesystem::path directoryName = "ConfigurationFiles";
    std::filesystem::path filePath;

    std::shared_ptr<Configuration> conf;
    std::filesystem::path pathToDir;

    void SetUp() override {
        conf = std::make_shared<Configuration>();
        pathToDir = conf->getPartialPathUntilKeyName(currentPath, projectName) /= directoryName;
    }

    void TearDown() override {
    }
};

/**
 * @brief Test case that checks if the configuration file 'AlignmentSettings.ini' contains the keys to align the X-Ray source.
 * 
 */
TEST_F(TestReadXRaySourceParameters, ReadAlignmentParameters) {
    EXPECT_EQ(1, conf->hasKey("XRAY_SOURCE_STAGE_ROTATIONAL", "SCRIPT_NAME", conf->getAlignmentSettingsConfigFilename(), pathToDir));
    EXPECT_EQ(1, conf->hasKey("XRAY_SOURCE_STAGE_ROTATIONAL", "FILENAME_TO_ALIGNMENT_POSITION", conf->getAlignmentSettingsConfigFilename(), pathToDir));
    EXPECT_EQ(1, conf->hasKey("XRAY_SOURCE_STAGE_ROTATIONAL", "STEP_SIZE", conf->getAlignmentSettingsConfigFilename(), pathToDir));
    EXPECT_EQ(1, conf->hasKey("XRAY_SOURCE_STAGE_ROTATIONAL", "RANGE", conf->getAlignmentSettingsConfigFilename(), pathToDir));
    EXPECT_EQ(1, conf->hasKey("XRAY_SOURCE_STAGE_ROTATIONAL", "DURATION_ACQUISITION", conf->getAlignmentSettingsConfigFilename(), pathToDir));
    EXPECT_EQ(1, conf->hasKey("XRAY_SOURCE_STAGE_ROTATIONAL", "DATA_LOG_FILENAME", conf->getAlignmentSettingsConfigFilename(), pathToDir));
    EXPECT_EQ(1, conf->hasKey("XRAY_SOURCE_STAGE_ROTATIONAL", "ERASE_CSV_CONTENT", conf->getAlignmentSettingsConfigFilename(), pathToDir));
}
