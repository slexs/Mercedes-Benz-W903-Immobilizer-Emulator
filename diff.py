# Read two binary files and print the diff between file 1 and file 2 

import os

def get_file_size(file_path):
    """Returns the size of the file at 'file_path' in bytes."""
    try:
        # Use os.path.getsize() to get the file size directly without opening the file
        size = os.path.getsize(file_path)
        return size
    except OSError as e:
        # Handle errors (e.g., file not found) and return a message
        return f"Error: {e}"
    
def read_bin_file(bin_filename):
    with open(bin_filename, 'rb') as bin_file:
        bin_data = bin_file.read()
        print(f"Reading binary file: {bin_filename}")
        return bin_data
    
def compare_bin_files(bin_filename1, bin_filename2):
    bin_data1 = read_bin_file(bin_filename1)
    bin_data2 = read_bin_file(bin_filename2)
    
    if len(bin_data1) != len(bin_data2):
        print("Files are of different sizes. Cannot compare.")
        return
    
    diff_count = 0
    for i in range(len(bin_data1)):
        if bin_data1[i] != bin_data2[i]:
            print(f"Mismatch at address 0x{i:03X}: file1 0x{bin_data1[i]:02X}, file2 0x{bin_data2[i]:02X}")
            diff_count += 1
    
    if diff_count == 0:
        print("Files are identical.")
    else:
        print(f"Total differences found: {diff_count}")
        
if __name__ == "__main__":
    # Example usage
    bin_filename1 = 'eeprom_data.bin'
    bin_filename2 = 'eeprom fix.bin'
    compare_bin_files(bin_filename1, bin_filename2)
    
    # # Example usage
    # file_path = 'G:\Python-playground\w903-new\Mercedes-Benz-W903-Immobilizer-Emulator\eeprom fix.bin'  # Replace 'example.txt' with the path to your file
    # print(f"The size of the file is {get_file_size(file_path)} bytes.")