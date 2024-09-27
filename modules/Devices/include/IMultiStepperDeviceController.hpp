/**
 * @file IMultiStepperDeviceController.hpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2022
 * @brief Interface used to control devices moved by two stepper motors.
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

class IMultiStepperDeviceController {
 public:
    int max_size = 1048576 * 5;  /**< Maximum size of the sink logger (5mb in this case). */
    int max_files = 3;

    ~IMultiStepperDeviceController() = default;

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
     * @brief Method used to process the event to go to the FSM state 'systemHome'.
     * 
     * @note The event is lauched only if the FSM is in state 'systemConnected', 'systemScan' or 'systemHome'.
     * 
     * @return false if after the execution of the event the FSM is not in state 'systemHome'.
     * @return true otherwise.
     */
    virtual bool goHome() = 0;
    
    /**
     * @brief Getter method that returns the private member 'stepper1LinearPosition_' of class Actions.
     * 
     * @return float representing the position of the stepper motor that controls the rotation of the crystal.
     */
    virtual float getPositionStepperLinear() = 0;

    /**
     * @brief Getter method that returns the private member 'stepper2RotationalPosition_' of class Actions.
     * 
     * @return float representing the position of the stepper motor that controls the rotation of the crystal.
     */
    virtual float getPositionStepperRotational() = 0;

    /**
     * @brief Method used to process the event to deinitialize the FSM.
     * 
     * @note The event is lauched only if the FSM is in state 'systemConnected', 'systemHome' or 'systemScan'.
     * 
     * @return false if after the execution of the event the FSM is not in state 'systemNotInitialized'.
     * @return true otherwise.
     */
    virtual bool disconnectStp() = 0;

    /**
     * @brief Method used to process the event to go to the FSM state 'systemConnected'.
     * 
     * @note The event is lauched only if the FSM is in state 'systemConnected', 'systemHome' or 'systemScan'.
     * 
     * @param position target position to reach.
     * @return false if after the execution of the event the FSM is not in state 'systemConnected'.
     * @return true otherwise.
     */
    virtual bool moveToPositionStepper1Linear(float position) = 0;

   /**
     * @brief Method used to process the event to go to the FSM state 'systemConnected'.
     * 
     * @note The event is lauched only if the FSM is in state 'systemConnected', 'systemHome' or 'systemScan'.
     * 
     * @param position target position to reach.
     * @return false if after the execution of the event the FSM is not in state 'systemConnected'.
     * @return true otherwise.
     */
    virtual bool moveToPositionStepper2Rotational(float position) = 0;

    /**
     * @brief Method used to process the event to go to the FSM state 'systemScan'.
     * 
     * @note The event is lauched only if the FSM is in state 'systemConnected', 'systemHome' or 'systemScan'.
     * The scan is a motion of the stepper motor (linear axis) to a final position divided in steps
     * (whose length is defined in input to this method).
     * At the end of every step an acquisition of data by the sensors X-Ray and Autocollimator is performed.
     * 
     * @param stepSize step size of the movements during the scan.
     * @param range range of the scan. The scan starts always from the current position of the hexapod robot and is executed in range / stepsize = steps.
     * @param durationAcquisition duration of acquisition of data from the X-Ray Sensor.
     * @param filename name of the file where to log X-Ray sensor data and motors positions.
     * @param flushFlag if true erase the content of the .csv file. If false append the new contents at the end of the file.
     * @param showPlot if true a plot will be showed at the end of the scan.
     * @return true if the scan was executed correctly.
     * @return false otherwise.
     */
    virtual bool scanStepper1Linear(float stepSize, float range, int durationAcquisition, std::string filename, bool eraseCsvContent, bool showPlot) = 0;

    /**
     * @brief Method used to process the event to go to the FSM state 'systemScan'.
     * 
     * @note The event is lauched only if the FSM is in state 'systemConnected', 'systemHome' or 'systemScan'.
     * The scan is a motion of the stepper motor (rotational axis) to a final position divided in steps
     * (whose length is defined in input to this method).
     * At the end of every step an acquisition of data by the sensors X-Ray and Autocollimator is performed.
     * 
     * @param stepSize step size of the movements during the scan.
     * @param range range of the scan. The scan starts always from the current position of the hexapod robot and is executed in range / stepsize = steps.
     * @param durationAcquisition duration of acquisition of data from the X-Ray Sensor.
     * @param filename name of the file where to log X-Ray sensor data and motors positions.
     * @param flushFlag if true erase the content of the .csv file. If false append the new contents at the end of the file.
     * @param showPlot if true a plot will be showed at the end of the scan.
     * @return true if the scan was executed correctly.
     * @return false otherwise.
     */
    virtual bool scanStepper2Rotational(float stepSize, float range, int durationAcquisition, std::string filename, bool eraseCsvContent, bool showPlot) = 0;

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
     * @brief This method creates a rotating logger with a specified filename and path to log files.
     * 
     * The logger is created using the spdlog library and is set as the default logger.
     * The maximum size and number of log files to keep can also be specified.
     * 
     * @param filename of the log file.
     * @return true to indicate that the logger was successfully created.
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
     * @brief Method used to process the event to go to the FSM state 'systemScan'.
     * 
     * This function is used to move the monochromator crystal and the slit out of the beam path in order
     * to align the X-Ray Source with the X-Ray Sensor.
     * 
     * @note The event is lauched only if the FSM is in state 'systemConnected', 'systemHome' or 'systemScan'.
     * 
     * @return false if after the execution of the event the FSM is not in state 'systemScan'.
     * @return true otherwise.
     */
    virtual bool alignSourceWithSensor() = 0;

    /**
     * @brief Method used to process the event to go to the FSM state 'systemScan'.
     * 
     * This function is used to align the monochromator device.
     * 
     * @note The event is lauched only if the FSM is in state 'systemConnected', 'systemHome' or 'systemScan'.
     * 
     * @param alignDevice boolean parameter. If true the alignment position is computed.
     *        If false the alignment position is not computed and the device is moved to the
     *        pre-computed position.
     * @return false if after the execution of the event the FSM is not in state 'systemScan'.
     * @return true otherwise.
     */
    virtual bool alignMonochromator(bool alignDevice) = 0;

    /**
     * @brief Method used to process the event to go to the FSM state 'systemScan'.
     * 
     * This function is used to search the Monochromator Bragg's peak.
     * 
     * @note The event is lauched only if the FSM is in state 'systemConnected', 'systemHome' or 'systemScan'.
     * 
     * @param alignDevice boolean parameter. If true the alignment position is computed.
     *        If false the alignment position is not computed and the device is moved to the
     *        pre-computed position.
     * @return false if after the execution of the event the FSM is not in state 'systemScan'.
     * @return true otherwise.
     */
    virtual bool searchMonochromatorBraggPeak() = 0;

    /**
     * @brief Method used to process the event to go to the FSM state 'systemScan'.
     * 
     * This function is used to align the slit device.
     * 
     * @note The event is lauched only if the FSM is in state 'systemConnected', 'systemHome' or 'systemScan'.
     * 
     * @param alignDevice boolean parameter. If true the alignment position is computed.
     *        If false the alignment position is not computed and the device is moved to the
     *        pre-computed position.
     * @return false if after the execution of the event the FSM is not in state 'systemScan'.
     * @return true otherwise.
     */
    virtual bool alignSlit(bool alignDevice) = 0;
};
