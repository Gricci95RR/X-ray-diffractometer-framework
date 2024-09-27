/**
 * @file Crystal/Actions.hpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2022
 * @brief Class that allows to define the behavior of the Crystal device in response to specific events during state transitions of the FSM.
 * @version 0.1
 * @date 2022
 * 
 * @copyright © Copyright CERN 2018. All rights reserved. This software is released under a CERN proprietary software license.
 * Any permission to use it shall be granted in writing. Requests shall be addressed to CERN through mail-KT@cern.ch
 * 
 */

#ifndef MODULES_DEVICES_INCLUDE_CRYSTAL_ACTIONS_HPP_
#define MODULES_DEVICES_INCLUDE_CRYSTAL_ACTIONS_HPP_

#include <spdlog/spdlog.h>

#include <memory>

#include "IHXP.hpp"
#include "IMotor.hpp"
#include "IScanning.hpp"
#include "ISensors.hpp"
#include "IConfiguration.hpp"
#include "IPostProcessing.hpp"

using namespace scanning;  // NOLINT
using namespace sensors;  // NOLINT

namespace crystal {

/**
 * @class Actions
 * @brief Class that allows to define the behavior of the Crystal device in response to specific events during state transitions of the FSM.
 * 
 * The methods of this class encapsulate the action that needs to be performed when executing a specific FSM event.
 */
class Actions {
 public:
  Actions() = delete;
  /**
   * @brief Construct a new Actions object.
   * 
   * @param clientHxp shared pointer to IHXP Class.
   * @param clientStepper shared pointer to IMotor Class.
   * @param clientScanningHXP shared pointer to IScanning Class.
   * @param clientScanningStepper shared pointer to IScanning Class.
   * @param clientSensors shared pointer to ISensors Class.
   * @param clientConfiguration shared pointer to IConfiguration Class.
   * @param clientPostProcessing shared pointer to IPostProcessing Class.
   */
  explicit Actions(std::shared_ptr<IHXP> clientHxp,
                   std::shared_ptr<IMotor> clientStepper,
                   std::shared_ptr<scanning::IScanning> clientScanningHXP,
                   std::shared_ptr<scanning::IScanning> clientScanningStepper,
                   std::shared_ptr<sensors::ISensors> clientSensors,
                   std::shared_ptr<IConfiguration> clientConfiguration,
                   std::shared_ptr<IPostProcessing> clientPostProcessing);
  /**
   * @brief Destroy the Actions object.
   * 
   */
  ~Actions();
  /**
   * @brief Method used to connect and calibrate the stepper motor and to connect the Hexapod robot that control the movement of the Crystal.
   * 
   * @return true if connection was successfull.
   * @return false if connection failed.
   */
  bool connect();
  /**
   * @brief Method used to disconnect the stepper motor and the Hexapod robot that control the movement of the Crystal.
   * 
   * @return true if the disconnection was successfull.
   * @return false if the disconnection failed.
   */
  bool disconnect();
  /**
   * @brief Method used to home the stepper motor and the Hexapod robot that control the movement of the Crystal.
   * 
   * @return true if the movement was successfull.
   * @return false otherwise.
   */
  bool goHome();
  /**
   * @brief Method used to move to a specific position the Hexapod robot that controls the movement of the Crystal.
   * 
   * @note The target position where the motor will be moved is stored in the private parameter 'hxpCoord_'
   * (structure variable of type HxpCoordinates of Class 'HXP') and must be set before calling this method.
   * 
   * @return true if the movement was successfull.
   * @return false otherwise.
   */
  bool setHxpPositionAbsolute();
  /**
   * @brief Method used to move to a specific position the stepper motor that controls the rotation of the Crystal.
   * 
   * @note The target position where the motor will be moved is stored in the private parameter 'stepperPosition_'
   * and must be set before calling this method.
   * 
   * @return true if the movement was successfull.
   * @return false otherwise.
   */
  bool moveCalibratedMotor();
  /**
   * @brief Method used to start a scan of the stepper motor that controls the rotation of the crystal.
   * 
   * This method calls the method 'scan' of class IScanning.
   * 
   * @return true if the scan was successfull.
   * @return false otherwise.
   */
  bool startScanStepper();
  /**
   * @brief Method used to stop a scan of the stepper motor or hexapod robot.
   * 
   * @return true if the stop was successfull.
   * @return false otherwise.
   */
  bool stop();
  /**
   * @brief Method used to start a scan of one of the axes of the hexapod that controls the pose of the crystal.
   * 
   * This method calls the method 'scan' of class IScanning.
   * 
   * @return true if the scan was successfull.
   * @return false otherwise.
   */
  bool startScanHxp();
  /**
   * @brief Getter function of the private parameter 'stepperPosition_'.
   * 
   * @return float representing the value of the private parameter 'stepperPosition_'.
   */
  float getStepperPosition();
   /**
   * @brief Setter function of the private parameter 'stepperPosition_'.
   * 
   * @param stepperPosition float representing to be set to the private parameter 'stepperPosition_'.
   */
  void setStepperPosition(float stepperPosition);
  /**
   * @brief Method used to move to a target position the hexapod robot and the stepper motor that control the rotation of the crystal.
   * 
   * @note Before calling this method the private parameter 'hxpCoord_' (structure variable of type HxpCoordinates of Class 'HXP') 
   * and the private parameter 'stepperPosition_' (of this Class) must be set.
   * 
   * @return true if the movements were successfull.
   * @return false otherwise.
   */
  bool moveBothMotors();
  /**
  * @brief This method is used to align the crystal on the horizontal (X) axis.
  * 
  * This alignment is performed by a movement in steps of the crystal on the X-axis of the hexapd.
  * 
  * @details
  * 1. The 'moveBothMotors' function is called to move the stepper motor and the hexapod to the initial positons;
  * 2. The .csv files used to register the result of the alignment are set-up;
  * 3. The 'scan' method (that performs a scan in the X axis direction) of the object named clientScanningHXP_ is called;
  * 4. The function then calls the 'executeScript2' function of the object named 'clientPostProcessing_';
  * 5. With the use of the latter an alignment position is computed and saved in a .csv file;
  * 6. The function then reads the new X-axis position from the .csv file and writes it in a .ini configuration file.
  * 7. The function sets the X-coordinate of the object named 'clientHxp_' to the new X-axis alignment position.
  * 8. The function calls the 'setHxpPositionAbsolute' function wich sets the position of the HXP to the new alignment position.
  * 
  * @return true if the alignment has been executed correctly.
  * @return false otherwise.
  */
  bool searchXAxisAlignmentCrystal();
  /**
   * @brief 
   * 
   * @return true 
   * @return false 
   */
  bool searchYAxisAlignmentCrystal();
  /**
  * @brief This method is used to align the crystal on the vertical (Z) axis.
  * 
  * This alignment is performed by a movement in steps of the crystal on the Z-axis of the hexapd.
  * 
  * @details
  * 1. The 'moveBothMotors' function is called to move the stepper motor and the hexapod to the initial positons;
  * 2. The .csv files used to register the result of the alignment are set-up;
  * 3. The 'scan' method (that performs a scan in the Z axis direction) of the object named clientScanningHXP_ is called;
  * 4. The function then calls the 'executeScript2' function of the object named 'clientPostProcessing_';
  * 5. With the use of the latter an alignment position is computed and saved in a .csv file;
  * 6. The function then reads the new Z-axis position from the .csv file and writes it in a .ini configuration file.
  * 7. The function sets the Z-coordinate of the object named 'clientHxp_' to the new Z-axis alignment position.
  * 8. The function calls the 'setHxpPositionAbsolute' function wich sets the position of the HXP to the new alignment position.
  * 
  * @return true if the alignment has been executed correctly.
  * @return false otherwise.
  */
  bool searchZAxisAlignmentCrystal();
  /**
  * @brief This method is used to check the alignment of the crystal on the horizontal (Y) axis.
  * 
  * This alignment check is performed by a movement in steps of the crystal on the Y-axis of the hexapd followed by a scan on the W-axis.
  * 
  * @details
  * 1. The 'moveBothMotors' function is called to move the stepper motor and the hexapod to the initial positons;
  * 2. The .csv files used to register the result and data of the alignment are set-up;
  * 3. The range of the scan and step size to execute on the Y-axis is read;
  * 4. For every movement performed on the y-axis the 'scan' method (that performs a scan in the W axis direction)
  *    of the object named clientScanningHXP_ is called;
  * 5. For every movement performed on the y-axis the 'executeScript5' function of the
  *    object named 'clientPostProcessing_' is also called;
  * 6. At the end of the motion in steps the alignment on the y-axis is checked;
  * 9. The function calls the 'setHxpPositionAbsolute' function wich sets the position of the HXP to the new alignment position.
  * 
  * @return true if the alignment has been executed correctly.
  * @return false otherwise.
  */
  bool searchYWAxesAlignmentCrystal();
  /**
  * @brief This method is used to search the Crystal's Bragg peak.
  * 
  * This measurement is performed by a movement in steps of the crystal on the W-axis of the hexapd.
  * 
  * @details
  * 1. The 'moveBothMotors' function is called to move the stepper motor and the hexapod to the initial positons;
  * 2. The .csv files used to register the result of the alignment are set-up;
  * 3. The 'scan' method (that performs a scan in the W axis direction) of the object named clientScanningHXP_ is called;
  * 4. The function then calls the 'executeScript' function of the object named 'clientPostProcessing_';
  * 5. With the use of the latter an alignment position is computed and saved in a .csv file;
  * 6. The function then reads the new w-axis position from the .csv file and writes it in a .ini configuration file.
  * 7. The function sets the W-coordinate of the object named 'clientHxp_' to the new W-axis alignment position.
  * 8. The function calls the 'setHxpPositionAbsolute' function wich sets the position of the HXP to the new alignment position.
  * 
  * @return true if the alignment has been executed correctly.
  * @return false otherwise.
  */
  bool searchBraggPeakCrystal();
   /**
  * @brief This method is used to execute the fine alignment of the crystal on the horizontal (Y) axis.
  * 
  * @details
  * 1. The 'moveBothMotors' function is called to move the stepper motor and the hexapod to the initial positons;
  * 2. The .csv files used to register the result of the alignment are set-up;
  * 3. The 'scan' method (that performs a scan in the W axis direction) of the object named clientScanningHXP_ is called
  *    to compute the value of the fully opened beam (the latter calculated with the call to a python script);
  * 4. The movement to the initial (y-axis) position (Y0 - (Crystal Width / 2)) is executed and
  *    the range of the scan and step size to execute on the Y-axis is read;
  * 5. For every movement performed on the y-axis the 'scan' method (that performs a scan in the W axis direction)
  *    of the object named clientScanningHXP_ is called;
  * 6. For every movement performed on the y-axis the 'executeScrip65' function of the
  *    object named 'clientPostProcessing_' is also called
  *    (this function executes a python script that computes the Bragg's peak);
  * 7. The movement to the initial (y-axis) position (Y0 + (Crystal Width / 2)) is executed;
  * 8. For every movement performed on the y-axis the 'scan' method (that performs a scan in the W axis direction)
  *    of the object named clientScanningHXP_ is called;
  * 9. For every movement performed on the y-axis the 'executeScrip65' function of the
  *    object named 'clientPostProcessing_' is also called
  *    (this function executes a python script that computes the Bragg's peak);
  * 10. The alignment position is computed;
  * 11. The alignment Y-coordinate of the object named 'clientHxp_' is set.
  * 12. The alignment position is written in a configuration file.
  * 13. The 'setHxpPositionAbsolute' function wich sets the position of the HXP to the new alignment position is called.
  * 
  * @return true if the alignment has been executed correctly.
  * @return false otherwise.
  */
  bool searchYAxisFineAlignment();
  /**
   * @brief This method checks if the crystal is aligned in a flipped orientation
   * by comparing the initial and flipped y-axis alignment positions
   * 
   * @details
   * 1. The initial y-axis alignment position is read from a configuration file;
   * 2. The coordinate W of the crystal stage is set by subtracting the offset
   *    from the Bragg angle from the value read from another configuration file.
   * 3. The method 'searchYAxisFineAlignment' is called;
   * 4. The new flipped y-axis alignment position is read from the configuration file,
   *    and the difference between the flipped and initial y-axis positions is calculated.
   * 5. If the difference is greater than a threshold value also read from the configuration file,
   *    the method 'compensateAndReAlign' is called to compensate for the difference and realign the crystal.
   * 
   * @return true if the alignment is correct.
   * @return false otherwise.
   */
  bool checkAlignmentFlippedOrientation();
  /**
   * @brief This method is used to compensate the misalignment on the hexapod y-axis and 
   * re-align the x-axis and y-axis with the compensation applied.
   * 
   * @details
   * 1. The hexapod y-axis is compensated by setting the y-coordinate
   *    of the clientHxp_ object to the compensation value;
   * 2. The stepper motor position is set to zero;
   * 3. Both motors are moved using the 'moveBothMotors' function,
   *    and the result is stored in 'result_movement';
   * 4. The x-axis is re-aligned using the searchXAxisAlignmentCrystal function,
   *    which returns a boolean value indicating whether the alignment was successful.
   *    If the result is false, the method returns false, indicating failure;
   * 5. The y-axis is re-aligned using the searchYAxisFineAlignment function,
   *    which returns a boolean value indicating whether the alignment was successful.
   *    If the result is false, the method returns false, indicating failure.
   * 6. The stepper motor position is set to 180;
   * 7. The alignment of the y-axis in the flipped orientation is checked using
   *    the 'checkAlignmentFlippedOrientation' function, which returns a boolean
   *    value indicating whether the alignment was successful. 
   *    The result of this function is returned from the compensateAndReAlign
   *    function, indicating success or failure.
   * 
   * @param compensation float value used to compensate for any misalignment in the y-axis
   * @return true if the compensation was successfull.
   * @return false otherwise.
   */
  bool compensateAndReAlign(float compensation);
  /**
  * @brief This method is used to measure the bending angle of the crystal under test.
  * 
  * The bending angle measurement consinsts in performing a double axis scan
  * (scan on the W axis for every step performed on the Y axis).
  * 
  * @details 
  * 1. The Hexapod and the stepper motor are moved to the predefined initial positions;
  * 2. The .csv files used to register the result of the alignment are set-up;
  * 3. The range of the scan and step size to execute on the Y-axis are read from the configuration file;
  * 4. For every movement performed on the y-axis the 'scan' method (that performs a scan in the W axis direction)
  *    of the object named clientScanningHXP_ is called;
  * 5. For every movement performed on the y-axis the 'executeScript6' function of the
  *    object named 'clientPostProcessing_' is also called;
  * 6. At the end of the motion in steps on the y-axis the beding angle is read from a .csv file
  *    and written in the configuration file;
  * 
  * @return true if the measurement has been executed correctly.
  * @return false otherwise.
  */
  bool bendingAngleMeasurement();
  /**
  * @brief This function is used to measure the miscut angle of the crystal under test.
  * 
  * The miscut angle measurement consinsts in performing a double axis scan
  * (scan on the W axis for every step performed on the Y axis).
  * 
  @details 
  * 1. The Hexapod and the stepper motor are moved to the predefined initial positions;
  * 2. The .csv files used to register the result of the alignment are set-up;
  * 3. The range of the scan and step size to execute on the Y-axis are read from the configuration file;
  * 4. For every movement performed on the y-axis the 'scan' method (that performs a scan in the W axis direction)
  *    of the object named clientScanningHXP_ is called;
  * 5. For every movement performed on the y-axis the 'executeScript7' function of the
  *    object named 'clientPostProcessing_' is also called;
  * 6. At the end of the motion in steps on the y-axis the miscut angle is read from a .csv file
  *    and written in the configuration file;
  * 
  * @return true if the measurement has been executed correctly.
  * @return false otherwise.
  */
  bool miscutAngleMeasurement();
  /**
  * @brief This function is used to measure the torsion angle of the crystal under test.
  * 
  * The torsion angle measurement consinsts in performing a double axis scan
  * (scan on the W axis for every step performed on the Z axis).
  * 
  @details 
  * 1. The Hexapod and the stepper motor are moved to the predefined initial positions;
  * 2. The .csv files used to register the result of the alignment are set-up;
  * 3. The range of the scan and step size to execute on the Z-axis are read from the configuration file;
  * 4. For every movement performed on the Z-axis the 'scan' method (that performs a scan in the W axis direction)
  *    of the object named clientScanningHXP_ is called;
  * 5. For every movement performed on the Z-axis the 'executeScript7' function of the
  *    object named 'clientPostProcessing_' is also called;
  * 6. At the end of the motion in steps on the Z-axis the torsion angle is read from a .csv file
  *    and written in the configuration file;
  * 
  * @return true if the measurement has been executed correctly.
  * @return false otherwise.
  */
  bool torsionAngleMeasurement();

 private:
  std::shared_ptr<IHXP> clientHxp_;  /**< Shared pointer to IHXP Class*/
  std::shared_ptr<IMotor> clientStepper_;  /**< Shared pointer to IMotor Class*/
  std::shared_ptr<scanning::IScanning> clientScanningHXP_;  /**< Shared pointer to IScanning Class*/
  std::shared_ptr<scanning::IScanning> clientScanningStepper_;  /**< Shared pointer to IScanning Class*/
  std::shared_ptr<sensors::ISensors> clientSensors_;  /**< Shared pointer to ISensors Class*/
  std::shared_ptr<IConfiguration> clientConfiguration_;  /**< Shared pointer to IConfiguration Class*/
  std::shared_ptr<IPostProcessing> clientPostProcessing_;  /**< Shared pointer to IPostProcessing Class*/
  float stepperPosition_;
  /**
   * @struct HxpAlignmentCoordinates
   * @brief Struct containing the 6-axes hexapod coordinates.
   * 
   */
  struct HxpAlignmentCoordinates {
        float CoordX = 0;
        float CoordY = 0;
        float CoordZ = 0;
        float CoordU = 0;
        float CoordV = 0;
        float CoordW = 0;
  };
  float crystalWidth_;  /**< Crystal Width is read from the configuration file. */
  HxpAlignmentCoordinates hxpAlignmentCoord_;  /**< Structure variable of type HxpAlignmentCoordinates. */
  std::filesystem::path pathToProjDirectory_ = clientSensors_->getPathToProjDirectory();  /**< Path to the project directory. */
  std::filesystem::path pathToScriptDirectory_ = pathToProjDirectory_ / "modules\\Devices\\scripts\\Crystal";  /**< Path to the directory where all the scripts to perform the alignments and measurements are contained. */
  std::filesystem::path pathToScriptMeasurementsDirectory_ = pathToProjDirectory_ / "modules\\Devices\\scripts\\Crystal\\Measurements";  /**< Path to the directory where all the scripts to perform the measurements are contained. */
  std::filesystem::path crystalAlignmentResultsDirectoryName_ = "CrystalAlignmentResults";  /**< Name of the directory where to save the results of the alignments and measurements. */
  std::string pathToCrystalAlinmentResultsDirectory_;  /**< Path to the directory where to save the results of the alignments and measurements. */
};

}  // namespace crystal

#endif  // MODULES_DEVICES_INCLUDE_CRYSTAL_ACTIONS_HPP_
