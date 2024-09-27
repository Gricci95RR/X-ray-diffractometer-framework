/**
 * @file ScanningHXP.cpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2022
 * @brief Implementation of Class ISCanning.
 * @version 0.1
 * @date 2022
 * 
 * @copyright © Copyright CERN 2018. All rights reserved. This software is released under a CERN proprietary software license.
 * Any permission to use it shall be granted in writing. Requests shall be addressed to CERN through mail-KT@cern.ch
 * 
 */

#include "ScanningHXP.hpp"

namespace scanning {

ScanningHXP::ScanningHXP(std::shared_ptr<IHXP> clientHxp,
                         std::shared_ptr<ISensors> clientSensors,
                         std::shared_ptr<IPostProcessing> clientPostProcessing):
    clientHxp_(clientHxp),
    clientSensors_(clientSensors),
    clientPostProcessing_(clientPostProcessing),
    stopMotor_(false) {
    spdlog::info("cTor ScanningHXP\n");
}

ScanningHXP::~ScanningHXP() {
    spdlog::info("dTor ScanningHXP\n");
}

void ScanningHXP::updateAxisPosition(double position) {
    spdlog::info("Method updateAxisPosition of Class ScanningHXP\n");
    switch (hxpAxisToScan_) {
    case 1:
        clientHxp_->setCoordinateX(position);
        break;
    case 2:
        clientHxp_->setCoordinateY(position);
        break;
    case 3:
        clientHxp_->setCoordinateZ(position);
        break;
    case 4:
        clientHxp_->setCoordinateU(position);
        break;
    case 5:
        clientHxp_->setCoordinateV(position);
        break;
    case 6:
        clientHxp_->setCoordinateW(position);
        break;
    default:
        spdlog::error("Invalid Axis.");
        break;
    }
}

double ScanningHXP::getAxisPosition() {
    spdlog::info("Method getAxisPosition of Class ScanningHXP\n");
    double position;
    switch (hxpAxisToScan_) {
    case 1:
        position = clientHxp_->getPositionX();
        return position;
        break;
    case 2:
        position = clientHxp_->getPositionY();
        return position;
        break;
    case 3:
        position = clientHxp_->getPositionZ();
        return position;
        break;
    case 4:
        position = clientHxp_->getPositionU();
        return position;
        break;
    case 5:
        position = clientHxp_->getPositionV();
        return position;
        break;
    case 6:
        position = clientHxp_->getPositionW();
        return position;
        break;
    default:
        spdlog::error("Invalid Axis.");
        return 0;
        break;
    }
}

bool ScanningHXP::checkReachingPosition(float currentPosition, float targetPosition) {
    float difference = targetPosition - currentPosition;
    if (difference > 0.1) {
        return false;
    } else {
        return true;
    }
}

bool ScanningHXP::relativeMotionHXP(double displacement) {
    bool resultMotion;
    switch (hxpAxisToScan_) {
    case 1:
        resultMotion = clientHxp_->HexapodMoveIncrementalCtrl("Line", displacement, 0, 0);
        return resultMotion;
        break;
    case 2:
        resultMotion = clientHxp_->HexapodMoveIncrementalCtrl("Line", 0, displacement, 0);
        return resultMotion;
        break;
    case 3:
        resultMotion = clientHxp_->HexapodMoveIncrementalCtrl("Line", 0, 0, displacement);
        return resultMotion;
        break;
    case 4:
        resultMotion = clientHxp_->HexapodMoveIncrementalCtrl("Rotation", displacement, 0, 0);
        return resultMotion;
        break;
    case 5:
        resultMotion = clientHxp_->HexapodMoveIncrementalCtrl("Rotation", 0, displacement, 0);
        return resultMotion;
        break;
    case 6:
        resultMotion = clientHxp_->HexapodMoveIncrementalCtrl("Rotation", 0, 0, displacement);
        return resultMotion;
        break;
    default:
        spdlog::error("Invalid Axis.");
        return 0;
        break;
    }
}

bool ScanningHXP::scan() {
    spdlog::info("Method scan of Class ScanningHXP\n");
    spdlog::debug("Scan parameters - Step Size: {}; Range: {}.\n", stepSize_, range_);
    double currentPosition = this->getAxisPosition();
    float finalPosition = currentPosition + range_;
    clientSensors_->startAcquisitionCrystal(filename_, eraseCsvContent_);
    clientSensors_->readXRaySensor(durationAcquisition_, clientHxp_->getPositionX(), clientHxp_->getPositionY(), clientHxp_->getPositionZ(), clientHxp_->getPositionU(), clientHxp_->getPositionV(), clientHxp_->getPositionW());
    if (fmodf(finalPosition, stepSize_) != 0.0  && finalPosition > 0) {
        int numSteps = round(finalPosition / stepSize_);
        stepSize_ = finalPosition / numSteps;  // Update stepSize if remainder of division is != 0
        spdlog::warn("Updated stepSize to: {}\n", stepSize_);
        spdlog::debug("Method startScanHxp::Crystal. Step Size: {} [UU]; Final Position: {} [UU]; Current Axis Position: {} [UU]\n", stepSize_, finalPosition, currentPosition);
    }
    if (finalPosition > currentPosition) {  // Forward Motion
        for (float nextPosition = currentPosition + stepSize_; nextPosition <= (finalPosition+0.001); nextPosition = nextPosition + stepSize_) {
            if (stopMotor_ != true) {
                spdlog::debug("Start Axis Movement to: {} (Forward)\n", nextPosition);
                this->updateAxisPosition(nextPosition);
                clientHxp_->setPositionAbsolute(clientHxp_->getCoordinateX(),
                                                clientHxp_->getCoordinateY(),
                                                clientHxp_->getCoordinateZ(),
                                                clientHxp_->getCoordinateU(),
                                                clientHxp_->getCoordinateV(),
                                                clientHxp_->getCoordinateW());
                clientSensors_->motionStabilizationTimer(100);
            } else {
                stopMotor_ = false;
                return true;
            }
            currentPosition = this->getAxisPosition();
            clientSensors_->readXRaySensor(durationAcquisition_, clientHxp_->getPositionX(), clientHxp_->getPositionY(), clientHxp_->getPositionZ(), clientHxp_->getPositionU(), clientHxp_->getPositionV(), clientHxp_->getPositionW());
            if (!this->checkReachingPosition(currentPosition, nextPosition)) {
                spdlog::error("Position {} not reached! The system is currently in position: {}\n", nextPosition, currentPosition);
                spdlog::debug("----------------------------------------------------------\n");
                return false;
            } else {
                spdlog::debug("Position {} reached!\n", currentPosition);
            }
            spdlog::debug("**************************************************************\n");
        }
    } else {  // Backward motion
        for (float nextPosition = currentPosition - stepSize_; nextPosition >= (finalPosition-0.001); nextPosition = nextPosition - stepSize_) {
            if (stopMotor_ != true) {
                spdlog::debug("Start Axis Movement to: {} (Forward)\n", nextPosition);
                this->updateAxisPosition(nextPosition);
                clientHxp_->setPositionAbsolute(clientHxp_->getCoordinateX(),
                                                clientHxp_->getCoordinateY(),
                                                clientHxp_->getCoordinateZ(),
                                                clientHxp_->getCoordinateU(),
                                                clientHxp_->getCoordinateV(),
                                                clientHxp_->getCoordinateW());
                clientSensors_->motionStabilizationTimer(100);
            } else {
                stopMotor_ = false;
                return true;
            }
            currentPosition = this->getAxisPosition();
            clientSensors_->readXRaySensor(durationAcquisition_, clientHxp_->getPositionX(), clientHxp_->getPositionY(), clientHxp_->getPositionZ(), clientHxp_->getPositionU(), clientHxp_->getPositionV(), clientHxp_->getPositionW());
            if (!this->checkReachingPosition(currentPosition, nextPosition)) {
                spdlog::error("Position {} not reached! The system is currently in position: {}\n", nextPosition, currentPosition);
                spdlog::debug("----------------------------------------------------------\n");
                return false;
            } else {
                spdlog::debug("Position {} reached!\n", currentPosition);
            }
            spdlog::debug("**************************************************************\n");
        }
    }
    spdlog::debug("#################################################################\n");
    if (showPlot_) {
        clientPostProcessing_->executeScript2(pathToPlotScanScript_.string(), filename_, std::to_string(hxpAxisToScan_));
    }
    return true;
}

bool ScanningHXP::scanRelative() {
    spdlog::info("Method scan of Class ScanningHXP\n");
    spdlog::debug("Scan parameters - Step Size: {}; Range: {}.\n", stepSize_, range_);
    double currentPosition = this->getAxisPosition();
    float finalPosition = currentPosition + range_;
    clientSensors_->startAcquisitionCrystal(filename_, eraseCsvContent_);
    clientSensors_->readXRaySensor(durationAcquisition_, clientHxp_->getPositionX(), clientHxp_->getPositionY(), clientHxp_->getPositionZ(), clientHxp_->getPositionU(), clientHxp_->getPositionV(), clientHxp_->getPositionW());
    if (fmodf(finalPosition, stepSize_) != 0.0  && finalPosition > 0) {
        int numSteps = round(finalPosition / stepSize_);
        stepSize_ = finalPosition / numSteps;  // Update stepSize if remainder of division is != 0
        spdlog::warn("Updated stepSize to: {}\n", stepSize_);
        spdlog::debug("Method startScanHxp::Crystal. Step Size: {} [UU]; Final Position: {} [UU]; Current Axis Position: {} [UU]\n", stepSize_, finalPosition, currentPosition);
    }
    if (finalPosition > currentPosition) {  // Forward Motion
        for (float nextPosition = currentPosition + stepSize_; nextPosition <= (finalPosition+0.001); nextPosition = nextPosition + stepSize_) {
            if (stopMotor_ != true) {
                spdlog::debug("Start Axis Movement to: {} (Forward)\n", nextPosition);
                this->updateAxisPosition(nextPosition);
                /* Execute relative step */
                this->relativeMotionHXP(stepSize_);
                clientSensors_->motionStabilizationTimer(100);
            } else {
                stopMotor_ = false;
                return true;
            }
            currentPosition = this->getAxisPosition();
            clientSensors_->readXRaySensor(durationAcquisition_, clientHxp_->getPositionX(), clientHxp_->getPositionY(), clientHxp_->getPositionZ(), clientHxp_->getPositionU(), clientHxp_->getPositionV(), clientHxp_->getPositionW());
            if (!this->checkReachingPosition(currentPosition, nextPosition)) {
                spdlog::error("Position {} not reached! The system is currently in position: {}\n", nextPosition, currentPosition);
                spdlog::debug("----------------------------------------------------------\n");
                return false;
            } else {
                spdlog::debug("Position {} reached!\n", currentPosition);
            }
            spdlog::debug("**************************************************************\n");
        }
    } else {  // Backward motion
        for (float nextPosition = currentPosition - stepSize_; nextPosition >= (finalPosition-0.001); nextPosition = nextPosition - stepSize_) {
            if (stopMotor_ != true) {
                spdlog::debug("Start Axis Movement to: {} (Forward)\n", nextPosition);
                this->updateAxisPosition(nextPosition);
                /* Execute relative step */
                this->relativeMotionHXP(stepSize_);
                clientSensors_->motionStabilizationTimer(100);
            } else {
                stopMotor_ = false;
                return true;
            }
            currentPosition = this->getAxisPosition();
            clientSensors_->readXRaySensor(durationAcquisition_, clientHxp_->getPositionX(), clientHxp_->getPositionY(), clientHxp_->getPositionZ(), clientHxp_->getPositionU(), clientHxp_->getPositionV(), clientHxp_->getPositionW());
            if (!this->checkReachingPosition(currentPosition, nextPosition)) {
                spdlog::error("Position {} not reached! The system is currently in position: {}\n", nextPosition, currentPosition);
                spdlog::debug("----------------------------------------------------------\n");
                return false;
            } else {
                spdlog::debug("Position {} reached!\n", currentPosition);
            }
            spdlog::debug("**************************************************************\n");
        }
    }
    spdlog::debug("#################################################################\n");
    if (showPlot_) {
        clientPostProcessing_->executeScript2(pathToPlotScanScript_.string(), filename_, std::to_string(hxpAxisToScan_));
    }
    return true;
}

void ScanningHXP::setupAlignmentParameters(float stepSize,
                                           float range,
                                           int durationAcquisition,
                                           std::string filename,
                                           bool eraseCsvContent,
                                           bool showPlot) {
    this->setStepSize(stepSize);
    this->setRange(range);
    this->setDurationAcquisition(durationAcquisition);
    std::string extension = filename.substr(filename.length() - 4);
    this->setFilename(filename);
    this->setEraseCsvContent(eraseCsvContent);
    this->setShowPlot(showPlot);
}

bool ScanningHXP::stop() {
    spdlog::info("Method stop of class ScanningHXP\n");
    int result_stop = clientHxp_->stopHxp();
    stopMotor_ = true;
    return true;
    /*
    if (result_stop == 0) {
        
        return true;
    } else {
        return false;
    }
    */
}

float ScanningHXP::getStepSize() {
    return stepSize_;
}

void ScanningHXP::setStepSize(float stepSize) {
    stepSize_ = stepSize;
}

int ScanningHXP::getDurationAcquisition() {
    return durationAcquisition_;
}

void ScanningHXP::setDurationAcquisition(int durationAcquisition) {
    durationAcquisition_ = durationAcquisition;
}

std::string ScanningHXP::getFilename() {
    return filename_;
}

void ScanningHXP::setFilename(std::string filename) {
    filename = this->checkExtension(filename);
    filename = this->replaceSpacesWithUnderscores(filename);
    filename_ = filename;
}


bool ScanningHXP::getEraseCsvContent() {
    return eraseCsvContent_;
}

void ScanningHXP::setEraseCsvContent(bool eraseCsvContent) {
    eraseCsvContent_ = eraseCsvContent;
}

float ScanningHXP::getRange() {
    return range_;
}

void ScanningHXP::setRange(float range) {
    range_ = range;
}

bool ScanningHXP::getShowPlot() {
    return showPlot_;
}

void ScanningHXP::setShowPlot(bool showPlot) {
    showPlot_ = showPlot;
}

std::string ScanningHXP::replaceSpacesWithUnderscores(const std::string& filename) {
    std::string result = filename;
    for (char& ch : result) {
        if (ch == ' ') {
            ch = '_'; // Replace space with underscore
        }
    }
    return result;
}

std::string ScanningHXP::checkExtension(std::string filename) {
    std::string extension = filename.substr(filename.length() - 4);
    if (extension != ".csv") {
        return filename + ".csv";
    } else {
        return filename;
    }
}

}  // namespace scanning
