/**
 * @file ScanningHXP.hpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2022
 * @brief Implementation of Class ISCanning.
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

#include "IHXP.hpp"
#include "IMotor.hpp"
#include "IScanning.hpp"
#include "ISensors.hpp"
#include "IPostProcessing.hpp"

using namespace sensors;  // NOLINT

namespace scanning {

/**
 * @class ScanningHXP
 * @brief Implementation of Class ISCanning.
 * 
 */
class ScanningHXP : public IScanning{
 public:
  /**
   * @brief Construct a new ScanningHXP object.
   * 
   * @param clientHxp shared pointer to IHXP Class.
   * @param clientSensors shared pointer to ISensors Class.
   * @param clientPostProcessing shared pointer to IPostProcessing Class.
   */
  ScanningHXP(std::shared_ptr<IHXP> clientHxp,
              std::shared_ptr<sensors::ISensors> clientSensors,
              std::shared_ptr<IPostProcessing> clientPostProcessing);
  /**
   * @brief Destroy the ScanningHXP object.
   * 
   */
  ~ScanningHXP();
  bool scan() override;
  bool scanRelative() override;
  float getStepSize() override;
  void setStepSize(float stepSize) override;
  int getDurationAcquisition() override;
  void setDurationAcquisition(int durationAcquisition) override;
  std::string getFilename() override;
  void setFilename(std::string filename) override;
  bool getEraseCsvContent() override;
  void setEraseCsvContent(bool eraseCsvContent) override;
  float getRange() override;
  void setRange(float range) override;
  bool stop() override;
  void setShowPlot(bool showPlot) override;
  bool getShowPlot() override;
  void setupAlignmentParameters(float stepSize,
                                float range,
                                int durationAcquisition,
                                std::string filename,
                                bool eraseCsvContent,
                                bool showPlot) override;
  bool checkReachingPosition(float currentPosition,
                             float targetPosition) override;
  /**
   * @brief This method sets the axis target position of the hexapod based on the
   * selected axis that is currently scanning ('hxpAxisToScan_').
   * 
   * @param position target position to set.
   */
  void updateAxisPosition(double position);
  /**
   * @brief This method gets the actual axis position of the hexapod based on the
   * selected axis that is currently scanning ('hxpAxisToScan_').
   * 
   * @return double representing the position of the axis.
   */
  double getAxisPosition();
  /**
   * @brief Replaces spaces in the input filename with underscores.
   *
   * This method takes a `std::string` input `filename` and substitutes any spaces
   * it contains with underscores ('_'). The original filename remains unchanged,
   * and the modified filename is returned as a new `std::string`.
   *
   * @param filename The input filename that may contain spaces.
   * @return A new `std::string` with spaces replaced by underscores.
   *
   * @note The input `filename` is not modified by this method.
   * @warning This method assumes that the input `filename` is a valid `std::string`.
   *          Calling this method with an invalid string may lead to undefined behavior.
   *
   * @code
   * // Example Usage:
   * std::string filename = "example file name.txt";
   * std::string modifiedFilename = replaceSpacesWithUnderscores(filename);
   * std::cout << "Original filename: " << filename << std::endl;
   * std::cout << "Modified filename: " << modifiedFilename << std::endl;
   * @endcode
   */
  std::string replaceSpacesWithUnderscores(const std::string& filename);
  /**
   * @brief Checks the extension of a file name and ensures it has a ".csv" extension.
   *
   * This method checks the extension of the given file name and ensures that it ends with
   * the ".csv" extension. If the file name already has the ".csv" extension, it is returned
   * as is. If not, the method appends the ".csv" extension to the file name and returns the
   * updated file name.
   *
   * @param filename The file name to check the extension for.
   * @return std::string The updated file name with ".csv" extension if needed.
   *
   * @note The method assumes that the file name has a minimum length of 4 characters to avoid
   * potential undefined behavior.
   * @note The method is case-sensitive and will not convert the extension to lowercase.
   *
   * @code
   * // Example Usage:
   * std::string filename = "data";
   * std::string updatedFilename = checkExtension(filename); // Returns "data.csv"
   * @endcode
   */
  std::string checkExtension(std::string filename);
  /**
   * @brief Moves the Hexapod along a specified axis by a given displacement.
   *
   * This function controls the Hexapod's movement by sending an incremental 
   * movement command to the specified axis. The movement can be either 
   * linear or rotational depending on the axis being controlled.
   *
   * @param displacement The amount of displacement to move along the specified axis.
   *                     The unit of displacement depends on the Hexapod's configuration.
   *
   * @return true if the motion command was successful, false otherwise.
   *
   * @note The axis to be controlled is determined by the member variable `hxpAxisToScan_`.
   *       - Axis 1, 2, 3 correspond to linear movements along the X, Y, and Z axes, respectively.
   *       - Axis 4, 5, 6 correspond to rotational movements about the U, V, and W axes, respectively.
   * @note If an invalid axis is specified, the function logs an error and returns false.
   */
  bool relativeMotionHXP(double displacement);
 private:
  std::shared_ptr<IHXP> clientHxp_;  /**< shared pointer to IHXP Class.*/
  std::shared_ptr<sensors::ISensors> clientSensors_;  /**< shared pointer to ISensor Class.*/
  std::shared_ptr<IPostProcessing> clientPostProcessing_;  /**< shared pointer to IPostProcessing Class.*/
  std::filesystem::path pathToProjDirectory_ = clientSensors_->getPathToProjDirectory();
  std::filesystem::path pathToPlotScanScript_ = pathToProjDirectory_ / "modules\\Devices\\scripts\\Scans\\plotScanHXP.py";
  float stepSize_;  /**< Step size of the scan. */
  int durationAcquisition_;  /**< Acquisition time (in seconds) between steps of a scan. */
  float range_;  /**< Range of the scan. */
  std::string filename_;  /**< Filename where the data logged by the sensors will be saved. */
  bool eraseCsvContent_;  /**< Boolean flag used to control if the data registered by the xray sensor will not be saved at the end of the scan.*/
  bool stopMotor_;  /**< Boolean flag used to control if the stop command has been called. */
  bool showPlot_;  /**< Boolean flag used to control whether to show the plot at the end of the scan or not. */
};

}  // namespace scanning
