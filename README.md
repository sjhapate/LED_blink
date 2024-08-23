# ESP32 LED Blinking and ADC Reading Example

## Overview

This project demonstrates how to control a RGB LED strip and read analog values from a potentiometer using the ESP32 microcontroller. The LED strip will blink in various colors based on the readings from the ADC, showcasing the integration of FreeRTOS for multitasking.

## Features

- **LED Control**: Utilize the RMT (Remote Control) protocol to control WS2812 or compatible LED strips.
- **ADC Functionality**: Read analog values from a potentiometer connected to the ADC input.
- **Multitasking**: Implement FreeRTOS tasks to handle LED blinking and ADC reading concurrently.

## Hardware Requirements

- **ESP32 Development Board**: Any ESP32 board will work.
- **LED Strip**: WS2812 or similar addressable RGB LED strip.
- **Potentiometer**: 10kΩ recommended for analog input.
- **Breadboard and Jumper Wires**: For connections.

## Software Requirements

- **ESP-IDF**: Ensure you have the latest version of the Espressif IoT Development Framework installed.
- **CMake**: Required for building the project.
- **Terminal/Command Line Interface**: For executing build and flash commands.