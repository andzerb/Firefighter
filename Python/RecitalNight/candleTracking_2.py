import sys
import serial
import time
import math

SIU = serial.Serial('/dev/ttyACM2', 115200, timeout=0.75)
MCU = serial.Serial('/dev/ttyACM0', 115200, timeout=0.75)
time.sleep(1);
distCandle = 0
inputString = ""
flameDist = 50
mmper255th = 0.1

while distCandle==0 or distCandle>= 200:
    time.sleep(0.2)
    inputString = SIU.readline()
    print(inputString);
    # Getting two ints from the string for Heron's forumula
    if(inputString.startswith("Okay ")):
        print("Correct")
        MCU.write("0,0,0");
        f1index = inputString.index("F1L")
        f2index = inputString.index("F2R")
        f1 = int(inputString[f1index + 3:f2index])
        MCU.write("0,0,160")
        inputstring = SIU.readline()
        f1index = inputString.index("F1L")
        f2index = inputString.index("F2R")
        f2 = int(inputString[f1index + 3:f2index])
        f1*= mmper255th
        f2*= mmper255th
        semiP = (f1 + f2 + flameDist) * 1/2
        area = math.sqrt(semiP * (semiP-f1) * (semiP-f2) * (semiP-flameDist));
        y = 2 * area * 1/flameDist
        x = math.sqrt(f2**2 - y**2)
        toSend = str(x) + "," + str(y)+ '\n'
        print(toSend)
        MCU.write(toSend)
        
