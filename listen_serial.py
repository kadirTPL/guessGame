import serial
import time

# Change this to your Arduino COM port (Windows) or /dev/ttyUSB0 (Linux/Mac)
SERIAL_PORT = 'COM3'
BAUD_RATE = 9600
OUTPUT_FILE = 'highscores.csv'

def main():
    try:
        ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
        print(f"Connected to {SERIAL_PORT} at {BAUD_RATE} baud.")
        time.sleep(2)  # wait for Arduino to reset

        with open(OUTPUT_FILE, 'a') as file:
            print(f"Saving incoming data to {OUTPUT_FILE}. Press Ctrl+C to stop.")
            while True:
                line = ser.readline().decode('utf-8').strip()
                if line:
                    print("Received:", line)
                    file.write(line + '\n')
                    file.flush()
    except serial.SerialException:
        print(f"Could not open serial port {SERIAL_PORT}. Make sure it's correct and not in use.")
    except KeyboardInterrupt:
        print("\nExiting program.")
    finally:
        if 'ser' in locals() and ser.is_open:
            ser.close()

if __name__ == '__main__':
    main()

