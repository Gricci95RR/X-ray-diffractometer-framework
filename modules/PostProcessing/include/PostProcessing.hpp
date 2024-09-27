/**
 * @file PostProcessing.hpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2022
 * @brief Implementation of Class @ref IPostProcessing.
 * @version 0.1
 * @date 2022
 * 
 * @copyright © Copyright CERN 2018. All rights reserved. This software is released under a CERN proprietary software license.
 * Any permission to use it shall be granted in writing. Requests shall be addressed to CERN through mail-KT@cern.ch
 * 
 */

#pragma once

#include <spdlog/spdlog.h>

#include <string>
#include <cassert>
#include <memory>
#include <filesystem>

#include "IPostProcessing.hpp"

/**
 * @class PostProcessing
 * @brief Implementation of Class @ref IPostProcessing.
 * 
 */
class PostProcessing : public IPostProcessing {
 public:
  /**
   * @brief Construct a new Post Processing object.
   * 
   */
  explicit PostProcessing();
  /**
   * @brief Destroy the Post Processing object.
   * 
   */
  ~PostProcessing();

  void executeScript1(std::string pathToPythonScript, 
                      std::string argument1) override;

  void executeScript2(std::string pathToPythonScript, 
                      std::string argument1,
                      std::string argument2) override;

  void executeScript5(std::string pathToPythonScript, 
                      std::string argument1,
                      std::string argument2,
                      std::string argument3,
                      std::string argument4,
                      std::string argument5) override;

  void executeScript6(std::string pathToPythonScript, 
                      std::string argument1,
                      std::string argument2,
                      std::string argument3,
                      std::string argument4,
                      std::string argument5,
                      std::string argument6) override;

  void executeScript7(std::string pathToPythonScript, 
                      std::string argument1,
                      std::string argument2,
                      std::string argument3,
                      std::string argument4,
                      std::string argument5,
                      std::string argument6,
                      std::string argument7) override;

 private:
  
};
