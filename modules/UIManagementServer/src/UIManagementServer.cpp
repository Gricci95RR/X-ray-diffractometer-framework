/**
 * @file UIManagemenetServer.cpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2023
 * @brief This file implements the management server class that allows the communication with the GUI.
 * @version 0.1
 * @date 2023
 * 
 * @copyright © Copyright CERN 2018. All rights reserved. This software is released under a CERN proprietary software license.
 * Any permission to use it shall be granted in writing. Requests shall be addressed to CERN through mail-KT@cern.ch
 * 
 */

#include "UIManagementServer.hpp"

UIManagementServer::UIManagementServer(int port,
                                       std::shared_ptr<IDevicesFactory> devicesFactory):
                                       devicesFactory_(devicesFactory) {
    spdlog::info("cTor UIManagementServer\n");

    /*Creation of the objects*/
    server_ = std::make_shared<TCPServer>(IPV::V4, port);
    client_XRaySource = devicesFactory_->createXRaySourceDeviceController();
    client_Crystal = devicesFactory_->createCrystalDeviceController();
    client_Monochromator = devicesFactory_->createMonochromatorDeviceController();
    client_Slit = devicesFactory_->createSlitDeviceController();
    client_XraySensor = devicesFactory_->createXRaySensorDeviceController();

    /*Json to send initialization*/
    jsonToSend_ =  nlohmann::json::parse(R"(
        {
            "FSM Devices Status": {
                "Autocollimator": {
                    "state": "Not Initialized",
                    "position stepper rotational": 0
                },
                "Crystal": {
                    "state": "Not Initialized",
                    "position stepper rotational": 0,
                    "position axes hxp": [0, 0, 0, 0, 0, 0]
                },
                "Monochromator": {
                    "state": "Not Initialized",
                    "position stepper rotational": 0,
                    "position stepper linear": 0
                },
                "Slit": {
                    "state": "Not Initialized",
                    "position stepper rotational": 0,
                    "position stepper linear": 0
                },
                "X-Ray Sensor": {
                    "state": "Not Initialized",
                    "position stepper rotational": 0
                },
                "X-Ray Source": {
                    "state": "Not Initialized",
                    "position stepper rotational": 0
                }
            }
        }
    )");

    /*Definitions of Server Class obj functions*/
    server_->OnJoin = [this](TCPSocket::pointer socket) {
        spdlog::info("User has joined the server: {}\n", socket->GetUsername());
        socket_ = socket;  // socket passed to std::shared_ptr<ISocket> socket_
    };

    server_->OnLeave = [this](TCPSocket::pointer socket) {
        spdlog::info("User has left the server: {}\n", socket->GetUsername());
    };

    server_->OnClientMessage = [this](const std::string& message) { // Grabber
        message_ = message;
        spdlog::debug("User message: {}", message_);
        nlohmann::json j = nlohmann::json::parse(message_);  // deserialize message
        if (j.contains("Motor Controls") &&  // do not use threads for crystal device connection and disconnection
            (j["Motor Controls"]["Crystal Rotational"]["disconnect"].get<bool>() == true ||
             j["Motor Controls"]["Crystal Rotational"]["connect"].get<bool>() == true)) {
                interpreter();
                /* Detach a thread that runs until client_Crystal is connected */
                
                t_status_ = std::thread([this](){
                while (client_Crystal->getFsmState() != "Not Initialized") {
                    // Send FSM Status every second
                    this->sendFSMStatus();
                    std::this_thread::sleep_for(std::chrono::milliseconds(500));
                }
                return 0;
                });
                t_status_.detach();
        } else {  // detach a thread
            t1_ = std::thread([this](){
                interpreter();
                return 0;
                });
            t1_.detach();
        }
    };  // end Grabber
}

UIManagementServer::~UIManagementServer() {
    spdlog::info("dTor UIManagementServer\n");
}

bool UIManagementServer::open() {
    spdlog::info("Method open of class UIManagementServer\n");
    if (!server_->open()){
        return 0;
    }
    return 1;
}

bool UIManagementServer::close() {
    spdlog::info("Method close of class UIManagementServer\n");
    if (!server_->close()){
        return 0;
    }
    return 1;
}

bool UIManagementServer::initServer() {
    spdlog::info("Method initServer of class UIManagementServer\n");
    if (!server_->open()){
        return 0;
    }
    return 1;
}

void UIManagementServer::interpreter() {
    spdlog::info("Method interpreter of class UIManagementServer\n");
    nlohmann::json jsonFile = nlohmann::json::parse(message_);
    this->handleMotorControls(jsonFile);
    this->handleDeviceScans(jsonFile);
    this->handleBeamAlignments(jsonFile);
    this->handleCrystalAlignments(jsonFile);
    this->handleCrystalMeasurements(jsonFile);
    /* Send FSM Status */
    this->sendFSMStatus();
}

/** Handle Motor Controls */
// Function to handle X-Ray Source Rotational motor controls
void UIManagementServer::handleXRaySourceRotational(const nlohmann::json& jsonFile) {
    if (jsonFile["Motor Controls"].contains("X-Ray Source Rotational")) {  // check Branch
        if (jsonFile["Motor Controls"]["X-Ray Source Rotational"]["connect"].get<bool>() == true) {
            client_XRaySource->start();
        } else if (jsonFile["Motor Controls"]["X-Ray Source Rotational"]["disconnect"].get<bool>() == true) {
            client_XRaySource->disconnectStp();
        } else if (jsonFile["Motor Controls"]["X-Ray Source Rotational"]["go home"].get<bool>() == true) {
            client_XRaySource->goHome();
        } else if (jsonFile["Motor Controls"]["X-Ray Source Rotational"]["relative motion"].get<bool>() == true) {
            double position = jsonFile["Motor Controls"]["X-Ray Source Rotational"]["stepper position (deg)"].get<double>();
            client_XRaySource->moveToPositionStepper(position);
        }
    }
}

// Function to handle Monochromator Rotational motor controls
void UIManagementServer::handleMonochromatorRotational(const nlohmann::json& jsonFile) {
    if (jsonFile["Motor Controls"].contains("Monochromator Rotational")) {  // check Branch
        if (jsonFile["Motor Controls"]["Monochromator Rotational"]["connect"].get<bool>() == true) {
            client_Monochromator->start();
        } else if (jsonFile["Motor Controls"]["Monochromator Rotational"]["disconnect"].get<bool>() == true) {
            client_Monochromator->disconnectStp();
        } else if (jsonFile["Motor Controls"]["Monochromator Rotational"]["go home"].get<bool>() == true) {
            client_Monochromator->goHome();
        } else if (jsonFile["Motor Controls"]["Monochromator Rotational"]["relative motion"].get<bool>() == true) {
            double position = jsonFile["Motor Controls"]["Monochromator Rotational"]["stepper position (deg)"].get<double>();
            client_Monochromator->moveToPositionStepper2Rotational(position);
        }
    }
}

// Function to handle Monochromator Linear motor controls
void UIManagementServer::handleMonochromatorLinear(const nlohmann::json& jsonFile) {
    if (jsonFile["Motor Controls"].contains("Monochromator Linear")) {  // check Branch
        if (jsonFile["Motor Controls"]["Monochromator Linear"]["connect"].get<bool>() == true) {
            client_Monochromator->start();
        } else if (jsonFile["Motor Controls"]["Monochromator Linear"]["disconnect"].get<bool>() == true) {
            client_Monochromator->disconnectStp();
        } else if (jsonFile["Motor Controls"]["Monochromator Linear"]["go home"].get<bool>() == true) {
            client_Monochromator->goHome();
        } else if (jsonFile["Motor Controls"]["Monochromator Linear"]["relative motion"].get<bool>() == true) {
            double position = jsonFile["Motor Controls"]["Monochromator Linear"]["stepper position (deg)"].get<double>();
            client_Monochromator->moveToPositionStepper1Linear(position);
        }
    }
}

// Function to handle Slit Rotational motor controls
void UIManagementServer::handleSlitRotational(const nlohmann::json& jsonFile) {
    if (jsonFile["Motor Controls"].contains("Slit Rotational")) {  // check Branch
        if (jsonFile["Motor Controls"]["Slit Rotational"]["connect"].get<bool>() == true) {
            client_Slit->start();
        } else if (jsonFile["Motor Controls"]["Slit Rotational"]["disconnect"].get<bool>() == true) {
            client_Slit->disconnectStp();
        } else if (jsonFile["Motor Controls"]["Slit Rotational"]["go home"].get<bool>() == true) {
            client_Slit->goHome();
        } else if (jsonFile["Motor Controls"]["Slit Rotational"]["relative motion"].get<bool>() == true) {
            double position = jsonFile["Motor Controls"]["Slit Rotational"]["stepper position (deg)"].get<double>();
            client_Slit->moveToPositionStepper2Rotational(position);
        }
    }
}

// Function to handle Slit Linear motor controls
void UIManagementServer::handleSlitLinear(const nlohmann::json& jsonFile) {
    if (jsonFile["Motor Controls"].contains("Slit Linear")) {  // check Branch
        if (jsonFile["Motor Controls"]["Slit Linear"]["connect"].get<bool>() == true) {
            client_Slit->start();
        } else if (jsonFile["Motor Controls"]["Slit Linear"]["disconnect"].get<bool>() == true) {
            client_Slit->disconnectStp();
        } else if (jsonFile["Motor Controls"]["Slit Linear"]["go home"].get<bool>() == true) {
            client_Slit->goHome();
        } else if (jsonFile["Motor Controls"]["Slit Linear"]["relative motion"].get<bool>() == true) {
            double position = jsonFile["Motor Controls"]["Slit Linear"]["stepper position (deg)"].get<double>();
            client_Slit->moveToPositionStepper1Linear(position);
        }
    }
}

// Function to handle Crystal Rotational motor controls
void UIManagementServer::handleCrystalRotational(const nlohmann::json& jsonFile) {
    if (jsonFile["Motor Controls"].contains("Crystal Rotational")) {  // check Branch
        if (jsonFile["Motor Controls"]["Crystal Rotational"]["connect"].get<bool>() == true) {
            client_Crystal->start();
        } else if (jsonFile["Motor Controls"]["Crystal Rotational"]["disconnect"].get<bool>() == true) {
            client_Crystal->disconnect();
        } else if (jsonFile["Motor Controls"]["Crystal Rotational"]["go home"].get<bool>() == true) {
            client_Crystal->goHome();
        } else if (jsonFile["Motor Controls"]["Crystal Rotational"]["relative motion"].get<bool>() == true) {
            double position = jsonFile["Motor Controls"]["Crystal Rotational"]["stepper position (deg)"].get<double>();
            client_Crystal->moveToPositionStepper(position);
        }
    }
}

// Function to handle Hexapod motor controls
void UIManagementServer::handleHexapod(const nlohmann::json& jsonFile) {
    if (jsonFile["Motor Controls"].contains("Hexapod")) {  // check Branch
        if (jsonFile["Motor Controls"]["Hexapod"]["connect"].get<bool>() == true) {
            client_Crystal->start();
        } else if (jsonFile["Motor Controls"]["Hexapod"]["disconnect"].get<bool>() == true) {
            client_Crystal->disconnect();
        } else if (jsonFile["Motor Controls"]["Hexapod"]["go home"].get<bool>() == true) {
            client_Crystal->goHome();
        } else if (jsonFile["Motor Controls"]["Hexapod"]["absolute motion"].get<bool>() == true) {
            double posX = jsonFile["Motor Controls"]["Hexapod"]["axes positions"][0].get<double>();
            double posY = jsonFile["Motor Controls"]["Hexapod"]["axes positions"][1].get<double>();
            double posZ = jsonFile["Motor Controls"]["Hexapod"]["axes positions"][2].get<double>();
            double posU = jsonFile["Motor Controls"]["Hexapod"]["axes positions"][3].get<double>();
            double posV = jsonFile["Motor Controls"]["Hexapod"]["axes positions"][4].get<double>();
            double posW = jsonFile["Motor Controls"]["Hexapod"]["axes positions"][5].get<double>();
            client_Crystal->moveToAbsPosition(posX, posY, posZ, posU, posV, posW);
        }
    }
}

// Function to handle Autocollimator Rotational motor controls
void UIManagementServer::handleAutocollimatorRotational(const nlohmann::json& jsonFile) {
    if (jsonFile["Motor Controls"].contains("Autocollimator Rotational")) {  // check Branch
        if (jsonFile["Motor Controls"]["Autocollimator Rotational"]["connect"].get<bool>() == true) {
            // Implement handling for "connect" case
        } else if (jsonFile["Motor Controls"]["Autocollimator Rotational"]["disconnect"].get<bool>() == true) {
            // Implement handling for "disconnect" case
        } else if (jsonFile["Motor Controls"]["Autocollimator Rotational"]["go home"].get<bool>() == true) {
            // Implement handling for "go home" case
        } else if (jsonFile["Motor Controls"]["Autocollimator Rotational"]["relative motion"].get<bool>() == true) {
            double result = jsonFile["Motor Controls"]["Autocollimator Rotational"]["stepper position (deg)"].get<double>();
            // Implement handling for "relative motion" case
        }
    }
}

// Function to handle X-Ray Detector Rotational motor controls
void UIManagementServer::handleXRayDetectorRotational(const nlohmann::json& jsonFile) {
    if (jsonFile["Motor Controls"].contains("X-Ray Detector Rotational")) {  // check Branch
        if (jsonFile["Motor Controls"]["X-Ray Detector Rotational"]["connect"].get<bool>() == true) {
            client_XraySensor->start();
        } else if (jsonFile["Motor Controls"]["X-Ray Detector Rotational"]["disconnect"].get<bool>() == true) {
            client_XraySensor->disconnectStp();
        } else if (jsonFile["Motor Controls"]["X-Ray Detector Rotational"]["go home"].get<bool>() == true) {
            client_XraySensor->goHome();
        } else if (jsonFile["Motor Controls"]["X-Ray Detector Rotational"]["relative motion"].get<bool>() == true) {
            double position = jsonFile["Motor Controls"]["X-Ray Detector Rotational"]["stepper position (deg)"].get<double>();
            client_XraySensor->moveToPositionStepper(position);
        }
    }
}

// Main function to handle all motor controls
void UIManagementServer::handleMotorControls(const nlohmann::json& jsonFile) {
    if (jsonFile.contains("Motor Controls")) {
        handleXRaySourceRotational(jsonFile);
        handleMonochromatorRotational(jsonFile);
        handleMonochromatorLinear(jsonFile);
        handleSlitRotational(jsonFile);
        handleSlitLinear(jsonFile);
        handleCrystalRotational(jsonFile);
        handleHexapod(jsonFile);
        handleAutocollimatorRotational(jsonFile);
        handleXRayDetectorRotational(jsonFile);
    }
}

/** Handle Device Scans */
// Function to handle X-Ray Source Rotational scans
void UIManagementServer::handleXRaySourceRotationalScan(const nlohmann::json& jsonFile) {
    if (jsonFile["Device Scans"].contains("X-Ray Source Rotational")) {  // check Branch
        if (jsonFile["Device Scans"]["X-Ray Source Rotational"]["run scan"].get<bool>() == true) {
            double stepSize = jsonFile["Device Scans"]["X-Ray Source Rotational"]["step size"].get<double>();
            double range = jsonFile["Device Scans"]["X-Ray Source Rotational"]["range"].get<double>();
            bool eraseCsvContent = jsonFile["Device Scans"]["X-Ray Source Rotational"]["erase csv content"].get<bool>();
            int durationAcquisition = jsonFile["Device Scans"]["X-Ray Source Rotational"]["duration acquisition"].get<int>();
            std::string dataLogFileName = jsonFile["Device Scans"]["X-Ray Source Rotational"]["data log filename"].get<std::string>();
            client_XRaySource->scanStepper(stepSize, range, durationAcquisition, dataLogFileName, eraseCsvContent, true);
        } else if (jsonFile["Device Scans"]["X-Ray Source Rotational"]["stop"].get<bool>() == true) {
            client_XRaySource->stop();
        }
    }
}

// Function to handle Monochromator Rotational scans
void UIManagementServer::handleMonochromatorRotationalScan(const nlohmann::json& jsonFile) {
    if (jsonFile["Device Scans"].contains("Monochromator Rotational")) {  // check Branch
        if (jsonFile["Device Scans"]["Monochromator Rotational"]["run scan"].get<bool>() == true) {
            double stepSize = jsonFile["Device Scans"]["Monochromator Rotational"]["step size"].get<double>();
            double range = jsonFile["Device Scans"]["Monochromator Rotational"]["range"].get<double>();
            bool eraseCsvContent = jsonFile["Device Scans"]["Monochromator Rotational"]["erase csv content"].get<bool>();
            int durationAcquisition = jsonFile["Device Scans"]["Monochromator Rotational"]["duration acquisition"].get<int>();
            std::string dataLogFileName = jsonFile["Device Scans"]["Monochromator Rotational"]["data log filename"].get<std::string>();
            client_Monochromator->scanStepper2Rotational(stepSize, range, durationAcquisition, dataLogFileName, eraseCsvContent, true);
        } else if (jsonFile["Device Scans"]["Monochromator Rotational"]["stop"].get<bool>() == true) {
            client_Monochromator->stop();
        }
    }
}

// Function to handle Monochromator Linear scans
void UIManagementServer::handleMonochromatorLinearScan(const nlohmann::json& jsonFile) {
    if (jsonFile["Device Scans"].contains("Monochromator Linear")) {  // check Branch
        if (jsonFile["Device Scans"]["Monochromator Linear"]["run scan"].get<bool>() == true) {
            double stepSize = jsonFile["Device Scans"]["Monochromator Linear"]["step size"].get<double>();
            double range = jsonFile["Device Scans"]["Monochromator Linear"]["range"].get<double>();
            bool eraseCsvContent = jsonFile["Device Scans"]["Monochromator Linear"]["erase csv content"].get<bool>();
            int durationAcquisition = jsonFile["Device Scans"]["Monochromator Linear"]["duration acquisition"].get<int>();
            std::string dataLogFileName = jsonFile["Device Scans"]["Monochromator Linear"]["data log filename"].get<std::string>();
            client_Monochromator->scanStepper1Linear(stepSize, range, durationAcquisition, dataLogFileName, eraseCsvContent, true);
        } else if (jsonFile["Device Scans"]["Monochromator Linear"]["stop"].get<bool>() == true) {
            client_Monochromator->stop();
        }
    }
}

// Function to handle Slit Rotational scans
void UIManagementServer::handleSlitRotationalScan(const nlohmann::json& jsonFile) {
    if (jsonFile["Device Scans"].contains("Slit Rotational")) {  // check Branch
        if (jsonFile["Device Scans"]["Slit Rotational"]["run scan"].get<bool>() == true) {
            double stepSize = jsonFile["Device Scans"]["Slit Rotational"]["step size"].get<double>();
            double range = jsonFile["Device Scans"]["Slit Rotational"]["range"].get<double>();
            bool eraseCsvContent = jsonFile["Device Scans"]["Slit Rotational"]["erase csv content"].get<bool>();
            int durationAcquisition = jsonFile["Device Scans"]["Slit Rotational"]["duration acquisition"].get<int>();
            std::string dataLogFileName = jsonFile["Device Scans"]["Slit Rotational"]["data log filename"].get<std::string>();
            client_Slit->scanStepper2Rotational(stepSize, range, durationAcquisition, dataLogFileName, eraseCsvContent, true);
        } else if (jsonFile["Device Scans"]["Slit Rotational"]["stop"].get<bool>() == true) {
            client_Slit->stop();
        }
    }
}

// Function to handle Slit Linear scans
void UIManagementServer::handleSlitLinearScan(const nlohmann::json& jsonFile) {
    if (jsonFile["Device Scans"].contains("Slit Linear")) {  // check Branch
        if (jsonFile["Device Scans"]["Slit Linear"]["run scan"].get<bool>() == true) {
            double stepSize = jsonFile["Device Scans"]["Slit Linear"]["step size"].get<double>();
            double range = jsonFile["Device Scans"]["Slit Linear"]["range"].get<double>();
            bool eraseCsvContent = jsonFile["Device Scans"]["Slit Linear"]["erase csv content"].get<bool>();
            int durationAcquisition = jsonFile["Device Scans"]["Slit Linear"]["duration acquisition"].get<int>();
            std::string dataLogFileName = jsonFile["Device Scans"]["Slit Linear"]["data log filename"].get<std::string>();
            client_Slit->scanStepper1Linear(stepSize, range, durationAcquisition, dataLogFileName, eraseCsvContent, true);
        } else if (jsonFile["Device Scans"]["Slit Linear"]["stop"].get<bool>() == true) {
            client_Slit->stop();
        }
    }
}

// Function to handle Crystal Rotational scans
void UIManagementServer::handleCrystalRotationalScan(const nlohmann::json& jsonFile) {
    if (jsonFile["Device Scans"].contains("Crystal Rotational")) {  // check Branch
        if (jsonFile["Device Scans"]["Crystal Rotational"]["run scan"].get<bool>() == true) {
            double stepSize = jsonFile["Device Scans"]["Crystal Rotational"]["step size"].get<double>();
            double range = jsonFile["Device Scans"]["Crystal Rotational"]["range"].get<double>();
            bool eraseCsvContent = jsonFile["Device Scans"]["Crystal Rotational"]["erase csv content"].get<bool>();
            int durationAcquisition = jsonFile["Device Scans"]["Crystal Rotational"]["duration acquisition"].get<int>();
            std::string dataLogFileName = jsonFile["Device Scans"]["Crystal Rotational"]["data log filename"].get<std::string>();
            client_Crystal->scanStepper(stepSize, range, durationAcquisition, dataLogFileName, eraseCsvContent, true);
        } else if (jsonFile["Device Scans"]["Crystal Rotational"]["stop"].get<bool>() == true) {
            client_Crystal->stop();
        }
    }
}

// Function to handle Hexapod scans
void UIManagementServer::handleHexapodScan(const nlohmann::json& jsonFile) {
    if (jsonFile["Device Scans"].contains("Hexapod")) {  // check Branch
        if (jsonFile["Device Scans"]["Hexapod"]["run scan"].get<bool>() == true) {
            double stepSize = jsonFile["Device Scans"]["Hexapod"]["step size"].get<double>();
            double range = jsonFile["Device Scans"]["Hexapod"]["range"].get<double>();
            bool eraseCsvContent = jsonFile["Device Scans"]["Hexapod"]["erase csv content"].get<bool>();
            int durationAcquisition = jsonFile["Device Scans"]["Hexapod"]["duration acquisition"].get<int>();
            int axisToScan = jsonFile["Device Scans"]["Hexapod"]["axis to scan"].get<int>();
            std::string dataLogFileName = jsonFile["Device Scans"]["Hexapod"]["data log filename"].get<std::string>();
            client_Crystal->scanHxp(axisToScan, stepSize, range, durationAcquisition, dataLogFileName, eraseCsvContent, true);
        } else if (jsonFile["Device Scans"]["Hexapod"]["stop"].get<bool>() == true) {
            client_Crystal->stop();
        }
    }
}

// Function to handle Autocollimator Rotational scans
void UIManagementServer::handleAutocollimatorRotationalScan(const nlohmann::json& jsonFile) {
    if (jsonFile["Device Scans"].contains("Autocollimator Rotational")) {  // check Branch
        if (jsonFile["Device Scans"]["Autocollimator Rotational"]["run scan"].get<bool>() == true) {
            double stepSize = jsonFile["Device Scans"]["Autocollimator Rotational"]["step size"].get<double>();
            double range = jsonFile["Device Scans"]["Autocollimator Rotational"]["range"].get<double>();
            bool eraseCsvContent = jsonFile["Device Scans"]["Autocollimator Rotational"]["erase csv content"].get<bool>();
            int durationAcquisition = jsonFile["Device Scans"]["Autocollimator Rotational"]["duration acquisition"].get<int>();
            std::string dataLogFileName = jsonFile["Device Scans"]["Autocollimator Rotational"]["data log filename"].get<std::string>();
            // Implement handling for Autocollimator Rotational scans
        } else if (jsonFile["Device Scans"]["Autocollimator Rotational"]["stop"].get<bool>() == true) {
            // Implement handling for stopping Autocollimator Rotational scans
        }
    }
}

// Function to handle X-Ray Detector Rotational scans
void UIManagementServer::handleXRayDetectorRotationalScan(const nlohmann::json& jsonFile) {
    if (jsonFile["Device Scans"].contains("X-Ray Detector Rotational")) {  // check Branch
        if (jsonFile["Device Scans"]["X-Ray Detector Rotational"]["run scan"].get<bool>() == true) {
            double stepSize = jsonFile["Device Scans"]["X-Ray Detector Rotational"]["step size"].get<double>();
            double range = jsonFile["Device Scans"]["X-Ray Detector Rotational"]["range"].get<double>();
            bool eraseCsvContent = jsonFile["Device Scans"]["X-Ray Detector Rotational"]["erase csv content"].get<bool>();
            int durationAcquisition = jsonFile["Device Scans"]["X-Ray Detector Rotational"]["duration acquisition"].get<int>();
            std::string dataLogFileName = jsonFile["Device Scans"]["X-Ray Detector Rotational"]["data log filename"].get<std::string>();
            client_XraySensor->scanStepper(stepSize, range, durationAcquisition, dataLogFileName, eraseCsvContent, true);
        } else if (jsonFile["Device Scans"]["X-Ray Detector Rotational"]["stop"].get<bool>() == true) {
            client_XraySensor->stop();
        }
    }
}

// Main function to handle all device scans
void UIManagementServer::handleDeviceScans(const nlohmann::json& jsonFile) {
    if (jsonFile.contains("Device Scans")) {
        handleXRaySourceRotationalScan(jsonFile);
        handleMonochromatorRotationalScan(jsonFile);
        handleMonochromatorLinearScan(jsonFile);
        handleSlitRotationalScan(jsonFile);
        handleSlitLinearScan(jsonFile);
        handleCrystalRotationalScan(jsonFile);
        handleHexapodScan(jsonFile);
        handleAutocollimatorRotationalScan(jsonFile);
        handleXRayDetectorRotationalScan(jsonFile);
    }
}


/** Handle Beam Alignments */
void UIManagementServer::handleXRaySourceRotationalAlignment(const nlohmann::json& jsonFile) {
    if (jsonFile["Beam Alignments"].contains("X-Ray Source Rotational")) {  // check Branch
        if (jsonFile["Beam Alignments"]["X-Ray Source Rotational"]["start alignment"].get<bool>() == true) {
            bool computeAlignPosition = jsonFile["Beam Alignments"]["X-Ray Source Rotational"]["compute alignment position"].get<bool>();
            if (computeAlignPosition) {
                client_Monochromator->alignSourceWithSensor();
                client_Slit->alignSourceWithSensor();
                client_Crystal->alignSourceWithSensor();
                client_XraySensor->alignSourceWithSensor(false);
            }
            client_XRaySource->alignSourceWithSensor(computeAlignPosition);
        }
    }
}

void UIManagementServer::handleMonochromatorAlignment(const nlohmann::json& jsonFile) {
    if (jsonFile["Beam Alignments"].contains("Monochromator")) {  // check Branch
        if (jsonFile["Beam Alignments"]["Monochromator"]["start alignment"].get<bool>() == true) {
            bool computeAlignPosition = jsonFile["Beam Alignments"]["Monochromator"]["compute alignment position"].get<bool>();
            client_Monochromator->alignMonochromator(computeAlignPosition);
        }
        if (jsonFile["Beam Alignments"]["Monochromator"]["search Bragg peak"].get<bool>() == true) {
            client_XRaySource->setupMonochromatorBraggPeakSearch();
            client_Monochromator->searchMonochromatorBraggPeak();
        }
    }
}

void UIManagementServer::handleSlitAlignment(const nlohmann::json& jsonFile) {
    if (jsonFile["Beam Alignments"].contains("Slit")) {  // check Branch
        if (jsonFile["Beam Alignments"]["Slit"]["start alignment"].get<bool>() == true) {
            bool computeAlignPosition = jsonFile["Beam Alignments"]["Slit"]["compute alignment position"].get<bool>();
            client_Slit->alignSlit(computeAlignPosition);
        }
    }
}

void UIManagementServer::handleBeamAlignments(const nlohmann::json& jsonFile) {
    if (jsonFile.contains("Beam Alignments")) {
        handleXRaySourceRotationalAlignment(jsonFile);
        handleMonochromatorAlignment(jsonFile);
        handleSlitAlignment(jsonFile);
    }
}

/** Handle Crystal Alignments */
// Function to handle X-Axis Alignment
void UIManagementServer::handleCrystalXAxisAlignment(const nlohmann::json& jsonFile) {
    if (jsonFile["Crystal Alignments"].contains("X-Axis Alignment")) {  // check Branch
        if (jsonFile["Crystal Alignments"]["X-Axis Alignment"]["start alignment"].get<bool>()) {
            client_Crystal->xAxisAlignmentCrystal();
        }
    }
}

// Function to handle Y-Axis  Alignment
void UIManagementServer::handleCrystalYAxisAlignment(const nlohmann::json& jsonFile) {
    if (jsonFile["Crystal Alignments"].contains("Y-Axis Alignment")) {  // check Branch
        if (jsonFile["Crystal Alignments"]["Y-Axis Alignment"]["start alignment"].get<bool>()) {
            client_Crystal->yAxisAlignmentCrystal();
        }
    }
}

// Function to handle Y-Axis Fine Alignment
void UIManagementServer::handleCrystalYAxisFineAlignment(const nlohmann::json& jsonFile) {
    if (jsonFile["Crystal Alignments"].contains("Y-Axis Fine Alignment")) {  // check Branch
        if (jsonFile["Crystal Alignments"]["Y-Axis Fine Alignment"]["start alignment"].get<bool>()) {
            client_Crystal->yAxisFineAlignmentCrystal();
        }
    }
}

// Function to handle YW-Axes Alignment
void UIManagementServer::handleCrystalYWAxesAlignment(const nlohmann::json& jsonFile) {
    if (jsonFile["Crystal Alignments"].contains("YW-Axes Alignment")) {  // check Branch
        if (jsonFile["Crystal Alignments"]["YW-Axes Alignment"]["start alignment"].get<bool>()) {
            client_Crystal->yWAxesAlignmentCrystal();
        }
    }
}

// Function to handle Z-Axis Alignment
void UIManagementServer::handleCrystalZAxisAlignment(const nlohmann::json& jsonFile) {
    if (jsonFile["Crystal Alignments"].contains("Z-Axis Alignment")) {  // check Branch
        if (jsonFile["Crystal Alignments"]["Z-Axis Alignment"]["start alignment"].get<bool>()) {
            //client_Crystal->zAxisAlignmentCrystal();
        }
    }
}

// Function to handle Bragg Peak Search
void UIManagementServer::handleCrystalBraggPeakSearch(const nlohmann::json& jsonFile) {
    if (jsonFile["Crystal Alignments"].contains("Bragg Peak Search")) {  // check Branch
        if (jsonFile["Crystal Alignments"]["Bragg Peak Search"]["start search"].get<bool>()) {
            client_XraySensor->setupForCrystalBraggPeakSearch();
            client_Crystal->braggPeakSearchCrystal();
        }
    }
}

// Function to handle Check Alignment in Flipped Orientation
void UIManagementServer::handleCrystalCheckAlignmentFlippedOrientation(const nlohmann::json& jsonFile) {
    if (jsonFile["Crystal Alignments"].contains("Check Alignment in Flipped Orientation")) {  // check Branch
        if (jsonFile["Crystal Alignments"]["Check Alignment in Flipped Orientation"]["start alignment"].get<bool>()) {
            client_Crystal->checkAlignmentInFlippedOrientation();
        }
    }
}

// Function to handle X-Axis Fine Alignment (separate from X-Axis Alignment)
void UIManagementServer::handleCrystalXAxisFineAlignment(const nlohmann::json& jsonFile) {
    if (jsonFile["Crystal Alignments"].contains("X-Axis Fine Alignment")) {  // check Branch
        if (jsonFile["Crystal Alignments"]["X-Axis Fine Alignment"]["start alignment"].get<bool>()) {
            client_Crystal->xAxisFineAlignmentCrystal();
        }
    }
}

// Main function to handle all crystal alignments
void UIManagementServer::handleCrystalAlignments(const nlohmann::json& jsonFile) {
    if (jsonFile.contains("Crystal Alignments")) {
        handleCrystalXAxisAlignment(jsonFile);
        handleCrystalYAxisAlignment(jsonFile);
        handleCrystalYAxisFineAlignment(jsonFile);
        handleCrystalYWAxesAlignment(jsonFile);
        handleCrystalZAxisAlignment(jsonFile);
        handleCrystalBraggPeakSearch(jsonFile);
        handleCrystalCheckAlignmentFlippedOrientation(jsonFile);
        handleCrystalXAxisFineAlignment(jsonFile);
    }
}

/** Handle Crystal Measurements */
// Function to handle Bending Angle measurement
void UIManagementServer::handleCrystalBendingAngleMeasurement(const nlohmann::json& jsonFile) {
    if (jsonFile["Crystal Measurements"].contains("Bending Angle")) {  // check Branch
        if (jsonFile["Crystal Measurements"]["Bending Angle"]["start measurement"].get<bool>()) {
            client_Crystal->bendingAngleMeasurement();
        }
    }
}

// Function to handle Miscut Angle measurement
void UIManagementServer::handleCrystalMiscutAngleMeasurement(const nlohmann::json& jsonFile) {
    if (jsonFile["Crystal Measurements"].contains("Miscut Angle")) {  // check Branch
        if (jsonFile["Crystal Measurements"]["Miscut Angle"]["start measurement"].get<bool>()) {
            client_Crystal->miscutAngleMeasurement(true);
        }
    }
}

// Function to handle Torsion Angle measurement
void UIManagementServer::handleCrystalTorsionAngleMeasurement(const nlohmann::json& jsonFile) {
    if (jsonFile["Crystal Measurements"].contains("Torsion Angle")) {  // check Branch
        if (jsonFile["Crystal Measurements"]["Torsion Angle"]["start measurement"].get<bool>()) {
            client_Crystal->torsionAngleMeasurement();
        }
    }
}

// Main function to handle all crystal measurements
void UIManagementServer::handleCrystalMeasurements(const nlohmann::json& jsonFile) {
    if (jsonFile.contains("Crystal Measurements")) {
        handleCrystalBendingAngleMeasurement(jsonFile);
        handleCrystalMiscutAngleMeasurement(jsonFile);
        handleCrystalTorsionAngleMeasurement(jsonFile);
    }
}

void UIManagementServer::sendFSMStatus() {
    //  spdlog::set_level(spdlog::level::warn);  // Set the logging level to 'warn' to pause output
    if (client_Crystal->getFsmState() != "In Motion") {  // If the HXP is moving
        this->updateJsonToSendWithFSMsStates();
        this->updateJsonToSendWithStepperPositions();
        this->updateJsonToSendWithPositionsHXPAxes();
    } else {  // If the HXP is not moving
        this->updateJsonToSendWithFSMsStates();
        this->updateJsonToSendWithStepperPositions();
        this->updateJsonToSendWithPositionsHXPAxes();
    }
    socket_->write(jsonToSend_.dump());
    //  spdlog::set_level(spdlog::level::info);  // Restart the logging by setting the logging level to 'info'
}

void UIManagementServer::updateJsonToSendWithPositionsHXPAxes() {
    jsonToSend_["FSM Devices Status"]["Crystal"]["position axes hxp"][0] = client_Crystal->getPositionX();
    jsonToSend_["FSM Devices Status"]["Crystal"]["position axes hxp"][1] = client_Crystal->getPositionY();
    jsonToSend_["FSM Devices Status"]["Crystal"]["position axes hxp"][2] = client_Crystal->getPositionZ();
    jsonToSend_["FSM Devices Status"]["Crystal"]["position axes hxp"][3] = client_Crystal->getPositionU();
    jsonToSend_["FSM Devices Status"]["Crystal"]["position axes hxp"][4] = client_Crystal->getPositionV();
    jsonToSend_["FSM Devices Status"]["Crystal"]["position axes hxp"][5] = client_Crystal->getPositionW();
}

void UIManagementServer::updateJsonToSendWithFSMsStates() {
    //  jsonToSend_["FSM Devices Status"]["Autocollimator"]["state"] = client_Autocollimator->getFsmState();
    jsonToSend_["FSM Devices Status"]["Crystal"]["state"] = client_Crystal->getFsmState();
    jsonToSend_["FSM Devices Status"]["Monochromator"]["state"] = client_Monochromator->getFsmState();
    jsonToSend_["FSM Devices Status"]["Slit"]["state"] = client_Slit->getFsmState();
    jsonToSend_["FSM Devices Status"]["X-Ray Sensor"]["state"] = client_XraySensor->getFsmState();
    jsonToSend_["FSM Devices Status"]["X-Ray Source"]["state"] = client_XRaySource->getFsmState();
}

void UIManagementServer::updateJsonToSendWithStepperPositions() {
    jsonToSend_["FSM Devices Status"]["Crystal"]["position stepper rotational"] = client_Crystal->getPositionStepper();
    jsonToSend_["FSM Devices Status"]["Monochromator"]["position stepper rotational"] = client_Monochromator->getPositionStepperRotational();
    jsonToSend_["FSM Devices Status"]["Monochromator"]["position stepper linear"] = client_Monochromator->getPositionStepperLinear();
    jsonToSend_["FSM Devices Status"]["Slit"]["position stepper rotational"] = client_Slit->getPositionStepperRotational();
    jsonToSend_["FSM Devices Status"]["Slit"]["position stepper linear"] = client_Slit->getPositionStepperLinear();
    jsonToSend_["FSM Devices Status"]["X-Ray Sensor"]["position stepper rotational"] = client_XraySensor->getPositionStepper();
    jsonToSend_["FSM Devices Status"]["X-Ray Source"]["position stepper rotational"] = client_XRaySource->getPositionStepper();
}
