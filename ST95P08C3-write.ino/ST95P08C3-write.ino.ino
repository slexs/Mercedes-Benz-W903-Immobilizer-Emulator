/*
This programs intention is to prepare a ST95P08C3 EEPROM for a immobilizer emulator.
Installing this emulator in the Mercedes Benz W903 Engine Control Unit will let your bypass the Immobilizer Unit / WSP. 

This program uses a Arduino UNO to R/W a ST95P08C3 EEPROM using SPI. 

It writes the follow data to these addresses: 
Address: 0x1D0, Data: 0x22
Address: 0x1D1, Data: 0x22
Address: 0x1D2, Data: 0x00
Address: 0x1D3, Data: 0xFF
Address: 0x1D4, Data: 0xFF 

After the data has been written, it reads and prints the data from these addresses so you can confirm the change. 
Optionally you can run readEEPROMAll() to read the data in all the addresses. 
*/

#include <SPI.h>

#define CS_PIN 10 // Chip select pin (S)
#define W_PIN 7 // Write Protect (W)
#define HOLD_PIN 6 // Hold
/*
#define MOSI_PIN 11 // Master out, slave in, default pin 
#define MISO_PIN 12 // Master in, slave out , default pin
#define SCK_PIN 13 // Clock, default pin
*/

bool isWritingMode = false; // Set to 'true' for writing, 'false' for reading

// Function Prototypes
void writeEnable();
void writeEEPROM(unsigned int address, byte data);
byte readEEPROM(unsigned int address);
byte readStatusRegister();
void waitForWriteCompletion();

void setup() {
  pinMode(CS_PIN, OUTPUT);
  digitalWrite(W_PIN, HIGH);
  pinMode(W_PIN, OUTPUT);
  pinMode(HOLD_PIN, OUTPUT);

  digitalWrite(HOLD_PIN, HIGH); // Disable HOLD
  digitalWrite(CS_PIN, HIGH); // Ensure EEPROM is not selected

  SPI.begin();
  Serial.begin(115200); 
  SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0));

  /* UNCOMMENT WHEN YOU WANT TO WRITE IMMO OFF EMULATOR DATA TO ADDR 0x1D0-0X1D4 

  // // Write to addresses 0x1D0 through 0x1D4
  // byte dataToWrite[] = {0x02, 0x02, 0x00, 0xFF, 0xFF}; 
  // pageWriteEEPROM(0x1D0, dataToWrite, sizeof(dataToWrite));

  // Read the data we just wrote to addresses 0x1D0 through 0x1D4
  // byte readBuffer[5];
  // readEEPROMPage(0x1D0, readBuffer, sizeof(readBuffer));

  // // Print the data we read from addresses 0x1D0 through 0x1D4 to Serial for verification
  // for (byte i = 0; i < sizeof(readBuffer); i++) {
  //   Serial.print("Address 0x");
  //   Serial.print(0x1D0 + i, HEX);
  //   Serial.print(": 0x");
  //   Serial.println(readBuffer[i], HEX);

  // }
  */

  // Read all data from all addresses 
  // Start with this to ensure connection and data validity before running 
  // Proceed with "serial_reader_save_to_file.py" and 'eeprom_processing.py' to save EEPROM DATA as .bin file
  readEEPROMAll();

}

void loop() {
}

void testWriteEnableLatchAndWrite() {

  writeEEPROM(0x1D0, 0x02);
  waitForWriteCompletion();
  delay(100);

  byte readValue1 = readEEPROM(0x1D0);
  Serial.print("Read back value from address 0x1D0: 0x");
  Serial.println(readValue1, HEX);

  writeEEPROM(0x1D1, 0x02);
  waitForWriteCompletion();
  delay(100);

  byte readValue2 = readEEPROM(0x1D1);
  Serial.print("Read back value from address 0x1D1: 0x");
  Serial.println(readValue2, HEX);

  writeEEPROM(0x1D2, 0x00);
  waitForWriteCompletion();
  delay(100); 

  byte readValue3 = readEEPROM(0x1D2);
  Serial.print("Read back value from address 0x1D2: 0x");
  Serial.println(readValue3, HEX);

  writeEEPROM(0x1D3, 0xFF);
  waitForWriteCompletion();
  delay(100);

  byte readValue4 = readEEPROM(0x1D3);
  Serial.print("Read back value from address 0x1D3: 0x");
  Serial.println(readValue4, HEX);

  writeEEPROM(0x1D4, 0xFF);
  waitForWriteCompletion();
  delay(100);

  byte readValue5 = readEEPROM(0x1D4);
  Serial.print("Read back value from address 0x1D4: 0x");
  Serial.println(readValue5, HEX);
  
}

byte readStatusRegister() {
  digitalWrite(CS_PIN, LOW);
  SPI.transfer(0x05); // RDSR command
  byte status = SPI.transfer(0x00); // Dummy transfer to read the status
  digitalWrite(CS_PIN, HIGH);
  return status;
}

void checkBlockProtection() {
    byte status = readStatusRegister();
    Serial.print("Status Register: 0x");
    Serial.println(status, HEX);
    
    // Extracting BP bits
    byte bpBits = (status & 0x0C) >> 2; // BP bits are b3 and b4, shifting right by 2 to get the value

    Serial.print("Block Protection Bits (BP1 BP0): ");
    Serial.println(bpBits, BIN);
    
    switch(bpBits) {
        case 0b00:
            Serial.println("No block protection.");
            break;
        case 0b01:
            Serial.println("Upper quarter write-protected.");
            break;
        case 0b10:
            Serial.println("Upper half write-protected.");
            break;
        case 0b11:
            Serial.println("All memory write-protected.");
            break;
        default:
            // This case should not happen as BP bits are only two bits
            Serial.println("Invalid Block Protection Bits.");
            break;
    }
}

byte readEEPROM(unsigned int address) {
  digitalWrite(CS_PIN, LOW); 
  SPI.transfer(0x03); 
  SPI.transfer((address >> 8) & 0xFF); // MSB of the address
  SPI.transfer(address & 0xFF); // LSB of the address
  byte result = SPI.transfer(0x00);
  digitalWrite(CS_PIN, HIGH);
  return result; 
}

void writeEnable() {
  digitalWrite(CS_PIN, LOW);
  SPI.transfer(0x06); // WREN command
  delay(5);
  digitalWrite(CS_PIN, HIGH);
}

void waitForWriteCompletion() {
    Serial.println("Waiting for write to complete...");
    byte status;
    do {
        status = readStatusRegister();
        if (status & 0x01) { // If WIP bit is set, write is still in progress
            Serial.println("Write in progress...");
        }
        delay(10); // Short delay before checking again
    } while (status & 0x01); // Loop until WIP bit clears

    Serial.println("Write operation completed.");
}

bool checkWELCleared() {
    byte status = readStatusRegister();
    if (status & 0x02) { // If WEL bit is set, then it's not cleared yet
        Serial.println("WEL bit is not cleared yet.");
        return false;
    } else {
        Serial.println("WEL bit cleared.");
        return true;
    }
}

bool checkWriteEnable() {
  digitalWrite(CS_PIN, LOW);
  SPI.transfer(0x05); // RDSR command to read the status register
  byte status = SPI.transfer(0x00); // Dummy transfer to read the response
  digitalWrite(CS_PIN, HIGH);

  // Print the status register value for debugging
  Serial.print("Status register: 0x");
  Serial.println(status, HEX);

  // Check if the WEL bit is set
  bool isWELSet = status & 0x02; // 0x02 corresponds to the WEL bit
  if (isWELSet) {
    Serial.println("WEL bit is set. EEPROM is ready for write operation.");
  } else {
    Serial.println("WEL bit is not set. EEPROM is not ready for write operation.");
  }
  return isWELSet;
}

void writeEEPROM(unsigned int address, byte data) {
  // Serial.println("Enable EEPROM write...");
  writeEnable(); // Ensure the EEPROM is write-enabled
  delay(100); // Delay after sending WREN, ensure EEPROM has enough time to set WEL

  // Serial.println("Checking if WEL bit is set...");
  if (!checkWriteEnable()) {
    Serial.println("Write Enable Latch not set! Exiting..");
    return; // Exit if WEL not set
  }

  // Serial.println("Setting CS pin low...");
  digitalWrite(CS_PIN, LOW);

  // Serial.println("Sending WRITE instruction code...");
  SPI.transfer(0x02); // WRITE instruction code
  
  // Serial.println("Sending MSB of address...");
  SPI.transfer((address >> 8) & 0xFF); // MSB of the address
  
  // Serial.println("Sending LSB of address...");
  SPI.transfer(address & 0xFF); // LSB of the address
  
  // Serial.print("Sending data to address: 0x");
  // Serial.print(address, HEX);
  // Serial.print(" -> Data: 0x");
  // Serial.println(data, HEX);
  SPI.transfer(data);
  
  // Serial.println("Setting CS pin HIGH...");
  digitalWrite(CS_PIN, HIGH);
  delay(100);

  // delay(10); // Wait for the write to complete
  // Serial.println("Waiting for write completion...");
  // waitForWriteCompletion();
}

void pageWriteEEPROM(unsigned int startAddress, byte* data, byte length) {
    writeEnable(); // Ensure the EEPROM is write-enabled

    digitalWrite(CS_PIN, LOW);
    SPI.transfer(0x02); // WRITE command
    SPI.transfer((startAddress >> 8) & 0xFF); // MSB of the start address
    SPI.transfer(startAddress & 0xFF); // LSB of the start address

    for (byte i = 0; i < length; i++) {
        SPI.transfer(data[i]); // Send the data bytes
    }

    digitalWrite(CS_PIN, HIGH); // Deselect to start the write process
    waitForWriteCompletion(); // Wait for the write to complete
}

void readEEPROMPage(unsigned int startAddress, byte* buffer, byte length) {
    digitalWrite(CS_PIN, LOW);
    SPI.transfer(0x03); // READ command
    SPI.transfer((startAddress >> 8) & 0xFF); // MSB of the start address
    SPI.transfer(startAddress & 0xFF); // LSB of the start address

    for (byte i = 0; i < length; i++) {
        buffer[i] = SPI.transfer(0x00); // Send dummy byte to read data
    }

    digitalWrite(CS_PIN, HIGH);
}

void readEEPROMAll() {
    const unsigned int totalMemory = 1024; // Total EEPROM size in bytes
    const byte chunkSize = 16; // Read in page-sized chunks
    byte buffer[chunkSize]; // Temporary buffer to store read data

    for (unsigned int startAddress = 0; startAddress < totalMemory; startAddress += chunkSize) {
        readEEPROMPage(startAddress, buffer, chunkSize);

        // Print the read data for each chunk
        for (byte i = 0; i < chunkSize; i++) {
            Serial.print("Address 0x");
            Serial.print(startAddress + i, HEX);
            Serial.print(": 0x");
            Serial.println(buffer[i], HEX);
            delay(20); // Introducing a small delay between each byte
        }
        delay(50); // A longer delay after each chunk
    }
}
