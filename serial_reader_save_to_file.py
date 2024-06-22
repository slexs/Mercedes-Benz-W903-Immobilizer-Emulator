import serial

def read_serial_to_file(com_port, baud_rate, output_filename, num_lines):
    # Open serial port
    ser = serial.Serial(com_port, baud_rate)  # Adjust COM port and baud rate as needed

    # Open or create a file for writing
    with open(output_filename, "w") as f:
        for _ in range(num_lines):
            line = ser.readline().decode('utf-8')  # Read a line from the serial port
            print(line, end='')  # Print to console
            f.write(line)  # Write to file
            f.flush()  # Explicitly flush the buffer

    ser.close()
    print(f"Data saved to {output_filename}")

if __name__ == "__main__":
    # Define parameters
    com_port = 'COM3'
    baud_rate = 115200
    output_filename = 'serial_output.txt'
    num_lines = 1024  # Number of lines to read

    # Read data from EEPROM via serial and save to file
    read_serial_to_file(com_port, baud_rate, output_filename, num_lines)
