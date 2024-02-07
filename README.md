---
# This project is moving to Rust.
Progress can be followed in the new Repository.
[https://github.com/BitsAndDroids/FlightConnector-Rust](https://github.com/BitsAndDroids/FlightConnector-Rust)


[In this article I'm explaining why I've started this rewrite](https://bitsanddroids.com/post/flightsim/the-connector-deserves-a-first-class-upgrade-from-c-to-rust)
---

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

# Structure

## Dashboard

The dashboard is the mainscreen of the application. It consists of several independent units(widgets). The controlling logic has been delegated to seperate controller classes. See the image below for a visual representation of the structure.
![image](https://user-images.githubusercontent.com/77780263/194305249-a81d2d6e-61d3-48c5-9c45-b503e07b11fe.png)
![image](https://user-images.githubusercontent.com/77780263/194308800-1cee25e7-a99e-46d6-a621-9a6067043580.png)

# Arduino / ESP32 library

The libraries are a way to simplify the communication between your microcontroller and the connector. They have their own repository which can be found at:
https://github.com/BitsAndDroids/BitsAndDroidsFlightSimLibrary

# Documentation

The documentation for the connector can be found on the Bits and Droids website.
https://www.bitsanddroids.com/flight-sim-connector-library-documentation/

I've started to document the code using Doxygen. For more code insights visit the [Github pages section of this repository](https://bitsanddroids.github.io/Bits-and-Droids-flight-connector/).
