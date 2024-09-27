/* © Copyright CERN 2022. All rights reserved. This software is released under a CERN proprietary software license.
 * Any permission to use it shall be granted in writing. Requests shall be addressed to CERN through mail-KT@cern.ch
 *
 * Author: Gianmarco Ricci CERN BE/CEM/MRO 2022
 * #include "IXRaySensor.hpp"
 *  ==================================================================================================
 */


#include <direct.h>
#include <iostream>
#include <vector>
#include <cstring>
#include <filesystem>

#include "Configuration.hpp"
#include "XRaySensor.hpp"

int main(int argc, char* argv[]) {
    printf("\n");
    try {

        std::shared_ptr<IConfiguration> clientConfiguration_ = 
            std::make_shared<Configuration>();
        std::shared_ptr<IXRaySensor> clientXRaySensor =
            std::make_shared<XRaySensor>(clientConfiguration_);
        
        clientXRaySensor->connectToSensor();

        clientXRaySensor->getSensorStatus();
        int timeOfAcquisition;
        for (int i = 60; i <= 100; i = i + 60) {
            timeOfAcquisition = i;
            for (int j = 0; j < 1; ++j) {
                std::cout << "Time of Acquisition: " << i << std::endl;
                std::cout << "Iteration: " << j << std::endl;
                std::cout << clientXRaySensor->acquireKalphaRadiation(timeOfAcquisition) << std::endl;
            }
        }
        clientXRaySensor->disconnectSensor();
    }

    catch(asio::system_error& e) {
        std::cout << "Error: "<< e.what()<< std::endl;
        return 1;
    }
    printf("\n");
    return 0;
}
