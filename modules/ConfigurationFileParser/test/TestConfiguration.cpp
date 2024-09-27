/**
 * @file TestConfiguration.cpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2022
 * @brief Test methods of class 'Configuration'.
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
 * @brief Test fixture called 'TestConfiguration', which inherits from 'testing::Test'.
 * 
 */
struct TestConfiguration : public ::testing::Test {
    /**
     * @brief Create configuration file.
     * 
     */
    void SetUp() override {
        resultCreateConfigurationFile =
            conf1->createConfigurationFile("1.ini",
                                            fullPath);
    }
    /**
     * @brief Remove configuration file.
     * 
     */
    void TearDown() override {
        conf1->removeConfigurationFile("1.ini", fullPath);
    }

    std::shared_ptr<Configuration> conf1 =
        std::make_shared<Configuration>(); /**< Shared pointer to 'Configuration' Class. */

    std::filesystem::path currentPath = std::filesystem::current_path();
    std::filesystem::path projectName = "XRay_Machine";
    std::filesystem::path directoryName = "ConfigurationFiles";
    std::filesystem::path fullPath =
        conf1->getPartialPathUntilKeyName(currentPath,
                                          projectName) /= directoryName;

    int resultCreateConfigurationFile;
};

/**
 * @brief Test case that checks if the file 1.ini exits.
 * 
 */
TEST_F(TestConfiguration, CreateConfigurationFile) {
    EXPECT_EQ(1, conf1->hasFile("1.ini", fullPath));
}

/**
 * @brief Test case that checks if the file 1.ini has been created, writes on it and reads the value written.
 * 
 */
TEST_F(TestConfiguration, Check_if_Configuration_has_section_and_key) {
    EXPECT_EQ(1, resultCreateConfigurationFile);
    int defaultPort = 5001;
    std::string strPort = std::to_string(defaultPort);
    EXPECT_EQ(1,
              conf1->writeConfigurationFile(strPort,
                                            "Hexapod",
                                            "nPort",
                                            "1.ini",
                                            fullPath));
    EXPECT_EQ(1, conf1->hasSection("Hexapod", "1.ini", fullPath));
    EXPECT_EQ(1, conf1->hasKey("Hexapod", "nPort", "1.ini", fullPath));
}


