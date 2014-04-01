import sys
import serial
import time
import math

SIU = serial.Serial('/dev/ttyACM1', 115200, timeout=0.75)
MCU = serial.Serial('/dev/ttyACM0', 115200, timeout=0.75)
time.sleep(1);

lowerThreshold=15
upperThreshold=200
f2 = 0

if __name__ == "__main__":
    while f2 < upperThreshold:
        x = 0
        y = 0
        time.sleep(0.1)
        inputString = SIU.readline()
        SIU.flush();
        MCU.flush();
        print(inputString)
        if(inputString.startswith("Okay ") and "L4" in inputString):
            f1index = inputString.index("F1L")
            f2index = inputString.index("F2M")
            f3index = inputString.index("F3R")
            s1index = inputString.index("S1")
            f1 = int(inputString[f1index + 3:f2index])
            f2 = int(inputString[f2index + 3:f3index])
            f3 = int(inputString[f3index + 3:s1index])
            if(f1>lowerThreshold or f2>lowerThreshold or f3>lowerThreshold):
                if(f1 > f2 and f1 > f3):
                    MCU.write("-50,105\n")
                    #print("left")
                elif(f2 > f1 and f2 > f3):
                    MCU.write("0,105\n")
                    #print("forward")
                elif(f3 > f1 and f3 > f2):
                    MCU.write("50,105\n")
                    #print("right")
    MCU.write("On\n")
    while(f1>lowerThreshold or f2>lowerThreshold or f3>lowerThreshold):
        time.sleep(0.1)
        inputString = SIU.readline()
        SIU.flush();
        #print(inputString)
        if(inputString.startswith("Okay ") and "L4" in inputString):
            f2index = inputString.index("F2M")
            f3index = inputString.index("F3R")
            f2 = int(inputString[f2index + 3:f3index])
    time.sleep(2)
    MCU.write("Off\n")
