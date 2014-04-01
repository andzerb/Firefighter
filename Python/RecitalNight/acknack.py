import sys
import serial
import time
import math

SIU = serial.Serial('/dev/ttyACM0', 115200, timeout=0.75)
MCU = serial.Serial('/dev/ttyACM1', 115200, timeout=0.75)
time.sleep(1);
done = 0;

if __name__ == "__main__":
    MCU.write("0,-762\n")
    while(done == 0):
        inputString = MCU.readline()
        if( "Done" in inputString):
                done = 1;
        time.sleep(0.05)
