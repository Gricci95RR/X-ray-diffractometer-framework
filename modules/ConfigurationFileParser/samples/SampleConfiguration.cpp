/* © Copyright CERN 2021.  All rights reserved. This software is released under a CERN proprietary
 * software licence. Any permission to use it shall be granted in writing. Requests shall be
 * addressed to CERN through mail-KT@cern.ch
 *
 * Author: Gianmarco Ricci CERN BE/CEM/MRO 2022
 *         
 *  ===============================================================================================
 */
#include <direct.h>
#include <ini.h>

#include <iostream>

#include "Configuration.hpp"

int main(int, char**) {
    std::shared_ptr<IConfiguration> conf1 =
        std::make_shared<Configuration>();
    spdlog::set_level(spdlog::level::debug);
    std::cout << conf1->readStringFromConfigurationFile(conf1->getConfigFilename(), conf1->getPath(), "Hexapod", "IpAdress") << std::endl;
    return 0;
}
