import serial
import threading
import time

class SerialDataProcessor:
    def __init__(self, port, baudrate):
        self.ser = serial.Serial(port, baudrate, timeout=1)
        self.adc_data = []
        self.blockout_data = []
        self.pinread_event = threading.Event()
        self.blockout_event = threading.Event()
        self.last_blockout_data = []
        self.total_output_ct = 0
        self.same_output_ct = 0       

    def process_serial_data(self):
        # Read one line from serial input
        data = self.ser.readline().decode().strip().upper()

        if data == "PINREAD":
            # Clear the previous analog data and read new data
            self.adc_data = []
            adc_data_str = self.ser.readline().decode().strip()
            if adc_data_str.startswith('[') and ']' in adc_data_str:
                self.adc_data = eval(adc_data_str)
            self.pinread_event.set()

        elif data == "BLOCKOUT":
            # Clear the previous integer data and read new data
            current_time = time.time()
            self.last_blockout_data = self.blockout_data
            self.blockout_data = []
            blockout_data_str = self.ser.readline().decode().strip()
            if blockout_data_str.startswith('[') and ']' in blockout_data_str:
                self.blockout_data = eval(blockout_data_str)
            self.blockout_event.set()     

        elif data == "RESET":
            # Ignore reset signal
            pass

        else:
            # Unknown data
            print()
            pass
