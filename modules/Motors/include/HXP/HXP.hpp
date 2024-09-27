/**
 * @file HXP.hpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2021
 * @brief Class Implementation of "IHXP" used to control the hexapod robot.
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

#include "IHXP.hpp"

/**
 * @class HXP
 * @brief Class Implementation of "IHXP" used to control the hexapod robot.
 */
class HXP: public IHXP {
 private:
  std::string iPAddress_;  /**< The IP address of the hexapod controller. */
  int nPort_;  /**< The port number of the hexapod controller. */
  int socketID_;  /**< The socket ID of the connection to the hexapod controller. */
  std::string pGroup_;  /**< The group name used to control the hexapod. */
  std::string coordinateSystem_;  /**< The coordinate system used to control the hexapod.*/
  /**
   * @struct HxpCoordinates
   * @brief Struct containing the 6-axes hexapod coordinates.
   */
  struct HxpCoordinates {
        double CoordX = 0;
        double CoordY = 0;
        double CoordZ = 0;
        double CoordU = 0;
        double CoordV = 0;
        double CoordW = 0;
  };
  HxpCoordinates hxpCoord_;  /**< Structure variable of type HxpCoordinates, containing the target position of the hexapod. */

 public:
  /**
  * @brief Construct a new HXP object.
  * 
  * @param iPAddress IP Address of the HXP controller.
  * @param nPort port number of the TCP connection.
  */
  HXP(std::string iPAddress, int nPort);
  ~HXP();
  void setiPAddress(std::string iPAddress) override;
  void setNPort(int nPort) override;
  std::string getiPAddress() override;
  int getNPort() override;
  int connect(int dTimeOut, std::string pGroup) override;
  int disconnect() override;
  int setPositionAbsolute(double CoordX, double CoordY, double CoordZ, double CoordU, double CoordV, double CoordW) override;
  int setPositionAbsolute() override;
  int goHome() override;
  int HexapodMoveIncrementalCtrl(std::string TrajectoryType, double CoordX, double CoordY, double CoordZ) override;
  int HexapodMoveIncrementalCtrlWithTargetVelocity(std::string TrajectoryType, double CoordX, double CoordY, double CoordZ, double TargetVelocity) override;
  int HexapodMoveIncrementalCtrlLimitGet(std::string TrajectoryType, double CoordX, double CoordY, double CoordZ) override;
  int getPosition() override;
  double getPositionX() override;
  double getPositionY() override;
  double getPositionZ() override;
  double getPositionU() override;
  double getPositionV() override;
  double getPositionW() override;
  void setCoordinateX(double CoordX) override;
  void setCoordinateY(double CoordY) override;
  void setCoordinateZ(double CoordZ) override;
  void setCoordinateU(double CoordU) override;
  void setCoordinateV(double CoordV) override;
  void setCoordinateW(double CoordW) override;
  double getCoordinateX() override;
  double getCoordinateY() override;
  double getCoordinateZ() override;
  double getCoordinateU() override;
  double getCoordinateV() override;
  double getCoordinateW() override;
  int stopHxp() override;
  void setHxpCoordinates(double CoordX, double CoordY, double CoordZ, double CoordU, double CoordV, double CoordW) override;
  /**
  * @brief Get the Socket identifier used for TCP connection.
  * @return integer representing the socket identifier.
  */
  int getsocketID();
  /**
  * @brief Set coordinate system (Tool or Work).
  * 
  * @details
  * The "Tool" coordinate system is used for the Hexapod robot,
  * which has an origin located at the center of the top surface of the carriage,
  * and a Z-axis that is perpendicular to the carriage.
  * It is used to define the position and orientation of the tool or workpiece with respect to the Hexapod's end effector.
  * 
  * The "Work" coordinate system is used to determine the position of the Hexapod carriage using the "Tool" Coordinate System.
  * This is a fixed reference coordinate system that doesn't move with the Hexapod,
  * and it's used to describe where the Hexapod is located in the larger context of the work area.
  * 
  * In other words, the position of the Hexapod is always described relative to the Tool Coordinate System,
  * which is located inside the Work Coordinate System. This helps us keep track of where the Hexapod is and where it's moving.
  * 
  * @param coordinateSystem std::string representing the coordinate system.
  */
  void setcoordinateSystem(std::string coordinateSystem);
  /**
  * @brief Get coordinate system (Tool or Work).
  * @return std::string representing the coordinate system.
  */
  std::string getcoordinateSystem();
};
