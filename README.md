# Bits-and-Droids-flight-connector
Repositry for the mfs2020 flight connector by Bits and Droids

This project runs on the QT UI framework https://www.qt.io/download-open-source

# Tools needed to compile
Qt version 6.2
Qt additions:
  - Qt Charts
  - Qt Serialport
  - Qt Graphics

Can also be run from third party IDE's like CLion.

# Project setup
This project includes all the files and build steps required to build the project in your favorite IDE.
All the required DLL's are included in the dist folder which get added upon build.

- Clone / download
- Unzip
- Run CMAKE on the CMakeLists.txt in the binary folder
- Build
- Run

# Tests
Tests are added to the Test folder. CMake will generate a seperate .exe for the unit tests. You're able to run these test automatically by adding the .exe to your post build setup (IDE dependent).

> There are a boatload of inputs/outputs that are still missing Unit tests. All help on this front is welcome <3


# Arduino / ESP32 library
The libraries are a way to simplify the communication between your microcontroller and the connector. They have their own repository which can be found at:
https://github.com/BitsAndDroids/BitsAndDroidsFlightSimLibrary

# Documentation
The documentation for the connector can be found on the Bits and Droids website.
https://www.bitsanddroids.com/flight-sim-connector-library-documentation/


