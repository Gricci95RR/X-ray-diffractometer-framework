/**
 * @file IScanning.hpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2023
 * @brief Interface used to control the scan of a device moved by a stepper motor or an hexapod robot.
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

namespace scanning {

/**
 * @class IScanning
 * @brief Interface used to control the scan of a device moved by a stepper motor or an hexapod robot.
 * 
 */
class IScanning {
 public:
  ~IScanning() = default;
  /**
   * @brief This method is used to start a scan
   * (i.e. motion of the stepper motor device or axis of the hexapod in steps in a range).
   * 
   * @note The scan can be performed in forward (towards positive axis values)
   * or backward motion (towards negative axis values). 
   * The scan in forward motion is performed if the final position of the scan
   * is greater than the current position (finalPosition = currentPosition + range). 
   * So that if the range is negative the scan is performed in backward motion.
   * In forward motion otherwise.
   * 
   * @note Before calling this method the 'setupAlignmentParameters' method must be called.
   * 
   * @example Scan
   * To start a scan of the Hexapod X-Axis:
   * clientScanningHxp_->hxpAxisToScan_ = this->hxpAxis::X;  // Set the hexapod axis to scan.
   * clientScanningHxp_->setupAlignmentParameters(0.1, 1, 100, log.csv, true, true);  // To setup the alignment params.
   * clientScanningHxp_->scan();  // To start the scan with the parameters set before.
   * 
   * @example Scan2
   * To start a scan of a stepper motor:
   * clientScanningStepper_->setupAlignmentParameters(0.1, 1, 100, log.csv, true, true);  // Tto setup the alignment params.
   * clientScanningStepper_->scan();  // To start the scan with the parameters set before.
   * 
   * @return true if the scan has been completed succesfully.
   * @return false otherwise.
   */
  virtual bool scan() = 0;
  virtual bool scanRelative() = 0;
  /**
   * @brief Getter function of the 'stepSize_' parameter.
   * 
   * @return float representing the step size of the scan.
   */
  virtual float getStepSize() = 0;
  /**
   * @brief Setter function of the parameter 'stepSize_'.
   * 
   * @param stepSize float
   */
  virtual void setStepSize(float stepSize) = 0;
  /**
   * @brief Getter function of the 'durationAcquisition_' parameter.
   * 
   * @return int representing the duration of acquisition of the scan (in ms).
   */
  virtual int getDurationAcquisition() = 0;
  /**
   * @brief Setter function of the parameter 'durationAcquisition_'.
   * 
   * @param durationAcquisition int
   */
  virtual void setDurationAcquisition(int durationAcquisition) = 0;
  /**
   * @brief Getter function of the 'filename_' parameter.
   * 
   * @return std::string representing the name of the file where to log the data.
   */
  virtual std::string getFilename() = 0;
  /**
   * @brief Setter function of the parameter 'filename_'
   * 
   * @param filename std::string
   */
  virtual void setFilename(std::string filename) = 0;
  /**
   * @brief Getter function of the 'eraseCsvContent_' parameter.
   * 
   * @return bool.
   */
  virtual bool getEraseCsvContent() = 0;
  /**
   * @brief Setter function of the parameter 'eraseCsvContent_'.
   * 
   * @param eraseCsvContent bool
   */
  virtual void setEraseCsvContent(bool eraseCsvContent) = 0;
  /**
   * @brief Getter function of the 'range_' parameter.
   * 
   * @return float representing the range of the scan.
   */
  virtual float getRange() = 0;
  /**
   * @brief Setter function of the parameter 'range_'.
   * 
   * @param range float
   */
  virtual void setRange(float range) = 0;
  /**
   * @brief Method that toggles the parameter 'stopMotor_' to true.
   * 
   * @return true
   */
  virtual bool stop() = 0;
  /**
   * @brief Setter function of the parameter 'showPlot'
   * 
   * @param showPlot bool
   */
  virtual void setShowPlot(bool showPlot) = 0;
  /**
   * @brief Getter function of the 'showPlot' parameter.
   * 
   * @return bool
   */
  virtual bool getShowPlot() = 0;
  /**
   * @brief This method is used to setup the parameters of a scan.
   * @param stepSize step size of the scan.
   * @param range range of the scan if the range is negative the scan is performed in backward motion (towards negative axis values),
   * in forward motion (towards positive axis values) otherwise.
   * @param durationAcquisition amount of time that the device needs to stop in order to read the x-ray detector.
   * @param filename name of the file where to log the data. 
   * @param eraseCsvContent if true the data registered by the xray sensor will not be saved at the end of the scan.
   * @param showPlot boolean flag, if true at the end of the scan the data registered by the sensor will be plotted.
   * @details  If the last 4 characters of the input parameter "filename" are different from
   * ".csv" the extension will be added to the name of the file;
   * @note filename must finish with the file extention ".csv".
   */
  virtual void setupAlignmentParameters(float stepSize,
                                        float range,
                                        int durationAcquisition,
                                        std::string filename,
                                        bool eraseCsvContent,
                                        bool showPlot) = 0;
  /**
   * @brief This method checks if the target position after a motion of the device has been reached.
   * 
   * @details The function checks if the difference between the target position and the current position
   * is greater than '0.1'.
   * 
   * @param currentPosition position of the device (or axis of the hexapod) after the motion.
   * @param targetPosition target position set before the motion.
   * @return true if the difference between the target position and the current position
   * is less than '0.1'.
   * @return false difference between the target position and the current position
   * is greater than '0.1'.
   */
  virtual bool checkReachingPosition(float currentPosition,
                                     float targetPosition) = 0;
  int hxpAxisToScan_;  /**< Integer value representing the axis to scan. The value of this parameter must be within 0-6. */
};

}  // namespace scanning
