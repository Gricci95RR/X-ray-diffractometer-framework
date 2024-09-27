/**
 * @file Sensors.cpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2022
 * @brief Implementation of Class ISensors.
 * @version 0.1
 * @date 2022
 * 
 * @copyright © Copyright CERN 2018. All rights reserved. This software is released under a CERN proprietary software license.
 * Any permission to use it shall be granted in writing. Requests shall be addressed to CERN through mail-KT@cern.ch
 * 
 */

#include "Sensors.hpp"

namespace sensors {

Sensors::Sensors(std::filesystem::path pathToProjDirectory) :
    pathToProjDirectory_(pathToProjDirectory) {
    spdlog::info("cTor Sensors\n");
    std::filesystem::path directoryNameLogFiles = "LogFiles";
    std::filesystem::path directoryNameDeviceScans = "DeviceScans";
    std::filesystem::path pathToDirectoryLogFiles = pathToProjDirectory / directoryNameLogFiles / directoryNameDeviceScans;
    pathToDirectoryLogFiles_ = pathToDirectoryLogFiles.string();
    //  XRaySensor Initialization
    std::shared_ptr<IConfiguration> clientConfiguration_ = 
            std::make_shared<Configuration>();
    std::shared_ptr<XRaySensor> clientXRaySensor = std::make_shared<XRaySensor>(clientConfiguration_);  // Obj of class XRaySensor
    clientXRaySensor_ = clientXRaySensor;
    clientXRaySensor_->connectToSensor();
    clientXRaySensor->getSensorStatus();
}

Sensors::~Sensors() {
    spdlog::info("dTor Sensors\n");
    fs_.close();
    this->deinitializeXRaySensor();
}

void Sensors::startAcquisitionSingleStepper(std::string filename, bool flushFlag) {
    spdlog::info("Method startAcquisition of Class Sensors\n");
    pathToCsv_ = pathToDirectoryLogFiles_ + "\\" + filename;
    fs_.close();
    fs_.open(pathToCsv_, std::ofstream::app);
    if (!fs_) {
        spdlog::warn("Error opening file: {}!\n", filename);
        spdlog::debug("Path to csv: {}\n", pathToCsv_);
    }
    if (flushFlag == true) {
        this->flushCsv(pathToCsv_);
        fs_ << "X-Ray Sensor Data";  // 1st col
        this->separator();
        fs_ << "Stepper Motor Position";  // 2nd col
        this->separator();
        this->endRow();
    }
}

void Sensors::startAcquisitionCrystal(std::string filename, bool flushFlag) {
    spdlog::info("Method startAcquisitionCrystal of Class Sensors\n");
    pathToCsv_ = pathToDirectoryLogFiles_ + "\\" + filename;
    fs_.close();
    fs_.open(pathToCsv_, std::ofstream::app);
    if (!fs_) {
        spdlog::warn("Error opening file: {}!\n", filename);
        spdlog::debug("Path to csv: {}\n", pathToCsv_);
    }
    if (flushFlag == true) {
        this->flushCsv(pathToCsv_);
        fs_ << "X-Ray Sensor Data";  // 1st col
        this->separator();
        fs_ << "HXP X-Axis";
        this->separator();
        fs_ << "HXP Y-Axis";
        this->separator();
        fs_ << "HXP Z-Axis";
        this->separator();
        fs_ << "HXP U-Axis";
        this->separator();
        fs_ << "HXP V-Axis";
        this->separator();
        fs_ << "HXP W-Axis";
        this->separator();
        this->endRow();
    }
}

void Sensors::savePosition(float position) {
    this->separator();
    fs_ << position;  // write in 2nd col
    this->separator();
    this->endRow();
}

void Sensors::savePosition(float positionX, float positionY, float positionZ, float positionU, float positionV, float positionW) {
    this->separator();
    fs_ << positionX;
    this->separator();
    fs_ << positionY;
    this->separator();
    fs_ << positionZ;
    this->separator();
    fs_ << positionU;
    this->separator();
    fs_ << positionV;
    this->separator();
    fs_ << positionW;
    this->separator();
    this->endRow();
}

std::string Sensors::readXRaySensor(int durationAcquisition, float position) {
    spdlog::info("Method readXRaySensor of class Sensors\n");
    const int waitTimer = 200;
    this->motionStabilizationTimer(waitTimer);  // motion stabilization wait waitTimer ms
    int output = clientXRaySensor_->acquireKalphaRadiation(durationAcquisition);
    this->writeDataToCsv(std::to_string(output));
    this->savePosition(position);
    return std::to_string(output);
}

std::string Sensors::readXRaySensor(int durationAcquisition, float positionX, float positionY, float positionZ, float positionU, float positionV, float positionW) {
    spdlog::info("Method readXRaySensor of class Sensors\n");
    const int waitTimer = 200;
    this->motionStabilizationTimer(waitTimer);  // motion stabilization wait waitTimer ms
    int output = clientXRaySensor_->acquireKalphaRadiation(durationAcquisition);
    this->writeDataToCsv(std::to_string(output));
    this->savePosition(positionX, positionY, positionZ, positionU, positionV, positionW);
    return std::to_string(output);
}

void Sensors::deinitializeXRaySensor() {
    clientXRaySensor_->disconnectSensor();
}

void Sensors::motionStabilizationTimer(int timerLength) {
    spdlog::debug("Started motionStabilizationTimer for {} ms\n", timerLength);
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
    std::this_thread::sleep_for(std::chrono::milliseconds(timerLength));
    end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    // spdlog::debug("Elapsed time: {} [s]\n", elapsed_seconds.count());
}

void Sensors::writeDataToCsv(std::string data) {
    fs_ << data;  // write in 1st col
}

void Sensors::flushCsv(std::string pathToCsv) {
    fs_.close();
    fs_.open(pathToCsv, std::ofstream::out | std::ofstream::trunc);
}

void Sensors::endRow() {
    fs_ << std::endl;
}

void Sensors::separator() {
    fs_ << ";";
}

float Sensors::readCsvResult(std::string pathToFile) {
    spdlog::info("Method readCsv of class Sensors\n");
    std::vector<std::string> outputs;
    std::ifstream infile(pathToFile);
    std::vector<int> outputs_xray_sensor;
    std::vector<float> outputs_positions;
    float position = 0;
    // Temporary buffer
    std::string temp;
    // Get the input from the input file until EOF
    while (std::getline(infile, temp, '\n')) {
        outputs.push_back(temp);
    }
    int j = 0;
    // Read second row of .csv file that contains the position
    for (const auto& i : outputs) {
       if (j == 1) {
        position = std::stof(outputs[j]);
       }
       j++;
    }
    return  position;
}

std::vector<std::string> Sensors::split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
    tokens.push_back(token);
    }
    return tokens;
}

std::filesystem::path Sensors::getPathToProjDirectory() {
    return pathToProjDirectory_;
}

}  // namespace sensors
