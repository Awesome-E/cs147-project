# "The MicroController"

A simple Bluetooth gamepad built with an ESP32 microcontroller. Uses a gyroscope for joystick control and three input buttons

## Hardware

- ESP32 development board
- SparkFun Gyroscope
- Three buttons (red, blue, green) connected to pins 2, 15, and 13
- Four LED Lights (one for each button plus one for power)

## Setup

1. Install [PlatformIO](https://platformio.org/)
2. Clone this repository
3. Connect your hardware according to the pin definitions in `src/main.cpp`
4. Load the code onto the microcontroller

## How it works

The Gyroscope readings are mapped to joystick X/Y axes, and the three buttons send button presses over Bluetooth. The gamepad should automatically appear as a local available bluetooth device.
