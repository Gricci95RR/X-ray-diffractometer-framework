/**
 * @file IHXP.hpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2021
 * @brief Class Interface used for controlling the hexapod robot.
 * @version 0.1
 * @date 2021
 * 
 * @copyright © Copyright CERN 2018. All rights reserved. This software is released under a CERN proprietary software license.
 * Any permission to use it shall be granted in writing. Requests shall be addressed to CERN through mail-KT@cern.ch
 * 
 */

#pragma once

#include <iostream>
#include <string>

/**
 * @class IHXP
 * @brief Class Interface used for controlling the hexapod robot.
 * 
 */
class IHXP {
 public:
    /**
     * @brief Destroy the IHXP object.
     * 
     */
    ~IHXP() = default;
    /**
     * @brief Setter function to set the IP address of the hexapod controller.
     * @param iPAddress the IP Address of the hexapod controller to be set.
     */
    virtual void setiPAddress(std::string iPAddress) = 0;

    /**
     * @brief Setter function to set the port number of the hexapod controller (5001).
     * @param nPort the number of port to be set.
     */
    virtual void setNPort(int nPort) = 0;

    /**
     * @brief Get TCP IP address.
     * @return string representing the IP address used to establish TCP communication.
     */
    virtual std::string getiPAddress() = 0;

    /**
     * @brief Get TCP IP port (5001 for HXP controller).
     * @return integer representing the number of the port used for TCP connection.
     */
    virtual int getNPort() = 0;

    /**
     * @brief Connect HXP (Configures the TCP/IP communication and opens a socket).
     * @param dTimeOut timeout value in seconds used for each function execution.
     * @param pGroup group name.
     * @return 1 if connection failed 
     * @return 0 if connection was successfull.
     */
    virtual int connect(int dTimeOut, std::string pGroup) = 0;

    /**
     * @brief This function initiates a home search for the Hexapd.
     * Function to send a command to the hexapod to go to the home position.
     * @return 1 if an error occurred.
     * @return 0 if the movement to home position was executed correctly.
     */
    virtual int goHome() = 0;

    /**
     * @brief Disconnect the hexapod.
     * Close TCP connection with hexapod controller.
     * @return always 0.
     */
    virtual int disconnect() = 0;

    /**
     * @brief Allows to set manually the position of the HXP and move to the defined position.
     * @param CoordX X-axis coordinate.
     * @param CoordY Y-axis coordinate.
     * @param CoordZ Z-axis coordinate.
     * @param CoordU U-axis coordinate.
     * @param CoordV V-axis coordinate.
     * @param CoordW W-axis coordinate.
     * @return 1 if an error occurred.
     * @return 0 if the movement to the specified position was executed.
     */
    virtual int setPositionAbsolute(double CoordX, double CoordY, double CoordZ, double CoordU, double CoordV, double CoordW) = 0;

    /**
     * @brief Move to the preset axis coordinate.
     * Executes an absolute movement to the preset axis coordinate.
     * The coordinates must be stored in the Struct variable "hxpCoord_" before the execution of this method.
     * @return 0 if the absolute movement is successful, 1 otherwise.
     */
    virtual int setPositionAbsolute() = 0;

    /**
     * @brief This function allows the hexapod to execute a trajectory with the maximum velocity to the X, Y, Z axis positions.
     * @param TrajectoryType string representing the trajectory type (Line, Arc or Rotation).
     * @param CoordX X-axis coordinate incremental displacement value.
     * @param CoordY Y-axis coordinate incremental displacement value.
     * @param CoordZ Z-axis coordinate incremental displacement value.
     * @return 1 if an error occurred.
     * @return 0 if the movement to the specified position was executed.
     */
    virtual int HexapodMoveIncrementalCtrl(std::string TrajectoryType, double CoordX, double CoordY, double CoordZ) = 0;

    /**
     * @brief This function allows the hexapod to execute a trajectory with a velocity specified in input maximum velocity to the X, Y, Z axis positions.
     * @param TrajectoryType trajectory type (Line, Arc or Rotation).
     * @param CoordX X-axis coordinate incremental displacement value.
     * @param CoordY Y-axis coordinate incremental displacement value.
     * @param CoordZ Z-axis coordinate incremental displacement value.
     * @param TargetVelocity motion velocity (units / seconds).
     * @return 1 if an error occurred.
     * @return 0 if the movement to the specified position was executed.
     */
    virtual int HexapodMoveIncrementalCtrlWithTargetVelocity(std::string TrajectoryType, double CoordX, double CoordY, double CoordZ, double TargetVelocity) = 0;

    /**
     * @brief This function prints the maximum velocity of carriage and the percent of the trajectory executable.
     * @param TrajectoryType trajectory type (Line, Arc or Rotation).
     * @param CoordX X-axis coordinate incremental displacement value.
     * @param CoordY Y-axis coordinate incremental displacement value.
     * @param CoordZ Z-axis coordinate incremental displacement value.
     * @return 1 if an error occurred, 0 otherwise.
     */
    virtual int HexapodMoveIncrementalCtrlLimitGet(std::string TrajectoryType, double CoordX, double CoordY, double CoordZ) = 0;

    /**
     * @brief This function prints the current positions of the Hxp the Work coordinate system with X, Y, Z, U, V, W values.
     * @return 1 if an error occurred.
     * @return 0 if an error did not occur.
     */
    virtual int getPosition() = 0;

    /**
     * @brief This function returns the current X-Axis position of the Hexapod.
     * @return X-Axis position.
     */
    virtual double getPositionX() = 0;

    /**
     * @brief This function returns the current Y-Axis position of the Hexapod.
     * @return Y-Axis position.
     */
    virtual double getPositionY() = 0;

    /**
     * @brief This function returns the current Z-Axis position of the Hexapod.
     * @return Z-Axis position.
     */
    virtual double getPositionZ() = 0;

    /**
     * @brief This function returns the current U-Axis position of the Hexapod.
     * @return U-Axis position.
     */
    virtual double getPositionU() = 0;

    /**
     * @brief This function returns the current V-Axis position of the Hexapod.
     * @return V-Axis position.
     */
    virtual double getPositionV() = 0;

    /**
     * @brief This function returns the current W-Axis position of the Hexapod.
     * @return W-Axis position.
     */
    virtual double getPositionW() = 0;

    /**
     * @brief This function stops the movement of the Hexapod.
     * @return 0 if stop was executed correcty.
     * @return 1 if stop was not executed correcty.
     */
    virtual int stopHxp() = 0;

    /**
     * @brief Set the Hxp Coordinates.
     * 
     * Set the parameters of the "hxpCoord_" struct variable.
     * 
     * @param CoordX X-axis coordinate.
     * @param CoordY Y-axis coordinate.
     * @param CoordZ Z-axis coordinate.
     * @param CoordU U-axis coordinate.
     * @param CoordV V-axis coordinate.
     * @param CoordW W-axis coordinate.
     */
    virtual void setHxpCoordinates(double CoordX, double CoordY, double CoordZ, double CoordU, double CoordV, double CoordW) = 0;

    /**
     * @brief Set the parameter "CoordX" of the "hxpCoord_" struct variable.
     * 
     * @param CoordX hexapod coordinate to set.
     */
    virtual void setCoordinateX(double CoordX) = 0;

    /**
     * @brief Set the parameter "CoordY" of the "hxpCoord_" struct variable.
     * 
     * @param CoordY hexapod coordinate to set.
     */
    virtual void setCoordinateY(double CoordY) = 0;

    /**
     * @brief Set the parameter "CoordZ" of the "hxpCoord_" struct variable.
     * 
     * @param CoordZ hexapod coordinate to set.
     */
    virtual void setCoordinateZ(double CoordZ) = 0;

    /**
     * @brief Set the parameter "CoordU" of the "hxpCoord_" struct variable.
     * 
     * @param CoordU hexapod coordinate to set.
     */
    virtual void setCoordinateU(double CoordU) = 0;

    /**
     * @brief Set the parameter "CoordV" of the "hxpCoord_" struct variable.
     * 
     * @param CoordV hexapod coordinate to set.
     */
    virtual void setCoordinateV(double CoordV) = 0;

    /**
     * @brief Set the parameter "CoordW" of the "hxpCoord_" struct variable.
     * 
     * @param CoordW hexapod coordinate to set.
     */
    virtual void setCoordinateW(double CoordW) = 0;

    /**
     * @brief Get the parameter "CoordX" of the "hxpCoord_" struct variable.
     * 
     * @param CoordX hexapod coordinate to get.
     */
    virtual double getCoordinateX() = 0;

    /**
     * @brief Get the parameter "CoordY" of the "hxpCoord_" struct variable.
     * 
     * @param CoordY hexapod coordinate to get.
     */
    virtual double getCoordinateY() = 0;

    /**
     * @brief Get the parameter "CoordZ" of the "hxpCoord_" struct variable.
     * 
     * @param CoordZ hexapod coordinate to get.
     */
    virtual double getCoordinateZ() = 0;

    /**
     * @brief Get the parameter "CoordU" of the "hxpCoord_" struct variable.
     * 
     * @param CoordU hexapod coordinate to get.
     */
    virtual double getCoordinateU() = 0;

    /**
     * @brief Get the parameter "CoordV" of the "hxpCoord_" struct variable.
     * 
     * @param CoordV hexapod coordinate to get.
     */
    virtual double getCoordinateV() = 0;

    /**
     * @brief Get the parameter "CoordW" of the "hxpCoord_" struct variable.
     * 
     * @param CoordW hexapod coordinate to get.
     */
    virtual double getCoordinateW() = 0;
};
