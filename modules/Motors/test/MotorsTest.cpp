/**
 * @file MotorsTest.cpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2021
 * @brief
 * @version 0.1
 * @date 2021
 * 
 * @copyright © Copyright CERN 2018. All rights reserved. This software is released under a CERN proprietary software license.
 * Any permission to use it shall be granted in writing. Requests shall be addressed to CERN through mail-KT@cern.ch
 * 
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using testing::_;
using testing::Return;
using testing::NiceMock;
using testing::Invoke;
using ::testing::AtLeast;

struct HXPTests
    : public ::testing::Test {
        void SetUp() override {
            }

        void TearDown() override {
            }  
};
