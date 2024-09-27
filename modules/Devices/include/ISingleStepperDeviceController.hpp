/**
 * @file ISingleStepperDeviceController.hpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2022
 * @brief Interface used to control devices moved by only one stepper motor.
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

class ISingleStepperDeviceController {
 public:
    // sink logger parameters
    int max_size = 1048576 * 5;  /**< Maximum size of the sink logger (5mb in this case)*/
    int max_files = 3;

    ~ISingleStepperDeviceController() = default;

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
     * @note The event is lauched only if the FSM is in state 'systemConnected' or 'systemScan'.
     * 
     * @return false if after the execution of the event the FSM is not in state 'systemHome'.
     * @return true otherwise.
     */
    virtual bool goHome() = 0;

    /**
     * @brief Getter method that returns the private member 'stepperPosition_' of class Actions.
     * 
     * @return float 
     */
    virtual float getPositionStepper() = 0;

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
     * @return false if after the execution of the event the FSM is not in state 'systemConnected'.
     * @return true otherwise.
     */
    virtual bool moveToPositionStepper(float position) = 0;

    /**
     * @brief Method used to process the event to go to the FSM state 'systemScan'.
     * 
     * The scan is a motion of the motor to a final position divided in steps (whose length is defined in input to this method).
     * At the end of every step an acquisition of data by the sensors X-Ray and Autocollimator is performed.
     * 
     * @note The event is lauched only if the FSM is in state 'systemConnected', 'systemHome' or 'systemScan'.
     * 
     * @param stepSize step size of the scan.
     * @param range range of the scan if the range is negative the scan is performed in backward motion (towards negative axis values), in forward motion (towards positive axis values) otherwise.
     * @param durationAcquisition amount of time that the device needs to stop in order to read the x-ray detector.
     * @param filename name of the file where to log the data. Filename must finish with the file extention ".csv".
     * @param eraseCsvContent if true the data registered by the xray sensor will not be saved at the end of the scan.
     * @param showPlot boolean parameter to decide whether to plot the data register during the scan or not.
     * @return false if after the execution of the event the FSM is not in state 'systemScan'.
     * @return true otherwise.
     */
    virtual bool scanStepper(float stepSize, float range, int durationAcquisition, std::string filename, bool flushFlag, bool showPlot) = 0;

    /**
     * @brief @brief Method used to launch the event 'eventStopMovement'.
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
    * @brief This function is used to center the X-Ray Source with the X-Ray Sensor.
    * @param searchCenter if true the center is searched and the source is moved to the new center position. If false the motor is moved to the already known center position.
    * @note searchCenter param is valid only for device XRaySource. For every other device this parameter is ignored.
    * @return true if the alignment was executed correctly.
    * @return false otherwise.
    */
    virtual bool alignSourceWithSensor(bool searchCenter) = 0;

    /**
    * @brief This function is used to setup all the devices controlled by one stepper motor for the Monochromator Bragg Peak Search.
    */
    virtual bool setupMonochromatorBraggPeakSearch() = 0;

    /**
    * @brief This function is used to setup all the devices controlled by one stepper motor for the Crystal Bragg Peak Search.
    * Therefore, only the X-Ray Sensor will move to 2Theta position.
    */
    virtual bool setupForCrystalBraggPeakSearch() = 0;
};
