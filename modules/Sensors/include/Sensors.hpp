/**
 * @file Sensors.hpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2022
 * @brief Implementation of Class ISensors.
 * @version 0.1
 * @date 2022
 * 
 * @copyright © Copyright CERN 2018. All rights reserved. This software is released under a CERN proprietary software license.
 * Any permission to use it shall be granted in writing. Requests shall be addressed to CERN through mail-KT@cern.ch
 * 
 */

#pragma once

#include <spdlog/spdlog.h>

#include <iostream>
#include <string>
#include <memory>
#include <chrono>
#include <thread>
#include <fstream>
#include <iterator>
#include <sstream>
#include <vector>

#include "ISensors.hpp"
#include "XRaySensor.hpp"

namespace sensors {

/**
 * @class Sensors
 * @brief Implementation of Class ISensors.
 * 
 */
class Sensors : public ISensors{
 public:
  /**
   * @brief Construct a new Sensors object.
   * 
   * @param pathToProjDirectory 
   * @param xRaySensorName 
   * @param baudrate 
   */
  Sensors(std::filesystem::path pathToProjDirectory);
  ~Sensors();
  void startAcquisitionSingleStepper(std::string filename, bool eraseCsvContent) override;
  void startAcquisitionCrystal(std::string filename, bool eraseCsvContent) override;
  /**
   * @brief Save the position of the stepper motor.
   * @param position position of the stepper motor.
   */
  void savePosition(float position);
  /**
   * @brief Save the position of the stepper motor and HXP axis.
   * @param positionX X-axis coordinate of HXP.
   * @param positionY Y-axis coordinate of HXP.
   * @param positionZ Z-axis coordinate of HXP.
   * @param positionU U-axis coordinate of HXP.
   * @param positionV V-axis coordinate of HXP.
   * @param positionW W-axis coordinate of HXP.
   */
  void savePosition(float positionX, float positionY, float positionZ, float positionU, float positionV, float positionW);
  std::string readXRaySensor(int durationAcquisition, float position) override;
  std::string readXRaySensor(int durationAcquisition, float positionX, float positionY, float positionZ, float positionU, float positionV, float positionW) override;
  void deinitializeXRaySensor() override;
  void motionStabilizationTimer(int timerLength) override;
  /**
   * @brief Writes on a .csv file the data read by the X-Ray Sensor.
   * @param data data read by the X-Ray Sensor.
   */
  void writeDataToCsv(std::string data);
  void flushCsv(std::string pathToCsv) override;
  /**
   * @brief Writes "\n" character in the .csv file.
   */
  void endRow();
  /**
   * @brief Writes ";" character in the .csv file.
   * @details Used to separate colums facilitating the reading of the csv file in python scripts.
   */
  void separator();
  float readCsvResult(std::string pathToFile) override;
  std::filesystem::path getPathToProjDirectory() override;
  /**
   * @brief String splitting function that takes an input string and
   * a delimiter character and returns a vector of the substrings
   * that are separated by the delimiter.
   * 
   * @param s string to split.
   * @param delimiter delimiter used to split the string.
   * @return std::vector<std::string> of substrigs splitted.
   */
  std::vector<std::string> split(const std::string& s, char delimiter);

 private:
  std::shared_ptr<IXRaySensor> clientXRaySensor_;  /**< Shared pointer to IXRaySensor Class. */
  std::ofstream fs_;  /**< Declaration of type 'std::ofstream' ( standard C++ class that provides an interface to write data to a file). */
  std::string xRaySensorName_;  /**< Parameter that stores the name of the XRaySensor (name stored in configuration file). */
  unsigned int baudrate_;  /**< Baudrate (i.e. bits per second at which bits are transmitted) of the serial communication. */
  std::string pathToCsv_;  /**< Path to the .csv file where to log the data. */
  std::filesystem::path pathToProjDirectory_;
  std::string pathToDirectoryLogFiles_;  /**< Path to the directory where the log files need to be saved. */
};

}  // namespace sensors
