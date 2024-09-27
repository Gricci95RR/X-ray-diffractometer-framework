# Configuration File Management Interface

This module provides a set of methods to read and write configuration files used for various settings and parameters. The `IConfiguration` interface allows users to interact with configuration files in a standardized way, making it easier to manage and retrieve configuration data. The module is used to manipulate only configuration files (.ini). This type of files are used to read devices configuration such as: IP Address, Motor IDs, device alignment positions etc.

This project contains the configuration files in the directory ../ConfigurationFiles/.
1) 'config.ini': configuration file that stores the 'devices configurations' (i.e. Ip addresses, stepper motor IDs ...) and the 'alignment configurations' (i.e. computed alignment positions for every device and measured crystal parameters).
2) 'AlignmentSettings.ini': this configuration file stores all the informations required to perform scans in order to align the devices or to measure the crystal under test. 

## Table of Contents

- [Introduction](#configuration-file-management-interface)
- [Table of Contents](#table-of-contents)
- [Description](#description)
- [License](#license)
- [Dependencies](#dependencies)
- [Usage](#usage)
- [API Documentation](#api-documentation)
- [Contributing](#contributing)
- [Contact](#contact)

## Description

The `IConfiguration` interface abstracts the handling of configuration files, providing methods to read and write various types of data, such as floats, integers, strings, and booleans. The interface allows users to:

- Read and write configuration values to specific sections and keys in the configuration file.
- Create and remove configuration files.
- Check for the existence of configuration files, sections, and keys.
- Read configuration values and convert them to different data types.
- Retrieve paths to various directories and configuration files.

The interface also includes methods to read default IP addresses and port numbers for Hexapod and stepper motor controllers, providing a convenient way to access commonly used values.

## License

This software is released under a CERN proprietary software license. Any permission to use it shall be granted in writing. Requests shall be addressed to CERN through mail-KT@cern.ch.

## Dependencies

- C++ Standard Library
- C++17 Filesystem (std::filesystem)
- The module makes use of the external library "mINI" (<https://github.com/pulzed/mINI>).

## Usage

To use this interface, include the `IConfiguration.hpp` header file in your C++ project. Implement a class that inherits from the `IConfiguration` interface and provides concrete implementations for the virtual functions defined in the interface.

Here's a simple example of how to use the `IConfiguration` interface:

```cpp
#include "IConfiguration.hpp"
#include <iostream>

class MyConfiguration : public IConfiguration {
public:
    // Implement the virtual functions of the IConfiguration interface
    // ...

    // Example usage:
    void readConfigurationValue() {
        // Set the path to the configuration file directory
        setPath("/path/to/config/directory");

        // Read a float value from the configuration file
        std::filesystem::path configFile = getConfigFilename();
        float value = readFloatFromConfigurationFile(configFile, getPath(), "section_name", "key_name");

        // Use the value as needed
        std::cout << "Read value: " << value << std::endl;
    }
};

int main() {
    MyConfiguration config;
    config.readConfigurationValue();
    return 0;
}
```

## API Documentation

The detailed API documentation for each function can be found in the `IConfiguration.hpp` file as part of the Doxygen comments.

## Contributing

Contributions to this project are welcome. To contribute, please follow the standard GitHub workflow of fork, branch, commit, and pull request.

## Contact

For any questions or inquiries, please contact Gianmarco Ricci at gianmarco.ricci@cern.ch.