import sys
import serial
MCU = serial.Serial('/dev/ttyACM0', 115200);
while True:
    char = sys.stdin.read(1);
    if(char == 'w'):
        print 'up!!!! and away!!!'
    if(char == 'a'):
        print 'A little more to the left...'
    if(char == 's'):
        print 'Down she goes!'
    if(char == 'd'):
        print "That's right!"
