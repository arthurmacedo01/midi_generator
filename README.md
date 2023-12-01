# ESP32 MIDI Generator
## Overview
The ESP32 MIDI Generator is a project developed with the ESP-IDF framework, allowing an ESP32 microcontroller to generate MIDI protocol messages based on analog input readings. The microcontroller reads one or more analog input ports, typically connected to piezoelectric sensors, and generates MIDI NOTE_ON messages with the corresponding velocity parameters. This project is ideal for creating a simple and cost-effective MIDI-triggered system using the ESP32.

## Features
Analog Input Reading: Reads analog input ports to determine the velocity parameters.
MIDI NOTE_ON Messages: Generates MIDI NOTE_ON messages based on the analog input readings.
Configurable Channels and Notes: Easily configure the MIDI channel and note for each connected sensor.
Compatibility with Piezoelectric Sensors: Install one or more piezoelectric sensors to the ADC ports for triggering MIDI messages.


## Installation and Setup
Clone the repository
Build and flash the firmware to the ESP32:

## Usage

Connect piezoelectric sensors or other analog sensors to the ADC ports of the ESP32.
Power on the ESP32.
Use software such as ttyMIDI in Linux to receive and interpret the MIDI messages.
Optionally, use ARDOUR to leverage the MIDI messages for music creation.
Enhance your setup by incorporating LSP Sampler STEREO plugging in ARDOUR to load audio sample files and play sounds triggered by the sensors.

### Software for Linux
ttyMIDI: Receive and interpret MIDI messages.
ARDOUR: Utilize MIDI messages for music production.
LSP Sampler STEREO plugging in ARDOUR: Load audio sample files and play sounds triggered by the sensors.
