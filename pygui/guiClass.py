import tkinter as tk
from serialParse import SerialDataProcessor
import threading
import math
import time


class WellPlateGUI(tk.Tk):
    def __init__(self):
        super().__init__()

        self.row_dict = {0: 'A', 2: 'B', 4: 'C', 6: 'D', 8: 'E', 10: 'F', 12: 'G', 14: 'H'}
        self.col_dict = {1: 1, 3: 2, 5: 3, 7: 4, 9: 5, 11: 6, 13: 7, 15: 8}
        self.title("96-Well Plate")
        self.geometry("700x500")
        self.well_diameter = 40

        self.well_counts = {}
        self.colors = ["green", "blue", "red", "orange", "yellow", "purple", "indigo", "violet", "pink", "cyan",
                       "brown", "teal"]

        self.create_well_plate()
        self.create_input()

        self.timer_event = threading.Event()

        # Initialize the SerialDataProcessor instance
        self.serial_processor = SerialDataProcessor(port='COM3', baudrate=9600)

        # Create a thread to process serial data
        self.serial_thread = threading.Thread(target=self.read_serial_data, daemon=True)
        self.serial_thread.start()

        # Looping Timer
        self.timer = threading.Thread(target=self.secTimer, daemon=True)
        self.timer.start()

    def read_serial_data(self):
        while True:
           self.serial_processor.process_serial_data()
           self.ser_update_gui()
            # Update the GUI with the received data

    def secTimer(self):
        while True:
            self.timer_event.set()
            time.sleep(5)

    def ser_update_gui(self):
        # Check if new analog data is available
        if self.serial_processor.pinread_event.is_set():
            adc_data = self.serial_processor.adc_data
            print(adc_data)
            self.serial_processor.pinread_event.clear()

        # Check if new integer data is available
        elif self.serial_processor.blockout_event.is_set():
            blockout_data = self.serial_processor.blockout_data
            print(blockout_data)
            for ele in self.cellInterp(blockout_data):
                tempstr = tk.StringVar()
                tempstr.set(ele)
                self.entry.configure(textvariable=tempstr)
                self.update_well()
            self.serial_processor.blockout_event.clear()

        # No new data
        else:
            pass

    def cellInterp(self, blockout):
        print(blockout)
        tempRow = 0
        tempCol = 0
        blockedWells = []
        for i in blockout:
            if i % 2 == 0 or i == 0:
                tempRow = self.row_dict[i]
            else:
                tempCol = self.col_dict[i]
            for j in blockout:
                if j % 2 == 0 or j == 0:
                    tempRow = self.row_dict[j]
                else:
                    tempCol = self.col_dict[j]
            if not (f"{tempRow}{tempCol}" in blockedWells):
                    blockedWells.append(f"{tempRow}{tempCol}")
        print(blockedWells)
        return blockedWells

    def create_well_plate(self):
        self.canvas = tk.Canvas(self, width=self.well_diameter * 13, height=self.well_diameter * 9)
        self.canvas.pack(pady=20)

        for i in range(8):
            for j in range(12):
                x0 = (j + 1) * self.well_diameter
                y0 = (i + 1) * self.well_diameter
                x1 = x0 + self.well_diameter
                y1 = y0 + self.well_diameter
                self.canvas.create_oval(x0, y0, x1, y1)

        for i in range(8):
            label = chr(ord('A') + i)
            self.canvas.create_text(self.well_diameter / 2, (i + 1.5) * self.well_diameter, text=label)

        for j in range(12):
            label = str(j + 1)
            self.canvas.create_text((j + 1.5) * self.well_diameter, self.well_diameter / 2, text=label)

    def create_input(self):
        self.entry_label = tk.Label(self, text="Enter well identifier (e.g. A1):")
        self.entry_label.pack()

        self.entry = tk.Entry(self)
        self.entry.pack()

        self.submit_button = tk.Button(self, text="Submit", command=self.update_well)
        self.submit_button.pack()

    def update_well(self):
        well_id = self.entry.get().strip().upper()
        if not self.validate_well_id(well_id):
            print("Invalid well identifier.")
            return

        row = ord(well_id[0]) - ord('A')
        col = int(well_id[1:]) - 1

        x0 = (col + 1) * self.well_diameter
        y0 = (row + 1) * self.well_diameter
        x1 = x0 + self.well_diameter
        y1 = y0 + self.well_diameter


        self.well_counts[well_id] = self.well_counts.get(well_id, 0) + 1

        color_index = (self.well_counts[well_id] - 1) % len(self.colors)
        fill_color = self.colors[color_index]
        self.canvas.create_oval(x0, y0, x1, y1, fill=fill_color)

        text_color = "black" if fill_color == "yellow" else "white"
        self.canvas.create_text(x0 + self.well_diameter / 2, y0 + self.well_diameter / 2,
                                text=self.well_counts[well_id], fill=text_color, font=("Arial", 14, "bold"))

    def validate_well_id(self, well_id):
        if len(well_id) < 2 or len(well_id) > 3:
            return False

        row, col = well_id[0], well_id[1:]

        if not row.isalpha() or not col.isdigit():
            return False

        row = ord(row.upper()) - ord('A')
        col = int(col) - 1

        if row < 0 or row >= 8 or col < 0 or col >= 12:
            return False

        return True


if __name__ == "__main__":
    app = WellPlateGUI()
    app.mainloop()
