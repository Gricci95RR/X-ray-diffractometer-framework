# XRaySensor Communication Module

This module provides a standardized way to communicate with an X-ray sensor device. The `IXRaySensor` interface defines methods for managing connections, acquiring radiation data, and processing spectral information from the sensor.

## Table of Contents

- [Introduction](#xraysensor-communication-interface)
- [Table of Contents](#table-of-contents)
- [Description](#description)
- [License](#license)
- [Dependencies](#dependencies)
- [API Documentation](#api-documentation)
- [Contributing](#contributing)
- [Contact](#contact)

## Description

The `IXRaySensor` interface abstracts the complexities of interacting with an X-ray sensor device, offering a simplified API for users. It provides several key methods:

1. **`connectToSensor()`**: Attempts to establish a connection with the X-ray sensor. Returns `true` if the connection is successful, otherwise returns `false`.

2. **`disconnectSensor()`**: Disconnects the X-ray sensor, ensuring any established connection is properly terminated.

3. **`getSensorStatus()`**: Sends a status request to the sensor, retrieves the response, and updates internal status flags based on the received data.

4. **`acquireKalphaRadiation(int timeOfAcquisition)`**: Initiates the acquisition of K-alpha radiation data and returns the count for the specified acquisition time.

5. **`acquireKbetaRadiation(int timeOfAcquisition)`**: Similar to the K-alpha method, this acquires K-beta radiation data and returns the count.

6. **`acquireFullSpectrumOfRadiations(int timeOfAcquisition)`**: Collects the full spectrum of radiation data and returns it as a vector of integers.

7. **`computeIntegral(const std::vector<int>& numbers, size_t start, size_t stop)`**: Computes the integral of the specified range within the spectrum, useful for analyzing specific sections of the data.

These methods allow developers to connect to the sensor, retrieve critical data, and perform essential calculations, all while abstracting the low-level details.

## License

This software is released under a CERN proprietary software license. Any permission to use it shall be granted in writing. Requests shall be addressed to CERN through mail-KT@cern.ch.

## Dependencies

- C++ Standard Library
- DppLibUsb.h
- libusb.h
- ConsoleHelper.h

## API Documentation

The detailed API documentation for each function can be found in the `IXRaySensor.hpp` file as part of the Doxygen comments.

## Contributing

Contributions to this project are welcome. To contribute, please follow the standard GitHub workflow: fork the repository, create a branch, commit your changes, and submit a pull request.

## Contact

For any questions or inquiries, please contact Gianmarco Ricci at gianmarco.ricci@cern.ch.