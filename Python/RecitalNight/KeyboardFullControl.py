import sys
import serial
import time
import math
import termios, fcntl, sys, os

fd = sys.stdin.fileno()

oldterm = termios.tcgetattr(fd)
newattr = termios.tcgetattr(fd)
newattr[3] = newattr[3] & ~termios.ICANON & ~termios.ECHO
termios.tcsetattr(fd, termios.TCSANOW, newattr)

oldflags = fcntl.fcntl(fd, fcntl.F_GETFL)
fcntl.fcntl(fd, fcntl.F_SETFL, oldflags | os.O_NONBLOCK)

#SIU = serial.Serial('/dev/ttyACM0', 115200, timeout=0.75)
MCU = serial.Serial('/dev/ttyACM1', 115200, timeout=0.75)
time.sleep(1);

on = 0

def nack():
    done = 0
    while(done == 0):
            inputString = MCU.readline()
            time.sleep(0.05)
            if("Done" in inputString):
                    done = 1

if __name__ == "__main__":
    try:
        while 1:
            try:
                c = sys.stdin.read(1)
                #print ("Got character", repr(c))
                if(c=='w'):
                    MCU.write("0,10\n")
                    print("Forward")
                elif(c=='a'):
                    MCU.write("10,0\n")
                    print("Left")
                elif(c=='s'):
                    MCU.write("0,-10\n")
                    print("Backward")
                elif(c=='d'):
                    MCU.write("-10,0\n")
                    print("Right")
                elif(c=='q'):
                    MCU.write("10,10\n")
                    print("NW")
                elif(c=='e'):
                    MCU.write("-10,10\n")
                    print("NE")
                elif(c=='z'):
                    MCU.write("10,-10\n")
                    print("SW")
                elif(c=='c'):
                    MCU.write("-10,-10\n")
                    print("SE")
		elif(c==' '):
		    if(on == 0):
			MCU.write("On\n")
			on = 1
		    else:
			MCU.write("Off\n")
			on = 0
            except IOError: pass
    finally:
        termios.tcsetattr(fd, termios.TCSAFLUSH, oldterm)
        fcntl.fcntl(fd, fcntl.F_SETFL, oldflags)

