/**
 * @file TestReadMonochromatorParameters.cpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2022
 * @brief Test that checks if the configuration file 'AlignmentSettings.ini' contains all the required keys to perform the monochromator alignments.
 * @version 0.1
 * @date 2022
 * 
 * @copyright © Copyright CERN 2018. All rights reserved. This software is released under a CERN proprietary software license.
 * Any permission to use it shall be granted in writing. Requests shall be addressed to CERN through mail-KT@cern.ch
 * 
 */

#include <gtest/gtest.h>

#include "Configuration.hpp"

/**
 * @brief Test fixture called 'TestReadMonochromatorParameters', which inherits from 'testing::Test'.
 * 
 */
struct TestReadMonochromatorParameters : public ::testing::Test {
    std::filesystem::path currentPath = std::filesystem::current_path();
    std::filesystem::path projectName = "XRay_Machine";
    std::filesystem::path directoryName = "ConfigurationFiles";
    std::filesystem::path filePath;

    std::shared_ptr<IConfiguration> conf;
    std::filesystem::path pathToDir;

    void SetUp() override {
        conf = std::make_shared<Configuration>();
        pathToDir = conf->getPartialPathUntilKeyName(currentPath, projectName);
        pathToDir /= directoryName;
    }

    void TearDown() override {
    }
};

/**
 * @brief Test case that checks if the configuration file 'config.ini' contains the key 'IpAdress'.
 * 
 */
TEST_F(TestReadMonochromatorParameters, ReadStepperIP) {
    EXPECT_EQ(1, conf->hasKey("STEPPER_IP", "IpAdress", conf->getConfigFilename(), pathToDir));
}
