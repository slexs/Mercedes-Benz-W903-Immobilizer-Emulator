import serial

# Open serial port
ser = serial.Serial('COM3', 115200)  # Adjust COM port and baud rate as needed

# Open or create a file for writing
with open("serial_output.txt", "w") as f:
    while True:
        line = ser.readline().decode('utf-8')  # Read a line from the serial port
        print(line, end='')  # Print to console
        f.write(line)  # Write to file
        f.flush()  # Explicitly flush the buffer
