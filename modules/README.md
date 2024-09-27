# Code Structure

The code is structured following modular architecture. The modules that make up the framework are:
1. **Communication**: module used to establish TCP connection;
2. **ConfigurationFileParser**: module used to manipulate configuration files (.ini);
3. **Devices**: module that controls at high level every device involved in the measurements with the use of a finite state machine;
4. **Motors**: module that wraps the two libraries used to control respectively the stepper motors and the hexapod robot;
5. **PostProcessing**: module used to execute Python post processing scripts;
6. **Scan**: module used to control the scan of a device moved by a stepper motor or a Hexapod robot;
7. **Sensors**: module used to start data acquisition, save sensor data, and manage logging of the X-Ray sensor readings in a CSV file;
8. **UIManagementServer**: module used to comunicate with the GUI;
9. **XRaySensor**: module used to connect and trasfer information via serial communication to the X-Ray Sensor.
