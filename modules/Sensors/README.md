# XRaySensor Device Logging Interface

## Overview

This module contains the header file `ISensors.hpp`, which defines an interface to control the XRaySensor Device logging. The interface provides methods to start data acquisition, save sensor data, and manage logging of the X-Ray sensor readings in a CSV file. This interface is designed to be used with a stepper motor and a Hexapod Robot to log sensor data and motor positions.

## Table of Contents

- [Introduction](#xraysensor-device-logging-interface)
- [Table of Contents](#table-of-contents)
- [Description](#description)
- [License](#license)
- [Dependencies](#dependencies)
- [Usage](#usage)
- [API Documentation](#api-documentation)
- [Contributing](#contributing)
- [Contact](#contact)

## Description

The `ISensors` interface is used to control the XRaySensor Device logging in a convenient and structured manner. The interface abstracts away the low-level details of data acquisition, CSV file management, and positioning, allowing users to focus on logging X-Ray sensor data and motor positions without worrying about implementation intricacies.

The interface provides the following functionalities:

1. Start acquisition of sensor data and position for one stepper motor.
2. Start acquisition of sensor data, position for one stepper motor, and 6 axis positions for the Hexapod Robot.
3. Save the position of the stepper motor and/or Hexapod axis.
4. Read X-Ray sensor data and save it to a CSV file along with the motor position.
5. Read X-Ray sensor data and save it to a CSV file along with the Hexapod axis positions.
6. Deinitialize the X-Ray sensor.
7. Implement a delay for motor stabilization after movement.
8. Write data read by the X-Ray sensor to a CSV file.
9. Flush the CSV file used for X-Ray sensor data logging.
10. Read float from a CSV file.
11. Get the path to the project directory.

## License

This software is released under a CERN proprietary software license. Any permission to use it shall be granted in writing. Requests shall be addressed to CERN through mail-KT@cern.ch.

## Dependencies

- C++ Standard Library
- `std::filesystem` (C++17 or later)

## Usage

To use the `ISensors` interface, include the `ISensors.hpp` header file in your C++ project. Implement a class that inherits from the `ISensors` interface and provide concrete implementations for the virtual functions defined in the interface.

Here's a simple example of how to use the `ISensors` interface:

```cpp
#include "ISensors.hpp"

class MyXRaySensor : public sensors::ISensors {
public:
  // Implement the virtual functions of the ISensors interface
  // ...

  // Example usage:
  void performXRayScan() {
    std::string filename = "scan_results.csv";
    bool eraseCsvContent = true;
    
    // Start acquisition and log X-Ray sensor data with motor position
    startAcquisitionSingleStepper(filename, eraseCsvContent);

    // Perform X-Ray scan and save sensor data and motor position to CSV
    float motorPosition = 0.0;
    while (motorPosition < 360.0) {
      // Perform X-Ray scan and get sensor data and save them in the CSV file
      std::string sensorData = readXRaySensor(1000, motorPosition);

      // Increment motor position for the next scan
      motorPosition += 10.0;
    }
    
  }
};

int main() {
  MyXRaySensor xRaySensor;
  xRaySensor.performXRayScan();
  return 0;
}
```

## API Documentation

The detailed API documentation for each function can be found in the `ISensors.hpp` file as part of the Doxygen comments.

## Contributing

Contributions to this project are welcome. To contribute, please follow the standard GitHub workflow of fork, branch, commit, and pull request.

## Contact

For any questions or inquiries, please contact Gianmarco Ricci at gianmarco.ricci@cern.ch.