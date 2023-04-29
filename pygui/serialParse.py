import serial
import threading

class SerialDataProcessor:
    def __init__(self, port, baudrate):
        self.ser = serial.Serial(port, baudrate, timeout=1)
        self.analog_data = []
        self.integer_data = []
        self.pinread_event = threading.Event()
        self.blockout_event = threading.Event()        

    def process_serial_data(self):
        # Read one line from serial input
        data = self.ser.readline().decode().strip().upper()

        if data == "PINREAD":
            # Clear the previous analog data and read new data
            self.analog_data = []
            analog_data_str = self.ser.readline().decode().strip()
            if analog_data_str.startswith('[') and ']' in analog_data_str:
                self.analog_data = eval(analog_data_str)
            self.pinread_event.set()

        elif data == "BLOCKOUT":
            # Clear the previous integer data and read new data
            self.integer_data = []
            integer_data_str = self.ser.readline().decode().strip()
            if integer_data_str.startswith('[') and ']' in integer_data_str:
                self.integer_data = eval(integer_data_str)
            self.blockout_event.set()

        elif data == "RESET":
            # Ignore reset signal
            pass

        else:
            # Unknown data
            print()
            pass
