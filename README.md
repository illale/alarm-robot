# alarm-robot
This is the repository for course project for Principles of Digital Fabrication.
Our blog can be found at the documentation platform

# How to build this?

3D Printable files are located in STL directory. These files are ready to print, though some parts may require some minor tweaks.
 - Base v0.stl and Base_TOP v0.stl are for the main body.
 - Torni_Basing_over_servo.stl is for the component that connects the servo and the tower
 - Tower_base v1.stl and Tower_upper v2.stl are for the tower
 - Tyres.stl is for the tyres needed, 
 - And transmitter_something is for potential connection between tyres and motors while using bearings

Optionally if you want to use wood as the base, the body.svg
is file that contains drawing for simple wooden body, that can be cut with laser cutter.

The software for the Arduino is located at main/main.ino
To copy the circuit we used, there is images of the circuit at images directory

To run the GUI app, you need Python 3.8 and pyserial install
To install pyserial:
  pip install pyserial

Base contains entire 3D model of the robot, and components contains old code files that were used in main.ino
