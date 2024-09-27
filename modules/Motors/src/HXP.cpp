/**
 * @file HXP.cpp
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

#include <hxp_drivers.h>
#include "spdlog/spdlog.h"

#include <iostream>
#include <string>

#include "HXP.hpp"

HXP::HXP(std::string iPAddress, int nPort): iPAddress_(iPAddress), nPort_(nPort), socketID_(0) {
    spdlog::info("cTor HXP\n");
}

HXP::~HXP() {
    spdlog::info("dTor HXP\n");
}

void HXP::setiPAddress(std::string iPAddress) {
    iPAddress_ = iPAddress;
}

void HXP::setNPort(int nPort) {
    nPort_ = nPort;
}

std::string HXP::getiPAddress() {
    return iPAddress_;
}

int HXP::getNPort() {
    return nPort_;
}

int HXP::getsocketID() {
    return socketID_;
}

int HXP::connect(int dTimeOut, std::string pGroup) {
    spdlog::info("Method 'connect' of class HXP\n");
    pGroup_ = pGroup;  // Group name
    std::string iPAddress_temp_str = getiPAddress();
    char *iPAddress_temp_ch = iPAddress_temp_str.data();  // Conversion string to char *
    socketID_ = -1;
    socketID_ = TCP_ConnectToServer(iPAddress_temp_ch, getNPort(), dTimeOut);
    if (-1 == socketID_) {
        spdlog::debug("Connection to @ {}, port = {} failed\n", iPAddress_temp_ch, getNPort());
        return 1;
    } else {
        spdlog::debug("Connected to target!\n");
        return 0;
    }
}

int HXP::disconnect() {
    spdlog::info("Method disconnect of class HXP\n");
    /* TCP / IP disconnection */
    spdlog::debug("Disconnection of socket: {}\n", socketID_);
    TCP_CloseSocket(socketID_);
    spdlog::debug("Disconnected from target!\n");
    return 0;
}

int HXP::setPositionAbsolute(double CoordX, double CoordY, double CoordZ, double CoordU, double CoordV, double CoordW) {
    spdlog::info("Method 'setPositionAbsolute' of class HXP\n");
    spdlog::info("Moving to Coordinates: X: {}; Y: {}; Z: {}; U: {}; V: {}; W: {}.\n", CoordX, CoordY, CoordZ, CoordU, CoordV, CoordW);
    this->setHxpCoordinates(CoordX, CoordY, CoordZ, CoordU, CoordV, CoordW);
    setcoordinateSystem("Work");
    std::string pGroup_temp_str = pGroup_;
    char *pGroup_temp_ch = pGroup_temp_str.data();  // Conversion string to char *
    std::string coordinateSystem_temp_str = coordinateSystem_;
    char *coordinateSystem_temp_ch = coordinateSystem_temp_str.data();  // Conversion string to char *
    // Move Absolute
    int error = HexapodMoveAbsolute(socketID_,
                                    pGroup_temp_ch,
                                    coordinateSystem_temp_ch,
                                    CoordX,
                                    CoordY,
                                    CoordZ,
                                    CoordU,
                                    CoordV,
                                    CoordW);
    if (0 != error) {
        spdlog::error("Error {} in HexapodMoveAbsolute.\n", error);
        return 1;
    } else {
        spdlog::info("Absolute movement executed!\n");
        return 0;
    }
}

int HXP::setPositionAbsolute() {
    spdlog::info("Method 'setPositionAbsolute' of class HXP\n");
    float CoordX = this->getCoordinateX();
    float CoordY = this->getCoordinateY();
    float CoordZ = this->getCoordinateZ();
    float CoordU = this->getCoordinateU();
    float CoordV = this->getCoordinateV();
    float CoordW = this->getCoordinateW();
    spdlog::debug("Moving to Coordinates: X: {}; Y: {}; Z: {}; U: {}; V: {}; W: {}.\n", CoordX, CoordY, CoordZ, CoordU, CoordV, CoordW);
    this->setHxpCoordinates(CoordX, CoordY, CoordZ, CoordU, CoordV, CoordW);
    setcoordinateSystem("Work");
    std::string pGroup_temp_str = pGroup_;
    char *pGroup_temp_ch = pGroup_temp_str.data();  // Conversion string to char *
    std::string coordinateSystem_temp_str = coordinateSystem_;
    char *coordinateSystem_temp_ch = coordinateSystem_temp_str.data();  // Conversion string to char *
    // Move Absolute
    int error = HexapodMoveAbsolute(socketID_,
                                    pGroup_temp_ch,
                                    coordinateSystem_temp_ch,
                                    CoordX,
                                    CoordY,
                                    CoordZ,
                                    CoordU,
                                    CoordV,
                                    CoordW);
    if (0 != error) {
        spdlog::error("Error {} in HexapodMoveAbsolute.\n", error);
        return 1;
    } else {
        spdlog::debug("Absolute movement executed!\n");
        return 0;
    }
}

int HXP::goHome() {
    /* Kill group */
    spdlog::info("Method goHome of Class HXP\n");
    spdlog::debug("Homing HXP\n");
    std::string pGroup_temp_str = pGroup_;
    char *pGroup_temp_ch = pGroup_temp_str.data();  // Conversion string to char *
    int error = GroupKill(socketID_, pGroup_temp_ch);
    if (0 != error) {
        spdlog::error("Error in GroupKill.\n");
        return 1;
    }
    /* Initialize group */
    error = GroupInitializeWithEncoderCalibration(socketID_, pGroup_temp_ch);
    if (0 != error) {
        spdlog::error("Error in GroupInitialize.\n");
        return 1;
    }
    // Search home group
    error = GroupHomeSearch(socketID_, pGroup_temp_ch);
    if (0 != error) {
        spdlog::error("Error {} in GroupHomeSearch.\n", error);
        return 1;
    } else {
        spdlog::debug("HXP Home Position Reached!\n");
        return 0;
    }
}

int HXP::HexapodMoveIncrementalCtrl(std::string TrajectoryType, double CoordX, double CoordY, double CoordZ) {
    spdlog::info("Method 'HexapodMoveIncrementalCtrl' of class HXP\n");
    setcoordinateSystem("Tool");
    std::string pGroup_temp_str = pGroup_;
    char *pGroup_temp_ch = pGroup_temp_str.data();  // Conversion string to char *
    std::string coordinateSystem_temp_str = coordinateSystem_;
    char *coordinateSystem_temp_ch = coordinateSystem_temp_str.data();  // Conversion string to char *
    std::string TrajectoryType_temp_str = TrajectoryType;
    char *TrajectoryType_temp_ch = TrajectoryType_temp_str.data();  // Conversion string to char *
    int error = HexapodMoveIncrementalControl(socketID_, pGroup_temp_ch, coordinateSystem_temp_ch, TrajectoryType_temp_ch, CoordX, CoordY, CoordZ);
    if (0 != error) {
        spdlog::error("Error {} in HexapodMoveIncrementalControl.\n", error);
        return 1;
    } else {
        spdlog::debug("HexapodMoveIncrementalControl executed!\n");
        return 0;
    }
}

int HXP::HexapodMoveIncrementalCtrlWithTargetVelocity(std::string TrajectoryType, double CoordX, double CoordY, double CoordZ, double TargetVelocity) {
    spdlog::info("Method 'HexapodMoveIncrementalCtrlWithTargetVelocity' of class HXP\n");
    setcoordinateSystem("Work");
    std::string pGroup_temp_str = pGroup_;
    char *pGroup_temp_ch = pGroup_temp_str.data();  // Conversion string to char *
    std::string coordinateSystem_temp_str = coordinateSystem_;
    char *coordinateSystem_temp_ch = coordinateSystem_temp_str.data();  // Conversion string to char *
    std::string TrajectoryType_temp_str = TrajectoryType;
    char *TrajectoryType_temp_ch = TrajectoryType_temp_str.data();  // Conversion string to char *
    int error = HexapodMoveIncrementalControlWithTargetVelocity(socketID_, pGroup_temp_ch, coordinateSystem_temp_ch, TrajectoryType_temp_ch, CoordX, CoordY, CoordZ, TargetVelocity);
    if (0 != error) {
        spdlog::error("Error {} in HexapodMoveIncrementalControlWithTargetVelocity.\n", error);
        return 1;
    } else {
        spdlog::debug("HexapodMoveIncrementalControlWithTargetVelocity executed!\n");
        return 0;
    }
}

int HXP::HexapodMoveIncrementalCtrlLimitGet(std::string TrajectoryType, double CoordX, double CoordY, double CoordZ) {
    spdlog::info("Method 'HexapodMoveIncrementalCtrlLimitGet' of class HXP\n");
    double  maxvel;
    double  trajperc;
    std::string pGroup_temp_str = pGroup_;
    char *pGroup_temp_ch = pGroup_temp_str.data();  // Conversion string to char *
    std::string coordinateSystem_temp_str = coordinateSystem_;
    char *coordinateSystem_temp_ch = coordinateSystem_temp_str.data();  // Conversion string to char *
    std::string TrajectoryType_temp_str = TrajectoryType;
    char *TrajectoryType_temp_ch = TrajectoryType_temp_str.data();  // Conversion string to char *
    int error = HexapodMoveIncrementalControlLimitGet(socketID_, pGroup_temp_ch, coordinateSystem_temp_ch, TrajectoryType_temp_ch, 0, 10, 0, &maxvel, &trajperc);
    if (0 != error) {
        spdlog::error("Error {} in HexapodMoveIncrementalControlLimitGet.\n", error);
        return 1;
    } else {
        spdlog::debug("HexapodMoveIncrementalControlLimitGet executed!\n");
        spdlog::debug("maxvel: {}, trajperc: {} \n", maxvel, trajperc);
        return 0;
    }
}

int HXP::getPosition() {
    spdlog::info("Method 'get_Position' of class HXP\n");
    double  CurrentPosition[6];
    std::string pGroup_temp_str = pGroup_;
    char *pGroup_temp_ch = pGroup_temp_str.data();  // Conversion string to char *
    int error = GroupPositionCurrentGet(socketID_, pGroup_temp_ch, 6, CurrentPosition);
    if (0 != error) {
        spdlog::error("Error {} in GroupPositionCurrentGet.\n", error);
        return 1;
    } else {
        spdlog::debug("GroupPositionCurrentGet executed!\n");
        spdlog::debug("X: {} \n", CurrentPosition[0]);
        spdlog::debug("Y: {} \n", CurrentPosition[1]);
        spdlog::debug("Z: {} \n", CurrentPosition[2]);
        spdlog::debug("U: {} \n", CurrentPosition[3]);
        spdlog::debug("V: {} \n", CurrentPosition[4]);
        spdlog::debug("W: {} \n", CurrentPosition[5]);
        return 0;
    }
}

double HXP::getPositionX() {
    //  spdlog::info("Method 'get_PositionX' of class HXP\n");
    double  CurrentPosition[6];
    std::string pGroup_temp_str = pGroup_;
    char *pGroup_temp_ch = pGroup_temp_str.data();  // Conversion string to char *
    int error = GroupPositionCurrentGet(socketID_, pGroup_temp_ch, 6, CurrentPosition);
    if (0 != error) {
        spdlog::error("Error {} in GroupPositionCurrentGet.\n", error);
        return 00;
    } else {
        spdlog::debug("GroupPositionCurrentGet executed!\n");
        double output = CurrentPosition[0];
        return output;
    }
}

double HXP::getPositionY() {
    //  spdlog::info("Method 'get_PositionY' of class HXP\n");
    double  CurrentPosition[6];
    std::string pGroup_temp_str = pGroup_;
    char *pGroup_temp_ch = pGroup_temp_str.data();  // Conversion string to char *
    int error = GroupPositionCurrentGet(socketID_, pGroup_temp_ch, 6, CurrentPosition);
    if (0 != error) {
        spdlog::error("Error {} in GroupPositionCurrentGet.", error);
        return 00;
    } else {
        spdlog::debug("GroupPositionCurrentGet executed!\n");
        double output = CurrentPosition[1];
        return output;
    }
}

double HXP::getPositionZ() {
    //  spdlog::info("Method 'get_PositionZ' of class HXP\n");
    double  CurrentPosition[6];
    std::string pGroup_temp_str = pGroup_;
    char *pGroup_temp_ch = pGroup_temp_str.data();  // Conversion string to char *
    int error = GroupPositionCurrentGet(socketID_, pGroup_temp_ch, 6, CurrentPosition);
    if (0 != error) {
        spdlog::error("Error {} in GroupPositionCurrentGet.\n", error);
        return 00;
    } else {
        spdlog::debug("GroupPositionCurrentGet executed!\n");
        double output = CurrentPosition[2];
        return output;
    }
}

double HXP::getPositionU() {
    //  spdlog::info("Method 'get_PositionU' of class HXP\n");
    double  CurrentPosition[6];
    std::string pGroup_temp_str = pGroup_;
    char *pGroup_temp_ch = pGroup_temp_str.data();  // Conversion string to char *
    int error = GroupPositionCurrentGet(socketID_, pGroup_temp_ch, 6, CurrentPosition);
    if (0 != error) {
        spdlog::error("Error {} in GroupPositionCurrentGet.\n", error);
        return 00;
    } else {
        spdlog::debug("GroupPositionCurrentGet executed!\n");
        double output = CurrentPosition[3];
        return output;
    }
}

double HXP::getPositionV() {
    //  spdlog::info("Method 'get_PositionV' of class HXP\n");
    double  CurrentPosition[6];
    std::string pGroup_temp_str = pGroup_;
    char *pGroup_temp_ch = pGroup_temp_str.data();  // Conversion string to char *
    int error = GroupPositionCurrentGet(socketID_, pGroup_temp_ch, 6, CurrentPosition);
    if (0 != error) {
        spdlog::error("Error {} in GroupPositionCurrentGet.\n", error);
        return 00;
    } else {
        spdlog::debug("GroupPositionCurrentGet executed!\n");
        double output = CurrentPosition[4];
        return output;
    }
}

double HXP::getPositionW() {
    //  spdlog::info("Method 'get_PositionW' of class HXP\n");
    double  CurrentPosition[6];
    std::string pGroup_temp_str = pGroup_;
    char *pGroup_temp_ch = pGroup_temp_str.data();  // Conversion string to char *
    int error = GroupPositionCurrentGet(socketID_, pGroup_temp_ch, 6, CurrentPosition);
    if (0 != error) {
        spdlog::error("Error {} in GroupPositionCurrentGet.\n", error);
        return 00;
    } else {
        spdlog::debug("GroupPositionCurrentGet executed!\n");
        double output = CurrentPosition[5];
        return output;
    }
}

int HXP::stopHxp() {
    spdlog::info("Method stopHxp of class HXP\n");
    std::string pGroup_temp_str = pGroup_;
    char *pGroup_temp_ch = pGroup_temp_str.data();  // Conversion string to char *
    int error = GroupMoveAbort(getsocketID(), pGroup_temp_ch);
    while (error == -1) {
        spdlog::error("Busy Socket!\n", error);
        error = GroupMoveAbort(getsocketID(), pGroup_temp_ch);
    }
    if (0 != error) {
        spdlog::error("Error {} in GroupMoveAbort.\n", error);
        return 1;
    } else {
        spdlog::debug("GroupMoveAbort executed!\n");
        return 0;
    }
}

void HXP::setcoordinateSystem(std::string coordinateSystem) {
    coordinateSystem_ = coordinateSystem;
}

std::string HXP::getcoordinateSystem() {
    return coordinateSystem_;
}

void HXP::setHxpCoordinates(double CoordX, double CoordY, double CoordZ, double CoordU, double CoordV, double CoordW) {
    hxpCoord_.CoordX = CoordX;
    hxpCoord_.CoordY = CoordY;
    hxpCoord_.CoordZ = CoordZ;
    hxpCoord_.CoordU = CoordU;
    hxpCoord_.CoordV = CoordV;
    hxpCoord_.CoordW = CoordW;
}

void HXP::setCoordinateX(double CoordX) {
    hxpCoord_.CoordX = CoordX;
}

void HXP::setCoordinateY(double CoordY) {
    hxpCoord_.CoordY = CoordY;
}

void HXP::setCoordinateZ(double CoordZ) {
    hxpCoord_.CoordZ = CoordZ;
}

void HXP::setCoordinateU(double CoordU) {
    hxpCoord_.CoordU = CoordU;
}

void HXP::setCoordinateV(double CoordV) {
    hxpCoord_.CoordV = CoordV;
}

void HXP::setCoordinateW(double CoordW) {
    hxpCoord_.CoordW = CoordW;
}

double HXP::getCoordinateX() {
    return hxpCoord_.CoordX;
}

double HXP::getCoordinateY() {
    return hxpCoord_.CoordY;
}

double HXP::getCoordinateZ() {
    return hxpCoord_.CoordZ;
}

double HXP::getCoordinateU() {
    return hxpCoord_.CoordU;
}

double HXP::getCoordinateV() {
    return hxpCoord_.CoordV;
}

double HXP::getCoordinateW() {
    return hxpCoord_.CoordW;
}
