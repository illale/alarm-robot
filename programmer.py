import tkinter as tk
import serial
import sys
import glob




class MainWindow:
    def __init__(self):
        self.window = tk.Tk()
        self.INDEX = 0
        self.SERIAL = "Test"
        self.TEXT = "Empty"
        self.PORTS = []
        self.CONTINUOS = False


        self.window.title("Alarm-Robot Programmer")
        self.window.geometry("300x500")
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
        self.serials = tk.Listbox(self.window)
        self.insert_ports()
        self.serials.pack()

    def get_button(self):
        self.button = tk.Button(self.window, text="Select Serial", command=self.button_command)
        self.button.pack()

    def time_button(self):
        self.time_button = tk.Button(self.window, text="Insert", command=self.get_entry_text)
        self.time_button.pack()
    
    def get_entry_text(self):
        text = self.entry.get()
        if (int(text.split(":")[0]) > 24 ):
            print("Not acceptable value")
        else:
            self.TEXT = text
        print(self.TEXT)

    def button_command(self):
        self.INDEX = self.serials.curselection()
        self.SERIAL = self.serials.curselection()
        print(self.INDEX)

    def insert_ports(self):
        if (len(self.PORTS) > 0):
            for i, port in enumerate(self.PORTS):
                self.serials.insert(i, port)
        else:
            self.serials.insert(0, "Serial 1")
            self.serials.insert(1, "Serial 2")


    def create_entry(self):
        self.entry = tk.Entry(self.window, text="21:00")
        self.entry.pack()

    def set_continuos(self):
        self.CONTINUOS = not self.CONTINUOS
        print(self.CONTINUOS)

    def create_boolean_check(self):
        self_check = tk.Checkbutton(self.window, onvalue=1, offvalue=0,text="Continuos", command=self.set_continuos)
        self_check.pack()

    def list_available_serials(self):
        """
        This took inspiration from https://stackoverflow.com/questions/12090503/listing-available-com-ports-with-python
        """
        if sys.platform.startswith('win'):
            ports = ['COM{}'.format(i + 1) for i in range(256)]
        elif sys.platform.startswith('linux'):
            ports = glob.glob('/dev/tty[A-Za-z]*')

        for port in ports:
            try:
                serial = serial.Serial(port)
                serial.close()
                self.PORTS.append(port)
            except: 
                pass

    def main_loop(self):
        self.window.mainloop()

if __name__=="__main__":
    window = MainWindow()
    window.main_loop()