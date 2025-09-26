# RC Airplane and Transmitter
This repository contains Arduino code for a DIY RC airplane and its remote control.  
The project is a PlatformIO project adapted with Makefile for two codebases for different Arduinos.

For more informations like schematics and pictures visit ...

## Structure
Read Makefile for usage.

- **lib/shared/**
  Shared protocol and "debug" function.

- **src_plane/**  
  Code for the airplane, including motor and servo control, and RF signal reception.

- **src_controller/**  
  Code for the controller, maps user's input into values that the plane understand and sends them via RF transmission.

## Components
- Arduino Nano
- nRF24L01 modules 
- Joystick modules
- Brushless motor
- ESC
- Servomotors
