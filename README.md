# PSoC_5_Assignment
This repository contains 3 PSoC creator projects, all about the use of I2C protocol for the communication between PSOC5 CY8C5888LTI-LP097 
microcontroller (on the CY8CKIT-059) as master and the LIS3DH 3-Axis Accelerometer used as slave. In all 3 projects, I2C communication is 
used to configure the sensor and read output data through read and write of the proper registers. Output data are then sent to the pc to 
be plotted and analyzed through UART.
Project 1: temperatura data read and sent to the pc through UART protocol
Project 2: the accelerometer registers are properly configured to output 3 axis data in normal mode (10-bit resolution) at 100 Hz in the ±2g
FSR.
Data read from the sensor are converted in acceleration in mg and sent to the pc through UART communication protocol
Project 3: the accelerometer registers are properly configured to output 3 axis data in high resolution mode (12-bit resolution) at 100 Hz
in the ±4g FSR.
Data read from the sensor are converted in acceleration in m/s^2 and sent to the pc through UART communication protocol. (Data sent through 
UART have a precision of 3 decimals)
