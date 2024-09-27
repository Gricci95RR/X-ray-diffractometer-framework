# XRaySensor Device Logging Module

This module provides a set of classes that implement the `IScanning` interface for controlling the scan of a device moved by a stepper motor or a Hexapod robot. The module includes two concrete implementations of the interface: `ScanningHXP` and `ScanningStepper`.

## Table of Contents

- [Introduction](#xraysensor-device-logging-module)
- [Table of Contents](#table-of-contents)
- [Description](#description)
- [License](#license)
- [Dependencies](#dependencies)
- [Usage](#usage)
- [API Documentation](#api-documentation)
- [Contributing](#contributing)
- [Contact](#contact)

## Description

The `IScanning` interface defines a set of methods that allow users to control the scan of a device moved by a stepper motor or a Hexapod robot. The interface abstracts away the low-level details of the scan process and provides a unified way to start, configure, and stop the scan. It also allows users to set various scan parameters, such as step size, range, duration of acquisition, output filename, etc.

The module includes two concrete implementations of the `IScanning` interface:

1. `ScanningHXP`: This class implements the interface for controlling the scan of a Hexapod robot. It allows users to specify the axis to scan and provides methods to start and configure the scan process.

2. `ScanningStepper`: This class implements the interface for controlling the scan of a device moved by a stepper motor. It provides methods to start and configure the scan, as well as methods to check the reaching position and stop the scan.

## License

This software is released under a CERN proprietary software license. Any permission to use it shall be granted in writing. Requests shall be addressed to CERN through mail-KT@cern.ch.

## Dependencies

- C++ Standard Library

## Usage

To use this module, include the `IScanning.hpp` header file in your C++ project. Implement classes `ScanningHXP` and `ScanningStepper` that inherit from the `IScanning` interface and provide concrete implementations for the virtual functions defined in the interface.

Here's a simple example of how to use the `IScanning` interface:

```cpp
#include "IScanning.hpp"

class MyScanningHXP : public scanning::IScanning {
public:
  // Implement the virtual functions of the IScanning interface
  // ...

  // Example usage:
  void performHexapodScan() {
    // Set up scan parameters
    setStepSize(0.1);
    setRange(1.0);
    setDurationAcquisition(100);
    setFilename("scan_results.csv");
    setEraseCsvContent(true);
    setShowPlot(true);

    // Start the Hexapod scan
    scan();
  }
};

class MyScanningStepper : public scanning::IScanning {
public:
  // Implement the virtual functions of the IScanning interface
  // ...

  // Example usage:
  void performStepperScan() {
    // Set up scan parameters
    setStepSize(0.05);
    setRange(-2.0);
    setDurationAcquisition(50);
    setFilename("stepper_scan.csv");
    setEraseCsvContent(true);
    setShowPlot(false);

    // Start the stepper motor scan
    scan();
  }
};

int main() {
  MyScanningHXP hexapodScanner;
  hexapodScanner.performHexapodScan();

  MyScanningStepper stepperScanner;
  stepperScanner.performStepperScan();

  return 0;
}
```

## API Documentation

The detailed API documentation for each function can be found in the `IScanning.hpp` file as part of the Doxygen comments.

## Contributing

Contributions to this project are welcome. To contribute, please follow the standard GitHub workflow of fork, branch, commit, and pull request.

## Contact

For any questions or inquiries, please contact Gianmarco Ricci at gianmarco.ricci@cern.ch.