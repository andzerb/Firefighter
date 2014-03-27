import sys
import serial
import time
import math

SIU = serial.Serial('/dev/ttyACM0', 115200, timeout=0.75)
MCU = serial.Serial('/dev/ttyACM1', 115200, timeout=0.75)
time.sleep(1);
distCandle = 0
inputString = ""
flameDist = 10.24
mmper255th = 10

while 1 :
    x = 0
    y = 0
    time.sleep(0.2)
    inputString = SIU.readline()
    print(inputString);
    # Getting two ints from the string for Heron's forumula
    if(inputString.startswith("Okay ")):
        f1index = inputString.index("F1L")
        f2index = inputString.index("F2M")
        s1index = inputString.index("F3R")
        f1 = 10.24/int(inputString[f1index + 3:f2index])
        f2 = 10.24/int(inputString[f2index + 3:s1index])
        semiP = (f1 + f2 + flameDist) * 1/2
        print(semiP - f1)
        area = math.sqrt(abs(semiP * (semiP-f1) * (semiP-f2) * (semiP-flameDist)));
        y = 2 * area * 1/flameDist
        x = math.sqrt(abs(f2**2 - y**2))
        toSend = str(x) + "," + str(y)+ '\n'
        distCandle = f1
        print(toSend)
        MCU.write(toSend)
        
