import sys
import serial
import time
import math

SIU = serial.Serial('/dev/ttyACM0', 115200, timeout=0.75)
MCU = serial.Serial('/dev/ttyACM1', 115200, timeout=0.75)
time.sleep(1);


def nack():
    done = 0
    while(done == 0):
            inputString = MCU.readline()
            time.sleep(0.05)
            if("Done" in inputString):
                    done = 1

if __name__ == "__main__":
    while(1):
        MCU.write("0,500\n")
        nack()
        MCU.write("-500,0\n")
        nack()
        MCU.write("0,-500\n")
        nack()
        MCU.write("500,0\n")
        nack()
    
