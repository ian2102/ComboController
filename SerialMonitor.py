import os
import serial
import sys
from serial.tools import list_ports

# Function to list available serial ports
def list_available_ports():
    ports = list(list_ports.comports())
    if not ports:
        print("No serial ports found.")
    else:
        print("Available serial ports:")
        for idx, port_info in enumerate(ports, start=1):
            print(f"{idx}. {port_info.device}")
    return ports

def main():
    try:
        # List available ports and let the user select one
        available_ports = list_available_ports()
        port_selection = input("Enter the number of the port to use (e.g., 1) or press Enter for the last option: ")

        if not port_selection:
            # If Enter is pressed without entering a number, select the last available port
            selected_port_info = available_ports[-1]
            print(f"No input provided, selecting the last available port: {selected_port_info.device}")
        else:
            selected_port_info = available_ports[int(port_selection) - 1]

        selected_port = selected_port_info.device

        # Define the baud rate (adjust as needed)
        baud_rate = 9600

        # Create and configure the serial port
        ser = serial.Serial(selected_port, baud_rate)

        # Print a message indicating that the program is listening on the selected port
        print(f"Listening on {selected_port}")
        
        try:
            while True: # todo make backspace work "\b"
                data = ser.readline().decode('utf-8').strip()
                sys.stdout.write(data)
                sys.stdout.flush()

        except KeyboardInterrupt:
            print("Serial monitoring stopped.")

        finally:
            # Close the serial port and perform cleanup
            ser.close()

    #except ValueError:
        #print("Error: Please enter a valid number.")
    except IndexError:
        print("Error: Invalid port selection. Please select a valid port.")
    except Exception as e:
        print(f"An unexpected error occurred: {str(e)}")

if __name__ == "__main__":
    main()
