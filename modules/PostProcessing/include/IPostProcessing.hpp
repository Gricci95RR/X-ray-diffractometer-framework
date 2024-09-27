/**
 * @file IPostProcessing.hpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2022
 * @brief Interface used for post processing python script execution.
 * @version 0.1
 * @date 2022
 * 
 * @copyright © Copyright CERN 2018. All rights reserved. This software is released under a CERN proprietary software license.
 * Any permission to use it shall be granted in writing. Requests shall be addressed to CERN through mail-KT@cern.ch
 * 
 */

#pragma once

#include <iostream>
#include <string>

/**
 * @class IPostProcessing
 * @brief Interface used for post processing python script execution.
 * 
 */
class IPostProcessing {  // Interface
 public:

   ~IPostProcessing() = default;

  /**
   * @brief The method executes the Python script by creating a command using the provided path to
   * the Python script and the argument, and calling it with the system function.
   * 
   * @param pathToPythonScript - A string representing the path to the Python script.
   * @param argument1 - A string representing the argument to be passed to the Python script.
   */
   virtual void executeScript1(std::string pathToPythonScript, 
                               std::string argument1) = 0;

  /**
   * @brief The method executes the Python script by creating a command using the provided path to the Python script and the
   * arguments, and calling it with the system function.
   * 
   * @param pathToPythonScript - A string representing the path to the Python script.
   * @param argument1 - A string representing the argument to be passed to the Python script.
   * @param argument2 - A string representing the argument to be passed to the Python script.
   */
   virtual void executeScript2(std::string pathToPythonScript, 
                               std::string argument1,
                               std::string argument2) = 0;
  /**
   * @brief The method executes the Python script by creating a command using the provided path to the Python script and the
   * arguments, and calling it with the system function.
   * 
   * @param pathToPythonScript - A string representing the path to the Python script.
   * @param argument1 - A string representing the argument to be passed to the Python script.
   * @param argument2 - A string representing the argument to be passed to the Python script.
   * @param argument3 - A string representing the argument to be passed to the Python script.
   * @param argument4 - A string representing the argument to be passed to the Python script.
   * @param argument5 - A string representing the argument to be passed to the Python script.
   */
   virtual void executeScript5(std::string pathToPythonScript, 
                               std::string argument1,
                               std::string argument2,
                               std::string argument3,
                               std::string argument4,
                               std::string argument5) = 0;
   
  /**
   * @brief The method executes the Python script by creating a command using the provided path to the Python script and the
   * arguments, and calling it with the system function.
   * 
   * @param pathToPythonScript - A string representing the path to the Python script.
   * @param argument1 - A string representing the argument to be passed to the Python script.
   * @param argument2 - A string representing the argument to be passed to the Python script.
   * @param argument3 - A string representing the argument to be passed to the Python script.
   * @param argument4 - A string representing the argument to be passed to the Python script.
   * @param argument5 - A string representing the argument to be passed to the Python script.
   * @param argument6 - A string representing the argument to be passed to the Python script.
   */
   virtual void executeScript6(std::string pathToPythonScript, 
                               std::string argument1,
                               std::string argument2,
                               std::string argument3,
                               std::string argument4,
                               std::string argument5,
                               std::string argument6) = 0;
  /**
   * @brief The method executes the Python script by creating a command using the provided path to the Python script and the
   * arguments, and calling it with the system function.
   * 
   * @param pathToPythonScript - A string representing the path to the Python script.
   * @param argument1 - A string representing the argument to be passed to the Python script.
   * @param argument2 - A string representing the argument to be passed to the Python script.
   * @param argument3 - A string representing the argument to be passed to the Python script.
   * @param argument4 - A string representing the argument to be passed to the Python script.
   * @param argument5 - A string representing the argument to be passed to the Python script.
   * @param argument6 - A string representing the argument to be passed to the Python script.
   * @param argument7 - A string representing the argument to be passed to the Python script.
   */
   virtual void executeScript7(std::string pathToPythonScript, 
                               std::string argument1,
                               std::string argument2,
                               std::string argument3,
                               std::string argument4,
                               std::string argument5,
                               std::string argument6,
                               std::string argument7) = 0;
};
