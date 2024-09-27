/**
 * @file PostProcessingMockConfiguration.hpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2022
 * @brief Configuration class used to set the behavior of the mock object.
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
#include <spdlog/spdlog.h>

#include <memory>

#include "PostProcessingMock.hpp"

using testing::_;
using testing::Return;
using testing::NiceMock;
using testing::Invoke;
using ::testing::AtLeast;

/**
 * @class PostProcessingMockConfiguration
 * @brief Configuration class used to set the behavior of the mock object.
 * 
 * The PostProcessingMockConfiguration class sets default values for the mocked methods that will be called during the tests.
 * 
 */
class PostProcessingMockConfiguration {
 public:
  PostProcessingMockConfiguration() {
    PostProcessingMock_.reset(new NiceMock<PostProcessingMock>);
  }
  ~PostProcessingMockConfiguration() {}
  std::shared_ptr<NiceMock<PostProcessingMock>> getMock() {
        return PostProcessingMock_;
  }
  void configurePostProcessingMock() {
    ON_CALL(*PostProcessingMock_, executeScript1(_, _)).WillByDefault(Invoke(
      [this] {
          spdlog::info("Method executeScript1 of Class configurePostProcessingMock\n");
      }));
    ON_CALL(*PostProcessingMock_, executeScript2(_, _, _)).WillByDefault(Invoke(
      [this] {
          spdlog::info("Method executeScript2 of Class configurePostProcessingMock\n");
      }));
    ON_CALL(*PostProcessingMock_, executeScript5(_, _, _, _, _, _)).WillByDefault(Invoke(
      [this] {
          spdlog::info("Method executeScript5 of Class configurePostProcessingMock\n");
      }));
    ON_CALL(*PostProcessingMock_, executeScript6(_, _, _, _, _, _, _)).WillByDefault(Invoke(
      [this] {
          spdlog::info("Method executeScript6 of Class configurePostProcessingMock\n");
      }));
    ON_CALL(*PostProcessingMock_, executeScript7(_, _, _, _, _, _, _, _)).WillByDefault(Invoke(
      [this] {
          spdlog::info("Method executeScript7 of Class configurePostProcessingMock\n");
      }));
  }

 private:
  std::shared_ptr<NiceMock<PostProcessingMock>> PostProcessingMock_;  /**< Shared pointer to PostProcessingMock Class*/
};
