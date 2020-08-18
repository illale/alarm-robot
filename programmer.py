import tkinter as tk
import serial
import sys
import glob

class MainWindow:
    def __init__(self):
        #Initialize values to empty
        self.window = tk.Tk()
        self.INDEX = 0
        self.SERIAL = "Test"
        self.TEXT = "Empty"
        self.PORTS = []
        self.CONTINUOS = False
        self.window.title("Alarm-Robot Programmer")
        self.window.geometry("300x350")
        self.window.resizable(0, 0)
        self.label = tk.Label(self.window, text="All available serial ports")
        self.label.pack()
        self.list_available_serials()
        self.list_serials()
        self.get_button()
        self.create_entry()
        self.create_boolean_check()
        self.time_button()

    def list_serials(self):
        #Create listbox and insert all available serial ports into it
        self.serials = tk.Listbox(self.window)
        self.insert_ports()
        self.serials.pack()

    def get_button(self):
        #Create button that selects the used serial port when pressed
        self.button = tk.Button(self.window, text="Select Serial", command=self.button_command)
        self.button.pack()

    def time_button(self):
        #Button that inserts the wanted time into the arduino board
        self.time_button = tk.Button(self.window, text="Insert", command=self.get_entry_text)
        self.time_button.pack()
    
    def get_entry_text(self):
        #Get the time written into the input box
        text = self.entry.get()
        if (int(text.split(":")[0]) > 24 ):
            print("Not acceptable value")
        else:
            self.TEXT = text
        #Calculate the wanted interval into milliseconds
        time = int(self.TEXT.split(":")[0]) * 3600000 + int(self.TEXT.split(":")[1]) * 60000
        #Write the interval value to the arduino board
        self.SERIAL.write(time)
        if self.CONTINUOS:
            self.SERIAL.write(1)
        else:
            self.SERIAL.write(0)

    def button_command(self):
        #This is the method that takes care of selecting the serial port
        self.INDEX = self.serials.curselection()
        self.SERIAL = self.serials.curselection()
        self.SERIAL = serial_t = serial.Serial(self.PORTS[self.INDEX[0]])

    def insert_ports(self):
        #This method fills the serial list with all available ports.
        if (len(self.PORTS) > 0):
            for i, port in enumerate(self.PORTS):
                self.serials.insert(i, port)
        else:
            #If no available ports are found, then the list is filled with two examples
            self.serials.insert(0, "Serial 1")
            self.serials.insert(1, "Serial 2")

    def create_entry(self):
        #Create input box, that takes time interval to next alaram as input
        self.entry = tk.Entry(self.window, text="21:00")
        self.entry.pack()

    def set_continuos(self):
        #Set boolean value, so that we know if we want continuos alarm
        #E.g that after first alarm, second alarm is automatically set
        #with wanted interval
        self.CONTINUOS = not self.CONTINUOS
        print(self.CONTINUOS)

    def create_boolean_check(self):
        #Create checkbox that can be off/on that decides end value for continuos alarm
        self_check = tk.Checkbutton(self.window, onvalue=1, offvalue=0,text="Continuos", command=self.set_continuos)
        self_check.pack()

    def list_available_serials(self):
        #This part of the code was 'inspired' from Thomas's answer at
        #https://stackoverflow.com/questions/12090503/listing-available-com-ports-with-python
        if sys.platform.startswith("win"):
            ports = ["COM{}".format(i + 1) for i in range(256)]
        elif sys.platform.startswith("linux"):
            ports = glob.glob("/dev/tty[A-Za-z]*")

        #Try if python can open the given serial port, and if it can
        #add it to available serials list
        for port in ports:
            try:
                serial_t = serial.Serial(port)
                serial_t.close()
                self.PORTS.append(port)
            except:
                pass

    def main_loop(self):
        self.window.mainloop()

if __name__=="__main__":
    window = MainWindow()
    window.main_loop()