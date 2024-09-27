/**
 * @file PostProcessing.cpp
 * @author Gianmarco Ricci CERN BE/CEM/MRO 2022
 * @brief Implementation of Class @ref IPostProcessing.
 * @version 0.1
 * @date 2022
 * 
 * @copyright © Copyright CERN 2018. All rights reserved. This software is released under a CERN proprietary software license.
 * Any permission to use it shall be granted in writing. Requests shall be addressed to CERN through mail-KT@cern.ch
 * 
 */

#include "PostProcessing.hpp"

PostProcessing::PostProcessing() {
    spdlog::info("cTor PostProcessing\n");
}

PostProcessing::~PostProcessing() {
    spdlog::info("dTor PostProcessing\n");
}

void PostProcessing::executeScript1(std::string pathToPythonScript, 
                                    std::string argument1) {
    spdlog::info("Method executeScript1 of Class PostProcessing\n");
    std::string commandToExecuteScript = "py " + pathToPythonScript + " "
                                               + argument1;
    spdlog::debug("Executing: {}\n", commandToExecuteScript);
    std::system(commandToExecuteScript.data());
}

void PostProcessing::executeScript2(std::string pathToPythonScript, 
                                    std::string argument1,
                                    std::string argument2) {
    spdlog::info("Method executeScript2 of Class PostProcessing\n");
    std::string commandToExecuteScript = "py " + pathToPythonScript + " "
                                                        + argument1 + " "
                                                        + argument2;
    spdlog::debug("Executing: {}\n", commandToExecuteScript);
    std::system(commandToExecuteScript.data());
}

void PostProcessing::executeScript5(std::string pathToPythonScript, 
                                    std::string argument1,
                                    std::string argument2,
                                    std::string argument3,
                                    std::string argument4,
                                    std::string argument5) {
    spdlog::info("Method executeScript5 of Class PostProcessing\n");
    std::string commandToExecuteScript = "py " + pathToPythonScript + " "
                                                        + argument1 + " "
                                                        + argument2 + " "
                                                        + argument3 + " "
                                                        + argument4 + " "
                                                        + argument5;
    spdlog::debug("Executing: {}\n", commandToExecuteScript);
    std::system(commandToExecuteScript.data());
}

void PostProcessing::executeScript6(std::string pathToPythonScript, 
                                    std::string argument1,
                                    std::string argument2,
                                    std::string argument3,
                                    std::string argument4,
                                    std::string argument5,
                                    std::string argument6) {
    spdlog::info("Method executeScript5 of Class PostProcessing\n");
    std::string commandToExecuteScript = "py " + pathToPythonScript + " "
                                                        + argument1 + " "
                                                        + argument2 + " "
                                                        + argument3 + " "
                                                        + argument4 + " "
                                                        + argument5 + " "
                                                        + argument6;
    spdlog::debug("Executing: {}\n", commandToExecuteScript);
    std::system(commandToExecuteScript.data());
}

void PostProcessing::executeScript7(std::string pathToPythonScript, 
                                    std::string argument1,
                                    std::string argument2,
                                    std::string argument3,
                                    std::string argument4,
                                    std::string argument5,
                                    std::string argument6,
                                    std::string argument7) {
    spdlog::info("Method executeScript5 of Class PostProcessing\n");
    std::string commandToExecuteScript = "py " + pathToPythonScript + " "
                                                        + argument1 + " "
                                                        + argument2 + " "
                                                        + argument3 + " "
                                                        + argument4 + " "
                                                        + argument5 + " "
                                                        + argument6 + " "
                                                        + argument7;
    spdlog::debug("Executing: {}\n", commandToExecuteScript);
    std::system(commandToExecuteScript.data());
}