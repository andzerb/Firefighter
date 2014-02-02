import sys
import serial
MCU = serial.Serial('/dev/ttyACM0', 115200);

while True:
    char = sys.stdin.read(1);
    if(char == 'w'):
        print 'up!!!! and away!!!'
        MCU.write("0,10");
        MCU.write('\n');
    if(char == 'a'):
        print 'A little more to the left...'
        MCU.write("-10,0");
        MCU.write('\n');
    if(char == 's'):
        print 'Down she goes!'
        MCU.write("0,-10");
        MCU.write('\n');
    if(char == 'd'):
        print "That's right!"
        MCU.write("10,0");
        MCU.write('\n');
