/**
 * @file ScanningStepper.cpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2022
 * @brief Implementation of Class ISCanning.
 * @version 0.1
 * @date 2022
 * 
 * @copyright © Copyright CERN 2018. All rights reserved. This software is released under a CERN proprietary software license.
 * Any permission to use it shall be granted in writing. Requests shall be addressed to CERN through mail-KT@cern.ch
 * 
 */

#include "ScanningStepper.hpp"

namespace scanning {

ScanningStepper::ScanningStepper(std::shared_ptr<IMotor> clientStepper,
                   std::shared_ptr<ISensors> clientSensors,
                   std::shared_ptr<IPostProcessing> clientPostProcessing):
                   clientStepper_(clientStepper),
                   clientSensors_(clientSensors),
                   clientPostProcessing_(clientPostProcessing),
                   stopMotor_(false) {
    spdlog::info("cTor ScanningStepper\n");
}

ScanningStepper::~ScanningStepper() {
    spdlog::info("dTor ScanningStepper\n");
}

bool ScanningStepper::stop() {
    spdlog::info("Method stop of class ScanningStepper\n");
    int result_stop = clientStepper_->softStop();
    if (result_stop == 0) {
        stopMotor_ = true;
        return true;
    } else {
        return false;
    }
}

bool ScanningStepper::checkReachingPosition(float currentPosition, float targetPosition) {
    float difference = targetPosition - currentPosition;
    if (difference > 0.1) {
        return false;
    } else {
        return true;
    }
}

bool ScanningStepper::scanRelative() {
    return true;
}

bool ScanningStepper::scan() {
    spdlog::info("Method scan of Class ScanningStepper\n");
    spdlog::debug("Scan parameters - Step Size: {}; Range: {}.\n", stepSize_, range_);
    float stepSize = stepSize_;
    std::string dataXRaySensor;
    clientSensors_->startAcquisitionSingleStepper(filename_, eraseCsvContent_);
    float currentPosition = clientStepper_->getPositionUserUnits();
    float finalPosition = currentPosition + range_;
    dataXRaySensor = clientSensors_->readXRaySensor(durationAcquisition_, currentPosition);  // 1st Read X-Ray Sensor
    spdlog::debug("Method startScan. Step Size: {} [UU]; Final Position: {} [UU]; Current Position: {} [UU]\n", stepSize, finalPosition, currentPosition);
    if (fmodf(finalPosition, stepSize) != 0.0 && finalPosition > 0) {
        int numSteps = round(finalPosition / stepSize);
        stepSize = finalPosition / numSteps;  // Update stepSize if remainder of division is != 0
        spdlog::warn("Updated stepSize to: {}\n", stepSize);
        spdlog::debug("Method startScan. Step Size: {} [UU]; Final Position: {} [UU]; Current Position: {} [UU]\n", stepSize, finalPosition, currentPosition);
    }
    if (finalPosition > currentPosition) {  // Forward Motion
        for (float nextPosition = currentPosition + stepSize; nextPosition <= finalPosition + stepSize; nextPosition = nextPosition + stepSize) {
            spdlog::debug("Start Movement to: {} (Forward)\n", nextPosition);
            if (stopMotor_ != true) {
                int result_moveCalibratedMotor = clientStepper_->moveCalibratedMotor(nextPosition);
                if (result_moveCalibratedMotor != 0) {
                    return false;
                }
            } else {
                stopMotor_ = false;
                return true;
            }
            currentPosition = clientStepper_->getPositionUserUnits();  // Read Position after move
            dataXRaySensor = clientSensors_->readXRaySensor(durationAcquisition_, currentPosition);  // Read X-Ray Sensor
            if (!this->checkReachingPosition(currentPosition, nextPosition)) {
                spdlog::error("Position {} not reached! The system is currently in position: {}\n", nextPosition, currentPosition);
                spdlog::debug("----------------------------------------------------------\n");
                return false;
            } else {
                spdlog::debug("Position {} reached!\n", currentPosition);
            }
            spdlog::debug("**************************************************************\n");
        }  // end for
    } else {  //  Backward Motion
        for (float nextPosition = currentPosition - stepSize; nextPosition >= finalPosition; nextPosition = nextPosition - stepSize) {
            spdlog::debug("Start Movement to: {} (Backward)\n", nextPosition);
            if (stopMotor_ != true) {
                int result_moveCalibratedMotor = clientStepper_->moveCalibratedMotor(nextPosition);
                if (result_moveCalibratedMotor != 0) {
                    return false;
                }
            } else {
                stopMotor_ = false;
                return true;
            }
            currentPosition = clientStepper_->getPositionUserUnits();  // Read Position after move
            dataXRaySensor = clientSensors_->readXRaySensor(durationAcquisition_, currentPosition);  // Read X-Ray Sensor
            if (!this->checkReachingPosition(currentPosition, nextPosition)) {
                spdlog::error("Position {} not reached! The system is currently in position: {}\n", nextPosition, currentPosition);
                spdlog::debug("----------------------------------------------------------\n");
                return false;
            } else {
                spdlog::debug("Position {} reached!\n", currentPosition);
            }
            spdlog::debug("**************************************************************\n");
        }  // end for
    }
    spdlog::debug("#################################################################\n");
    if (showPlot_) {
        clientPostProcessing_->executeScript1(pathToPlotScanScript_.string(), filename_);
    }
    return true;
}

void ScanningStepper::setupAlignmentParameters(float stepSize,
                                               float range,
                                               int durationAcquisition,
                                               std::string filename,
                                               bool eraseCsvContent,
                                               bool showPlot) {
    this->setStepSize(stepSize);
    this->setRange(range);
    this->setDurationAcquisition(durationAcquisition);
    this->setFilename(filename);
    this->setEraseCsvContent(eraseCsvContent);
    this->setShowPlot(showPlot);
}

float ScanningStepper::getStepSize() {
    return stepSize_;
}

void ScanningStepper::setStepSize(float stepSize) {
    stepSize_ = stepSize;
}

int ScanningStepper::getDurationAcquisition() {
    return durationAcquisition_;
}

void ScanningStepper::setDurationAcquisition(int durationAcquisition) {
    durationAcquisition_ = durationAcquisition;
}

std::string ScanningStepper::getFilename() {
    return filename_;
}

void ScanningStepper::setFilename(std::string filename) {
    filename = this->checkExtension(filename);
    filename = this->replaceSpacesWithUnderscores(filename);
    filename_ = filename;
}

bool ScanningStepper::getEraseCsvContent() {
    return eraseCsvContent_;
}

void ScanningStepper::setEraseCsvContent(bool eraseCsvContent) {
    eraseCsvContent_ = eraseCsvContent;
}

float ScanningStepper::getRange() {
    return range_;
}

void ScanningStepper::setRange(float range) {
    range_ = range;
}

bool ScanningStepper::getShowPlot() {
    return showPlot_;
}

void ScanningStepper::setShowPlot(bool showPlot) {
    showPlot_ = showPlot;
}

std::string ScanningStepper::replaceSpacesWithUnderscores(const std::string& filename) {
    std::string result = filename;
    for (char& ch : result) {
        if (ch == ' ') {
            ch = '_'; // Replace space with underscore
        }
    }
    return result;
}

std::string ScanningStepper::checkExtension(std::string filename) {
    std::string extension = filename.substr(filename.length() - 4);
    if (extension != ".csv") {
        return filename + ".csv";
    } else {
        return filename;
    }
}

}  // namespace scanning
