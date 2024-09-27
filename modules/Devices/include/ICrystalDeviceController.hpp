/**
 * @file ICrystalDeviceController.hpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2022
 * @brief Interface used to control the crystal device moved by a stepper motor and an hexapod robot.
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
 * @brief Interface used to control the crystal device moved by a stepper motor and an hexapod robot.
 * 
 */
class ICrystalDeviceController {
 public:
    int max_size = 1048576 * 5;  /**< Maximum size of the sink logger (5mb in this case). */
    int max_files = 3;
    enum hxpAxis { X = 1, Y = 2, Z = 3, U = 4, V = 5, W = 6 }; /**< This enum defines the different axes of the hexapod. (this->hxpAxis::Z indicates the Z axis of the hxp).*/

    ~ICrystalDeviceController() = default;

    /**
     * @brief Method used to process the event to initialize the FSM.
     * 
     * @note The event is lauched only if the FSM is in state 'systemNotInitialized' or 'systemError'.
     * 
     * @return false if after the execution of the event the FSM is not in state 'systemConnected'.
     * @return true otherwise.
     */
    virtual bool start() = 0;

    /**
     * @brief Method used to launch the event 'eventStopMovement'.
     * 
     * @note The event is lauched only if the FSM is in state 'systemConnected', 'systemHome' or 'systemScan'.
     * 
     * @return false if after the execution of the event the FSM is not in state 'systemConnected'.
     * @return true otherwise.
     */
    virtual bool stop() = 0;

    /**
     * @brief Method used to process the event to deinitialize the FSM.
     * 
     * @note The event is lauched only if the FSM is in state 'systemConnected', 'systemHome' or 'systemScan'.
     * 
     * @return false if after the execution of the event the FSM is not in state 'systemNotInitialized'.
     * @return true otherwise.
     */
    virtual bool disconnect() = 0;

    /**
     * @brief Method used to process the event to go to the FSM state 'systemHome'.
     * 
     * @note The event is lauched only if the FSM is in state 'systemConnected', 'systemScan' or 'systemHome'.
     * 
     * @return false if after the execution of the event the FSM is not in state 'systemHome'.
     * @return true otherwise.
     */
    virtual bool goHome() = 0;

    /**
     * @brief Getter method that returns the private member 'stepperPosition_' of class Actions.
     * 
     * @return float representing the position of the stepper motor that controls the rotation of the crystal.
     */
    virtual float getPositionStepper() = 0;

    /**
     * @brief This function returns the current X-Axis position of the Hexapod.
     * 
     * It returns the axis position by calling the method 'getPositionX' of Class 'HXP'.
     * 
     * @return X-Axis position.
     */
    virtual double getPositionX() = 0;

    /**
     * @brief This function returns the current Y-Axis position of the Hexapod.
     * 
     * It returns the axis position by calling the method 'getPositionY' of Class 'HXP'.
     * 
     * @return Y-Axis position.
     */
    virtual double getPositionY() = 0;

    /**
     * @brief This function returns the current Z-Axis position of the Hexapod.
     * 
     * It returns the axis position by calling the method 'getPositionZ' of Class 'HXP'.
     * 
     * @return Z-Axis position.
     */
    virtual double getPositionZ() = 0;

    /**
     * @brief This function returns the current U-Axis position of the Hexapod.
     * 
     * It returns the axis position by calling the method 'getPositionU' of Class 'HXP'.
     * 
     * @return U-Axis position.
     */
    virtual double getPositionU() = 0;

    /**
     * @brief This function returns the current V-Axis position of the Hexapod.
     * 
     * It returns the axis position by calling the method 'getPositionV' of Class 'HXP'.
     * 
     * @return V-Axis position.
     */
    virtual double getPositionV() = 0;

    /**
     * @brief This function returns the current W-Axis position of the Hexapod.
     * 
     * It returns the axis position by calling the method 'getPositionW' of Class 'HXP'.
     * 
     * @return W-Axis position.
     */
    virtual double getPositionW() = 0;

    /**
     *  @brief Method used to process the event to go to the FSM state 'systemConnected'.
     * 
     * @note The event is lauched only if the FSM is in state 'systemConnected', 'systemHome' or 'systemScan'.
     * 
     * @param CoordX X-axis coordinate incremental displacement value.
     * @param CoordY Y-axis coordinate incremental displacement value.
     * @param CoordZ Z-axis coordinate incremental displacement value.
     * @param CoordU U-axis coordinate incremental displacement value.
     * @param CoordV V-axis coordinate incremental displacement value.
     * @param CoordW W-axis coordinate incremental displacement value.
     * @return false if after the execution of the event the FSM is not in state 'systemConnected'.
     * @return true otherwise.
     */
    virtual bool moveToAbsPosition(double CoordX, double CoordY, double CoordZ, double CoordU, double CoordV, double CoordW) = 0;

    /**
     * @brief Method used to process the event to go to the FSM state 'systemScan'.
     * 
     * @note The event is lauched only if the FSM is in state 'systemConnected', 'systemHome' or 'systemScan'.
     * The scan is a motion of one of the axes of the hexapod robot to a final position divided in steps
     * (whose length is defined in input to this method).
     * At the end of every step an acquisition of data by the sensors X-Ray and Autocollimator is performed.
     * 
     * @param axisToScan - integer number (1 to 6) representing the Hexapod axis to move during the scan.
     * @param stepSize - step size of the movements during the scan.
     * @param range - range of the scan. The scan starts always from the current position of the hexapod robot and is executed in range / stepsize = steps.
     * @param durationAcquisition - duration of acquisition of data from the X-Ray Sensor.
     * @param filename - name of the file where to log X-Ray sensor data and motors positions.
     * @param flushFlag - if true erase the content of the .csv file. If false append the new contents at the end of the file.
     * @param showPlot - if true a plot will be showed at the end of the scan.
     * @return false if after the execution of the event the FSM is not in state 'systemScan'.
     * @return true otherwise.
     */
    virtual bool scanHxp(int axisToScan, float stepSize, float range, int durationAcquisition, std::string filename, bool eraseCsvContent, bool showPlot) = 0;

    /**
     * @brief Method used to process the event to go to the FSM state 'systemScan'.
     * 
     * @note The event is lauched only if the FSM is in state 'systemConnected', 'systemHome' or 'systemScan'.
     * The scan is a motion of the stepper motor to a final position divided in steps (whose length is defined in input to this method).
     * At the end of every step an acquisition of data by the sensors X-Ray and Autocollimator is performed.
     * 
     * @param axisToScan - integer number (1 to 6) representing the Hexapod axis to move during the scan.
     * @param stepSize - step size of the movements during the scan.
     * @param range - range of the scan. The scan starts always from the current position of the hexapod robot and is executed in range / stepsize = steps.
     * @param durationAcquisition - duration of acquisition of data from the X-Ray Sensor.
     * @param filename - name of the file where to log X-Ray sensor data and motors positions.
     * @param flushFlag - if true erase the content of the .csv file. If false append the new contents at the end of the file.
     * @param showPlot - if true a plot will be showed at the end of the scan.
     * @return false if after the execution of the event the FSM is not in state 'systemScan'.
     * @return true otherwise.
     */
    virtual bool scanStepper(float stepSize, float range, int durationAcquisition, std::string filename, bool eraseCsvContent, bool showPlot) = 0;

    /**
     * @brief Method used to process the event to go to the FSM state 'systemConnected'.
     * 
     * @note The event is lauched only if the FSM is in state 'systemConnected', 'systemHome' or 'systemScan'.
     * 
     * @param position target position to reach.
     * @return false if after the execution of the event the FSM is not in state 'systemConnected'.
     * @return true otherwise.
     */
    virtual bool moveToPositionStepper(float position) = 0;

    /**
     * @brief This method creates a rotating logger with a specified filename and path to log files.
     * 
     * @note The logger is created using the spdlog library and is set as the default logger.
     * The maximum size and number of log files to keep can also be specified.
     * 
     * @param filename - std::string representing the name of the shell log file.
     * @return true if the logger was set correctly.
     * @return false otherwise.
     */
    virtual bool logger(std::string filename) = 0;

    /**
     * @brief Method used to get the FSM state.
     * 
     * @return std::string representing the FSM state.
     */
    virtual std::string getFsmState() = 0;

    /**
     * @brief Method used to launch the event 'eventMoveHxpToAbsolutePosition'.
     * 
     * @note The event is lauched only if the FSM is in state 'systemConnected', 'systemHome' or 'systemScan'.
     * This method moves the crystal out of the beam path in order to align the X-Ray Source with the X-Ray Sensor.
     * 
     * @return false if after the execution of the event the FSM is not in state 'systemScan'.
     * @return true otherwise.
     */
    virtual bool alignSourceWithSensor() = 0;

    /**
     * @brief Method used to launch the event 'eventXAxisAlignmentCrystal'.
     * 
     * @note The event is lauched only if the FSM is in state 'systemConnected', 'systemHome' or 'systemScan'.
     * This method aligns the crystal on the Hexapod robot X-Axis.
     * 
     * @details
     * 1. The hexapod and stepper motor coordinates are read from the configuration file;
     * 2. The hexapod and stepper motor coordinates are set;
     * 3. The axis to scan is set;
     * 4. The alignment parameters are read from the configuration file and set;
     * 5. The event 'eventXAxisAlignmentCrystal' is called.
     * 
     * @return false if after the execution of the event the FSM is not in state 'systemScan'.
     * @return true otherwise.
     */
    virtual bool xAxisAlignmentCrystal() = 0;

    /**
     * @brief Method used to launch the event 'eventYAxisAlignmentCrystal'.
     * 
     * @note The event is lauched only if the FSM is in state 'systemConnected', 'systemHome' or 'systemScan'.
     * This method aligns the crystal on the Hexapod robot Y-Axis.
     * 
     * @details
     * 1. The initial hexapod and stepper motor coordinates are read from the configuration file;
     * 2. The hexapod and stepper motor coordinates are set;
     * 3. The axis to scan is set;
     * 4. The alignment parameters are read from the configuration file and set;
     * 5. The event 'eventYAxisAlignmentCrystal' is called.
     * 
     * @return false if after the execution of the event the FSM is not in state 'systemScan'.
     * @return true otherwise.
     */
    virtual bool yAxisAlignmentCrystal() = 0;

    /**
     * @brief Method used to launch the event 'eventZAxisAlignmentCrystal'.
     * 
     * @note The event is lauched only if the FSM is in state 'systemConnected', 'systemHome' or 'systemScan'.
     * This method aligns the crystal on the Hexapod robot Z-Axis.
     * 
     * @details
     * 1. The hexapod and stepper motor coordinates are read from the configuration file;
     * 2. The hexapod and stepper motor coordinates are set;
     * 3. The axis to scan is set;
     * 4. The alignment parameters are read from the configuration file and set;
     * 5. The event 'eventZAxisAlignmentCrystal' is called.
     * 
     * @return false if after the execution of the event the FSM is not in state 'systemScan'.
     * @return true otherwise.
     */
    virtual bool zAxisAlignmentCrystal() = 0;

    /**
     * @brief Method used to launch the event 'eventYWAxesAlignmentCrystal'.
     * 
     * @note The event is lauched only if the FSM is in state 'systemConnected', 'systemHome' or 'systemScan'.
     * This method aligns the crystal on the Hexapod robot Y-Axis.
     * 
     * @details
     * 1. The initial hexapod and stepper motor coordinates are read from the configuration file;
     * 2. The hexapod and stepper motor coordinates are set;
     * 3. The axis to scan is set;
     * 4. The alignment parameters are read from the configuration file and set;
     * 5. The event 'eventYWAxesAlignmentCrystal' is called.
     * 
     * @return false if after the execution of the event the FSM is not in state 'systemScan'.
     * @return true otherwise.
     */
    virtual bool yWAxesAlignmentCrystal() = 0;

    /**
     * @brief Method used to launch the event 'eventXAxisAlignmentCrystal'.
     * 
     * @note The event is lauched only if the FSM is in state 'systemConnected', 'systemHome' or 'systemScan'.
     * This method executes a fine alignment of the crystal on the Hexapod robot X-Axis.
     * 
     * @details
     * 1. The initial hexapod and stepper motor coordinates are read from the configuration file;
     * 2. The hexapod and stepper motor coordinates are set;
     * 3. The axis to scan is set;
     * 4. The alignment parameters are read from the configuration file and set;
     * 5. The event 'eventXAxisAlignmentCrystal' is called.
     * 
     * @return false if after the execution of the event the FSM is not in state 'systemScan'.
     * @return true otherwise.
     */
    virtual bool xAxisFineAlignmentCrystal() = 0;

    /**
     * @brief Method used to launch the event 'eventSearchBraggPeakCrystal'.
     * 
     * @note The event is lauched only if the FSM is in state 'systemConnected', 'systemHome' or 'systemScan'.
     * This method executes the Bragg's Peak search of the crystal on the Hexapod robot W-Axis.
     * 
     * @details
     * 1. The initial hexapod and stepper motor coordinates are read from the configuration file;
     * 2. The hexapod and stepper motor coordinates are set;
     * 3. The axis to scan is set;
     * 4. The alignment parameters are read from the configuration file and set;
     * 5. The event 'eventSearchBraggPeakCrystal' is called.
     * 
     * @return false if after the execution of the event the FSM is not in state 'systemScan'.
     * @return true otherwise.
     */
    virtual bool braggPeakSearchCrystal() = 0;

    /**
     * @brief Method used to launch the event 'eventYAxisFineAlignmentCrystal'.
     * 
     * @note The event is lauched only if the FSM is in state 'systemConnected', 'systemHome' or 'systemScan'.
     * This method executes the fine alignment of the crystal on the Hexapod robot Y-Axis.
     * 
     * @details
     * 1. The initial hexapod and stepper motor coordinates are read from the configuration file;
     * 2. The hexapod and stepper motor coordinates are set;
     * 3. The axis to scan is set;
     * 4. The alignment parameters are read from the configuration file and set;
     * 5. The event 'eventYAxisFineAlignmentCrystal' is called.
     * 
     * @return false if after the execution of the event the FSM is not in state 'systemScan'.
     * @return true otherwise.
     */
    virtual bool yAxisFineAlignmentCrystal() = 0;

    /**
     * @brief Method used to launch the event 'eventCheckAlignmentInFlippedOrientation'.
     * 
     * @note The event is lauched only if the FSM is in state 'systemConnected', 'systemHome' or 'systemScan'.
     * This method executes the check of the alignment of the crystal in flipped orientation (stepper position to 180 deg).
     * 
     * @details
     * 1. The initial hexapod and stepper motor coordinates are read from the configuration file;
     * 2. The hexapod and stepper motor coordinates are set;
     * 3. The axis to scan is set;
     * 4. The alignment parameters are read from the configuration file and set;
     * 5. The event 'eventCheckAlignmentInFlippedOrientation' is called.
     * 
     * @return false if after the execution of the event the FSM is not in state 'systemScan'.
     * @return true otherwise.
     */
    virtual bool checkAlignmentInFlippedOrientation() = 0;

    /**
     * @brief Method used to launch the event 'eventBendingAngleMeasurement'.
     * 
     * @note The event is lauched only if the FSM is in state 'systemConnected', 'systemHome' or 'systemScan'.
     * This method executes the measurement of the bending angle of the crystal.
     * 
     * @details
     * 1. The initial hexapod and stepper motor coordinates are read from the configuration file;
     * 2. The hexapod and stepper motor coordinates are set;
     * 3. The axis to scan is set;
     * 4. The alignment parameters are read from the configuration file and set;
     * 5. The event 'eventBendingAngleMeasurement' is called.
     * 
     * @return false if after the execution of the event the FSM is not in state 'systemScan'.
     * @return true otherwise.
     */
    virtual bool bendingAngleMeasurement() = 0;

    /**
     * @brief Method used to launch the event 'eventMiscutAngleMeasurement'.
     * 
     * @note The event is lauched only if the FSM is in state 'systemConnected', 'systemHome' or 'systemScan'.
     * This method executes the measurement of the miscut angle of the crystal.
     * 
     * @details
     * 1. The initial hexapod and stepper motor coordinates are read from the configuration file;
     * 2. The hexapod and stepper motor coordinates are set;
     * 3. The axis to scan is set;
     * 4. The alignment parameters are read from the configuration file and set;
     * 5. The event 'eventMiscutAngleMeasurement' is called.
     * 
     * @return false if after the execution of the event the FSM is not in state 'systemScan'.
     * @return true otherwise.
     */
    virtual bool miscutAngleMeasurement(bool repeateBendingAngleMeas) = 0;

    /**
     * @brief Method used to launch the event 'eventTorsionAngleMeasurement'.
     * 
     * @note The event is lauched only if the FSM is in state 'systemConnected', 'systemHome' or 'systemScan'.
     * This method executes the measurement of the torsion angle of the crystal.
     * 
     * @details
     * 1. The initial hexapod and stepper motor coordinates are read from the configuration file;
     * 2. The hexapod and stepper motor coordinates are set;
     * 3. The axis to scan is set;
     * 4. The alignment parameters are read from the configuration file and set;
     * 5. The event 'eventTorsionAngleMeasurement' is called.
     * 
     * @return false if after the execution of the event the FSM is not in state 'systemScan'.
     * @return true otherwise.
     */
    virtual bool torsionAngleMeasurement() = 0;
};
