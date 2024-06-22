import serial
import re

def read_serial_to_file(com_port, baud_rate, output_filename):
    ser = serial.Serial(com_port, baud_rate)
    with open(output_filename, 'w') as f:
        while True:
            try:
                line = ser.readline().decode('utf-8')
                print(line, end='')
                f.write(line)
                f.flush()
            except KeyboardInterrupt:
                break
    ser.close()
    print(f"Data saved to {output_filename}")

def analyze_eeprom_data(filename):
    with open(filename, 'r') as f:
        data = f.readlines()
    
    address_data = {}
    address_pattern = re.compile(r"Address 0x([0-9A-F]+): 0x([0-9A-F]+)")

    for line in data:
        match = address_pattern.match(line.strip())
        if match:
            address = int(match.group(1), 16)
            value = int(match.group(2), 16)
            if address in address_data:
                print(f"Duplicate address found: 0x{address:X}")
            address_data[address] = value

    print(f"Total unique addresses read: {len(address_data)}")
    expected_addresses = set(range(0x0, 0x400))  # Addresses from 0x0 to 0x3FF
    read_addresses = set(address_data.keys())
    missing_addresses = expected_addresses - read_addresses

    if missing_addresses:
        print(f"Missing addresses: {sorted(missing_addresses)}")
    else:
        print("All addresses are present and unique.")

def convert_to_bin(input_filename, output_filename):
    with open(input_filename, 'r') as f:
        data = f.readlines()
    
    address_data = {}
    address_pattern = re.compile(r"Address 0x([0-9A-F]+): 0x([0-9A-F]+)")

    for line in data:
        match = address_pattern.match(line.strip())
        if match:
            address = int(match.group(1), 16)
            value = int(match.group(2), 16)
            address_data[address] = value

    # Create a byte array to store the EEPROM data
    eeprom_data = bytearray(1024)  # Assuming EEPROM size of 1024 bytes (0x0 to 0x3FF)

    for address, value in address_data.items():
        eeprom_data[address] = value

    # Write the byte array to a binary file
    with open(output_filename, 'wb') as bin_file:
        bin_file.write(eeprom_data)

    print(f"Binary data written to {output_filename}")

def verify_bin_file(bin_filename, text_filename):
    with open(bin_filename, 'rb') as bin_file:
        bin_data = bin_file.read()
    
    with open(text_filename, 'r') as f:
        text_data = f.readlines()
    
    address_data = {}
    address_pattern = re.compile(r"Address 0x([0-9A-F]+): 0x([0-9A-F]+)")

    for line in text_data:
        match = address_pattern.match(line.strip())
        if match:
            address = int(match.group(1), 16)
            value = int(match.group(2), 16)
            if bin_data[address] != value:
                print(f"Mismatch at address 0x{address:X}: file 0x{bin_data[address]:X}, expected 0x{value:X}")
                return False

    print("Binary file verification successful!")
    return True

def read_and_print_bin(bin_filename):
    with open(bin_filename, 'rb') as bin_file:
        bin_data = bin_file.read()
        print(f"Reading binary file: {bin_filename}")
    
    for address in range(len(bin_data)):
        value = bin_data[address]
        print(f"Address 0x{address:03X}: 0x{value:02X}")

    print("Binary file read and print successful!")

if __name__ == "__main__":
    # Step 1: Read data from EEPROM via serial and save to file
    read_serial_to_file('COM3', 115200, 'serial_output.txt')
    
    # Step 2: Analyze the data
    analyze_eeprom_data("serial_output.txt")
    
    # Step 3: Convert the data to a binary file
    convert_to_bin("serial_output.txt", "eeprom_data.bin")
    
    # Step 4: Verify the binary file
    verify_bin_file("eeprom_data.bin", "serial_output.txt")
    
    # Step 5: Read and print the data from the binary file
    read_and_print_bin("eeprom_data.bin")
