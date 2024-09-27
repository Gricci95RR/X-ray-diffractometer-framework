/**
 * @file CrystalDeviceController.hpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2022
 * @brief Implementation Class of the interface 'ICrystalDeviceController'.
 * @version 0.1
 * @date 2022
 * 
 * @copyright © Copyright CERN 2018. All rights reserved. This software is released under a CERN proprietary software license.
 * Any permission to use it shall be granted in writing. Requests shall be addressed to CERN through mail-KT@cern.ch
 * 
 */

#pragma once

#include <sml.hpp>
#include <spdlog/spdlog.h>
#include "spdlog/sinks/rotating_file_sink.h"

#include <string>
#include <cassert>
#include <memory>
#include <filesystem>

#include "ICrystalDeviceController.hpp"
#include "Crystal/System.hpp"
#include "Crystal/CrystalStatus.hpp"

using namespace scanning;  // NOLINT

namespace crystal {

/**
 * @class CrystalDeviceController
 * @brief Implementation Class of the interface 'ICrystalDeviceController'.
 * 
 */
class CrystalDeviceController : public ICrystalDeviceController {
 public:
  explicit CrystalDeviceController(std::filesystem::path logFilePath,
                                   std::shared_ptr<IHXP> clientHXP,
                                   std::shared_ptr<IMotor> clientMotor,
                                   std::shared_ptr<scanning::IScanning> clientScanningHXP,
                                   std::shared_ptr<scanning::IScanning> clientScanningStepper,
                                   std::shared_ptr<sensors::ISensors> clientSensors,
                                   std::shared_ptr<IConfiguration> clientConfiguration,
                                   std::shared_ptr<IPostProcessing> clientPostProcessing);
  ~CrystalDeviceController();
  bool start() override;
  bool stop() override;
  bool disconnect() override;
  bool goHome() override;
  float getPositionStepper() override;
  double getPositionX() override;
  double getPositionY() override;
  double getPositionZ() override;
  double getPositionU() override;
  double getPositionV() override;
  double getPositionW() override;
  bool logger(std::string filename) override;
  bool moveToAbsPosition(double CoordX,
                         double CoordY,
                         double CoordZ,
                         double CoordU,
                         double CoordV,
                         double CoordW) override;
  bool scanHxp(int axisToScan,
               float stepSize,
               float range,
               int durationAcquisition,
               std::string filename,
               bool eraseCsvContent,
               bool showPlot) override;
  bool scanStepper(float stepSize,
                   float range,
                   int durationAcquisition,
                   std::string filename,
                   bool eraseCsvContent,
                   bool showPlot) override;
  bool moveToPositionStepper(float position) override;
  std::string getFsmState() override;
  /**
   * @brief This method returns the current status of the Crystal device.
   * 
   * @details first checks the current state of the state machine associated with the device using Boost.SML library.
   * Depending on the current state of the state machine, the method sets the corresponding CrystalStatus.
   * If the state of the state machine is not recognized or defined, the method sets the status to "NotInitialized".
   * 
   * @return CrystalStatus 
   */
  CrystalStatus getStatus();
  bool alignSourceWithSensor() override;
  bool xAxisAlignmentCrystal() override;
  bool yAxisAlignmentCrystal() override;
  bool zAxisAlignmentCrystal() override;
  bool yWAxesAlignmentCrystal() override;
  bool xAxisFineAlignmentCrystal() override;
  bool braggPeakSearchCrystal() override;
  bool yAxisFineAlignmentCrystal() override;
  bool checkAlignmentInFlippedOrientation() override;
  bool bendingAngleMeasurement() override;
  bool miscutAngleMeasurement(bool repeateBendingAngleMeas) override;
  bool torsionAngleMeasurement() override;

 private:
  std::filesystem::path logFilePath_;  /**< Path to the log file directory. */
  CrystalStatus crystalStatus_;  /**< Object of type 'CrystalStatus'*/
  std::shared_ptr<Actions> actionsPtr_;  /**< Shared pointer to Actions Class. */
  std::shared_ptr<IHXP> clientHxp_;  /**< Shared pointer to IHXP Class. */
  std::shared_ptr<IMotor> clientStepper_;  /**< Shared pointer to IMotor Class. */
  std::shared_ptr<scanning::IScanning> clientScanningHxp_;  /**< Shared pointer to IScanning Class. */
  std::shared_ptr<scanning::IScanning> clientScanningStepper_;  /**< Shared pointer to IScanning Class. */
  std::shared_ptr<IConfiguration> clientConfiguration_;  /**< Shared pointer to IConfiguration Class. */
  boost::sml::sm<System> sMachine_;  /**<  State machine object. */
};

}  // namespace crystal
