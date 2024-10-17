# X-Ray Diffractometer for Crystal Characterization

This C++ framework manages a custom-built X-ray Diffractometer (XRD). The XRD system utilizes X-ray diffraction to assess silicon crystal collimators by measuring bending, miscut, and torsion angles. The code is designed with a modular architecture to optimize the control of the mechatronic device.

## Table of Contents

- [Introduction](#x-Ray-Diffractometer-for-Crystal-Characterization)
- [Measurement Setup](#measurement-setup)
- [Repository Structure](#repository-structure)

## Measurement Setup
The devices used in these measurements are:

1. Hexapod robot
2. 8 stepper motors
3. X-Ray Source
4. X-Ray Detector
5. Autocollimator
6. Monochromator
7. Slit

## Repository Structure

The repository is structured as follows:

1. **ConfigurationFiles**: Contains configuration files for setting up or customizing various aspects of the X-ray machine.
2. **cmake**: Holds CMake build scripts necessary for compiling the project.
3. **extern**: Includes external dependencies or libraries utilized by the project, such as third-party libraries or resources required for building and running the framework.
4. **modules**: Comprises modular components that extend the framework's functionality, with each module organized into its own subdirectory.
5. **scripts**: Contains scripts for automation, testing, or other tasks, which may include shell scripts, Python scripts, or other executable formats.
