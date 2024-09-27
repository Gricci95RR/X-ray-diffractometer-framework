/**
 * @file HXPMock.hpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2021
 * @brief Class Implementation of "IHXP" used to mock the method of the parent class.
 * @version 0.1
 * @date 2021
 * 
 * @copyright © Copyright CERN 2018. All rights reserved. This software is released under a CERN proprietary software license.
 * Any permission to use it shall be granted in writing. Requests shall be addressed to CERN through mail-KT@cern.ch
 * 
 */

#pragma once

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <string>

#include "IHXP.hpp"

using testing::_;
using testing::Return;
using testing::NiceMock;
using testing::Invoke;

/**
 * @class HXPMock
 * @brief Class Implementation of "IHXP" used for testing methods belonging to the parent Class.
 */
class HXPMock : public IHXP {
 public:
  /**
   * @brief Construct a new mock method1 object for setiPAddress method.
   * 
   */
  MOCK_METHOD1(setiPAddress,
  void(std::string iPAddress));
  /**
   * @brief Construct a new mock method1 object for setNPort method.
   * 
   */
  MOCK_METHOD1(setNPort,
  void(int nPort));
  MOCK_METHOD0(getiPAddress,
  std::string());
  MOCK_METHOD0(getNPort,
  int());
  MOCK_METHOD2(connect,
  int(int dTimeOut, std::string pGroup));
  MOCK_METHOD0(goHome,
  int());
  MOCK_METHOD0(disconnect,
  int());
  MOCK_METHOD6(setPositionAbsolute,
  int(double CoordX, double CoordY, double CoordZ, double CoordU, double CoordV, double CoordW));
  MOCK_METHOD0(setPositionAbsolute,
  int());
  MOCK_METHOD4(HexapodMoveIncrementalCtrl,
  int(std::string TrajectoryType, double CoordX, double CoordY, double CoordZ));
  MOCK_METHOD5(HexapodMoveIncrementalCtrlWithTargetVelocity,
  int(std::string TrajectoryType, double CoordX, double CoordY, double CoordZ, double TargetVelocity));
  MOCK_METHOD4(HexapodMoveIncrementalCtrlLimitGet,
  int(std::string TrajectoryType, double CoordX, double CoordY, double CoordZ));
  MOCK_METHOD0(getPosition,
  int());
  MOCK_METHOD0(getPositionX,
  double());
  MOCK_METHOD0(getPositionY,
  double());
  MOCK_METHOD0(getPositionZ,
  double());
  MOCK_METHOD0(getPositionU,
  double());
  MOCK_METHOD0(getPositionV,
  double());
  MOCK_METHOD0(getPositionW,
  double());
  MOCK_METHOD1(setCoordinateX,
  void(double coord));
  MOCK_METHOD1(setCoordinateY,
  void(double coord));
  MOCK_METHOD1(setCoordinateZ,
  void(double coord));
  MOCK_METHOD1(setCoordinateU,
  void(double coord));
  MOCK_METHOD1(setCoordinateV,
  void(double coord));
  MOCK_METHOD1(setCoordinateW,
  void(double coord));
  MOCK_METHOD0(getCoordinateX,
  double());
  MOCK_METHOD0(getCoordinateY,
  double());
  MOCK_METHOD0(getCoordinateZ,
  double());
  MOCK_METHOD0(getCoordinateU,
  double());
  MOCK_METHOD0(getCoordinateV,
  double());
  MOCK_METHOD0(getCoordinateW,
  double());
  MOCK_METHOD0(stopHxp,
  int());
  MOCK_METHOD6(setHxpCoordinates,
  void(double CoordX, double CoordY, double CoordZ, double CoordU, double CoordV, double CoordW));
};
