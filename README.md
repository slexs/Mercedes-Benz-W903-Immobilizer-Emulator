# Mercedes-Benz-W903-Immobilizer-Emulator
This project let's a user write the necessary data to the W903 CR2 ECU EEPROM data which let's a user install a Emulator in the ECU and circumvent the Immobilizer.

# Arduino EEPROM Interface for Mercedes Benz Sprinter W903 ECU

## Overview

This Arduino sketch provides a basic framework for interfacing with a ST95P08C3 EEPROM chip, commonly used in immobilizer emulator installations for Mercedes Benz Sprinter W903 Engine Control Units (ECUs). It enables the reading of EEPROM contents and demonstrates the structure necessary for writing data, although write functionality is restricted in the default setup to prevent accidental modifications.

## References

This section provides links to additional resources that can help in understanding the hardware specifications and the emulator installation process.

### Datasheet

- [ST95P08 EEPROM Chip Datasheet]([[(https://github.com/slexs/Mercedes-Benz-W903-Immobilizer-Emulator/blob/main/ST95P08.PDF]](https://github.com/slexs/Mercedes-Benz-W903-Immobilizer-Emulator/blob/main/ST95P08.PDF): This datasheet provides comprehensive details on the specifications, pinout, and communication protocols of the ST95P08 EEPROM chip, essential for proper interfacing and utilization.

### Installation Guides

- [Emulator Installation Process for Mercedes Benz Sprinter W903]([[https://github.com/slexs/Mercedes-Benz-W903-Immobilizer-Emulator/blob/main/ST95P08.PDF](https://github.com/slexs/Mercedes-Benz-W903-Immobilizer-Emulator/blob/main/ST95P08.PDF)]: A step-by-step guide to installing the immobilizer emulator in a Mercedes Benz Sprinter W903 ECU, including necessary precautions and setup instructions.

Please ensure to review these resources for a deeper understanding of the project components and installation procedures.


## Hardware Requirements

- Arduino Uno
- ST95P08C3 EEPROM chip
- SPI (Serial Peripheral Interface) setup with the following connections:
  - `CS_PIN` (Chip Select) connected to Digital Pin 10
  - `MOSI_PIN` (Master Out Slave In) connected to Digital Pin 11
  - `MISO_PIN` (Master In Slave Out) connected to Digital Pin 12
  - `SCK_PIN` (Serial Clock) connected to Digital Pin 13
  - `W_PIN` (Write Protect) connected to Digital Pin 7
  - `HOLD_PIN` connected to Digital Pin 6

## Software Dependencies

- SPI Library (`#include <SPI.h>`): Comes pre-installed with the Arduino IDE.

## Setup Instructions

1. Connect the hardware as per the specifications above.
2. Upload this sketch to your Arduino Uno.
3. Open the Serial Monitor at a baud rate of 115200 to view the EEPROM data readout.

## Program Description

This section details the core functions of the sketch, designed to interface with the ST95P08C3 EEPROM chip. These functions provide the capabilities to write data to specified EEPROM addresses, verify written data by reading from these addresses, and create a backup of all OEM data by reading the entire EEPROM.

### Core Functions

#### `pageWriteEEPROM(unsigned int startAddress, byte* data, byte length)`

- **Purpose**: Writes an array of bytes to the EEPROM starting at a specified address.
- **Parameters**:
  - `startAddress`: The starting address in the EEPROM where the data writing begins.
  - `data`: Pointer to the array of bytes to be written.
  - `length`: The number of bytes to write.
- **Operation**: This function enables writing to the EEPROM by first sending a WRITE command, followed by the address and the data bytes. It ensures the EEPROM is write-enabled before initiating the write process and waits for the write operation to complete.

#### `readEEPROMPage(unsigned int startAddress, byte* buffer, byte length)`

- **Purpose**: Reads a sequence of bytes from the EEPROM starting at a specified address into a buffer.
- **Parameters**:
  - `startAddress`: The starting address in the EEPROM from which to begin reading.
  - `buffer`: Pointer to the buffer where the read data will be stored.
  - `length`: The number of bytes to read.
- **Operation**: Initiates a READ command to the EEPROM, sequentially reads the specified number of bytes from the starting address, and stores them in the provided buffer. Useful for verifying data written to the EEPROM.

#### `readEEPROMAll()`

- **Purpose**: Reads all the data from the EEPROM and prints it to the Serial Monitor, useful for creating a backup of the OEM data.
- **Operation**: Iterates over the entire memory space of the EEPROM, reading in page-sized chunks. Each byte read is printed to the Serial Monitor, allowing for a complete backup of the EEPROM's contents before making any modifications.

### Usage Scenario

1. **Backup OEM Data**: Start with `readEEPROMAll()` to create a backup of the existing EEPROM data.
2. **Write Data**: Use `pageWriteEEPROM()` to write new data to specific EEPROM addresses.
3. **Verify Write Operation**: Employ `readEEPROMPage()` to read back and verify the data written to the EEPROM.

These functions collectively offer a comprehensive toolset for EEPROM data management, catering to the essential requirements of modifying EEPROM contents while ensuring data integrity and safety.


## Safety and Compliance

Ensure all modifications to ECU-related components comply with local regulations and standards. This software is intended for educational and testing purposes. The user is responsible for its application and the implications of any modifications to vehicle systems.
