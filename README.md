# Mercedes-Benz-W903-Immobilizer-Emulator
This project let's a user write the necessary data to the W903 CR2 ECU EEPROM data which let's a user install a Emulator in the ECU and circumvent the Immobilizer.

# Arduino EEPROM Interface for Mercedes Benz Sprinter W903 ECU

## Overview

This Arduino sketch provides a basic framework for interfacing with a ST95P08C3 EEPROM chip, commonly used in immobilizer emulator installations for Mercedes Benz Sprinter W903 Engine Control Units (ECUs). It enables the reading of EEPROM contents and demonstrates the structure necessary for writing data, although write functionality is restricted in the default setup to prevent accidental modifications.

## References

This section provides links to additional resources that can help in understanding the hardware specifications and the emulator installation process.

### Datasheet

- [ST95P08 EEPROM Chip Datasheet]([https://www.example.com/datasheet](https://github.com/slexs/Mercedes-Benz-W903-Immobilizer-Emulator/blob/main/ST95P08.PDF)): This datasheet provides comprehensive details on the specifications, pinout, and communication protocols of the ST95P08 EEPROM chip, essential for proper interfacing and utilization.

### Installation Guides

- [Emulator Installation Process for Mercedes Benz Sprinter W903]([https://www.example.com/installation-guide](https://github.com/slexs/Mercedes-Benz-W903-Immobilizer-Emulator/blob/main/CR2.pdf)): A step-by-step guide to installing the immobilizer emulator in a Mercedes Benz Sprinter W903 ECU, including necessary precautions and setup instructions.

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

### Constants

- `CS_PIN`, `MOSI_PIN`, `MISO_PIN`, `SCK_PIN`, `W_PIN`, `HOLD_PIN`: Pin assignments for interfacing with the EEPROM.

### `setup()`

Initializes the SPI communication and configures the pins for their respective roles. The SPI bus is configured for a clock speed of 2 MHz, MSB first, and SPI Mode 0, suitable for the ST95P08C3 EEPROM. Write protection is enabled by default to allow only read operations, ensuring the EEPROM's data remains unaltered.

### `loop()`

The loop function is empty, as we only want to do the write and read once, it is done in the setup() function. 

### `readEEPROM(unsigned int address)`

- **Parameters**: `address` - The EEPROM address from which to read.
- **Returns**: The byte read from the specified EEPROM address.

This function encapsulates the process of reading a single byte from the EEPROM, using the SPI `transfer` method to send the READ instruction followed by the address.

### `writeEEPROM(unsigned int address, byte data)`

- **Parameters**:
  - `address` - The EEPROM address at which to write.
  - `data` - The byte to write to the specified EEPROM address.

This function demonstrates how to write a byte to the EEPROM, although it is not utilized in the main loop to preserve data integrity. It sends the WRITE instruction followed by the address and data byte. The function includes a delay to respect the EEPROM's write cycle time; this delay may need adjustment depending on the specific EEPROM used.

## Safety and Compliance

Ensure all modifications to ECU-related components comply with local regulations and standards. This software is intended for educational and testing purposes. The user is responsible for its application and the implications of any modifications to vehicle systems.
