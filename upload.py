import serial

def calculate_checksum(data):
    """Calculate simple checksum by summing all bytes."""
    return sum(data) % 256

def send_firmware(file_path, com_port, baud_rate=115200):
    try:
        # Open the firmware file and read its content
        with open(file_path, 'rb') as file:
            firmware_data = file.read()
        
        # Display firmware size and request user confirmation
        firmware_size = len(firmware_data)
        print(f"Read firmware file of size: {firmware_size} bytes")
        user_input = input("Press 'Y' to start uploading the firmware or any other key to abort: ")
        if user_input.upper() != 'Y':
            print("Firmware upload aborted.")
            return
        
        # Open the serial port
        with serial.Serial(com_port, baud_rate, timeout=1) as ser:
            total_packets = len(firmware_data) // 64  # Assuming each packet has 64 bytes of data
            if len(firmware_data) % 64 != 0:
                total_packets += 1  # Add extra packet for the remaining bytes

            for i in range(total_packets):
                start_index = i * 64
                end_index = start_index + 64
                packet_data = firmware_data[start_index:end_index]
                
                # Prepare packet
                header = bytes([0xAB, 0xCD])  # Start marker
                seq_number = i.to_bytes(1, 'big')  # Packet sequence number
                packet = header + seq_number + packet_data
                packet += bytes([calculate_checksum(packet_data)])  # Checksum
                footer = bytes([0xDC, 0xBA])  # End marker
                packet += footer
                
                # Send packet
                for ii in range(0,3):
                    ser.write(packet)
                print(f"Sent packet {i+1}/{total_packets}")

            print("All packets sent successfully.")
    except serial.SerialException as e:
        print(f"Error opening serial port: {e}")
    except FileNotFoundError:
        print(f"Error: Firmware file '{file_path}' not found.")
    except Exception as e:
        print(f"An error occurred: {e}")


# Example usage
firmware_path = 'APP_MINI1\MDK-ARM\APP_MINI1\APP_MINI1.bin'
serial_port = 'COM7'  # Adjust this to your COM port
send_firmware(firmware_path, serial_port)
