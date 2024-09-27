/**
 * @file ISensors.hpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2022
 * @brief Interface used to control the XRaySensor Device logging.
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
#include <filesystem>

namespace sensors {

/**
 * @class ISensors
 * @brief Interface used to control the XRaySensor Device logging.
 * 
 */
class ISensors {
 public:
  ~ISensors() = default;

  /**
  * @brief Start the .csv acquisition of sensor data and position for one stepper motor.
  * @details
  * 1. Any previously opened file stream is closed using the close() member function;
  * 2. A new file stream is then opened using the open() member function with the
  *    std::ofstream::app flag, which opens the file in append mode;
  * 3. If flushFlag is true, the flushCsv() method of the Sensors class is called;
  * 4. "X-Ray Sensor Data" is written to the file followed by a column separator,
  *    then "Stepper Motor Position" is written to the file followed by another
  *    column separator. Finally, a new line is written to end the row.
  * @param filename name of the .csv file where to log data. Note: always add the extension (i.e. ".csv") at the end of the filename.
  * @param eraseCsvContent flag used to erase the content of the .csv file. If true the content of the file will be erased. If false the new data will be appendend.
  */
  virtual void startAcquisitionSingleStepper(std::string filename, bool eraseCsvContent) = 0;

  /**
  * @brief Start the .csv acquisition of sensor data, position for one stepper motor and 6 axis positions for Hexapod Robot.
  * @details 
  * 1. Any previously opened file stream is closed using the close() member function;
  * 2. A new file stream is then opened using the open() member function with the
  *    std::ofstream::app flag, which opens the file in append mode;
  * 3. If flushFlag is true, the flushCsv() method of the Sensors class is called;
  * 4. "X-Ray Sensor Data" is written to the file followed by a column separator,
  *    then all the HXP Axis positions are written to the file followed by another
  *    column separator. Finally, a new line is written to end the row.
  * @param filename name of the .csv file. Note: always add the extension (i.e. ".csv") at the end of the filename.
  * @param eraseCsvContent flag used to erase the content of the .csv file. If true the content of the file will be erased. If false the new data will be appendend.
  */
  virtual void startAcquisitionCrystal(std::string filename, bool eraseCsvContent) = 0;

  /**
  * @brief Read X-Ray sensor and saves data read and position of the steppr motor in the .csv file.
  * @param durationAcquisition amount of time used for the acquisition of data.
  * @param position of the stepper motor.
  */
  virtual std::string readXRaySensor(int durationAcquisition, float position) = 0;

  /**
  * @brief Read X-Ray sensor and saves data read and positions of the hexapod axis in the .csv file.
  * @param durationAcquisition amount of time used for the acquisition of data.
  * @param positionX position of X-axis.
  * @param positionY position of Y-axis.
  * @param positionZ position of Z-axis.
  * @param positionU position of U-axis.
  * @param positionV position of V-axis.
  * @param positionW position of W-axis.
  */
  virtual std::string readXRaySensor(int durationAcquisition, float positionX, float positionY, float positionZ, float positionU, float positionV, float positionW) = 0;

  /**
  * @brief Deinitialize X-Ray sensor.
  */
  virtual void deinitializeXRaySensor() = 0;

  /**
  * @brief Implements a delay of variable length.
  * @note Delay required to stabilize the motor after moving in order to increase the accuracy of measurement.
  * @param timerLength length of the delay.
  */
  virtual void motionStabilizationTimer(int timerLength) = 0;

  /**
  * @brief Flushes .csv file used for X-Ray Sensor data logging.
  */
  virtual void flushCsv(std::string pathToCsv) = 0;

  /**
  * @brief Read float from .csv file.
  * @param pathToFile.
  */
  virtual float readCsvResult(std::string pathToFile) = 0;

  /**
  * @brief Getter funtion of "pathToProjDirectory_" representing the path to project directory.
  */
  virtual std::filesystem::path getPathToProjDirectory() = 0;
};

}  // namespace sensors
