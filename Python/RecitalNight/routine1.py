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
    MCU.write("0,-762\n")
    nack()
    MCU.write("-762,0\n")
    nack()
    MCU.write("0,400\n")
    nack()
    MCU.write("On\n")
    time.sleep(10)
    MCU.write("Off\n")
    time.sleep(1)
    MCU.write("-10,0\n")
    nack()
    MCU.write("On\n")
    time.sleep(10)
    MCU.write("Off\n")
    time.sleep(1)
    MCU.write("20,0\n")
    nack()
    MCU.write("On\n")
    time.sleep(10)
    MCU.write("Off\n")
    time.sleep(1)
