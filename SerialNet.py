import serial
import time
from multiprocessing import Array

def deleteArray(category_names):
    category_names[:] = b'0000'
    
ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=1)

def run(category_names):
    while(True):
        ser.reset_input_buffer()
        line = ser.readline().decode('ISO-8859-1').rstrip()
        print(line)
        
        if line == 'constOpen':
            #ser.write(b'hello')
            run = True
            while run:
                line = ser.readline().decode('ISO-8859-1').rstrip()
                if line == 'ret':
                    #ser.write(b'goodbye')
                    run = False
        #print(category_names[2])
        #print(category_names[:])
        if category_names[2] != b'0':
            #print('hello')
            category_names[3] = b'1'
            if category_names[0] == category_names[1] and category_names[1]== category_names[2]:
                ser.write(b'3')
                ser.write(category_names[0])
                deleteArray(category_names)
            #following events happen when different kinds of snacks are running 
            elif category_names[0] == category_names[1] and category_names[1] != category_names[2]:
                ser.write(b'3')
                ser.write(category_names[0])
                deleteArray(category_names)
            elif category_names[0] == category_names[2] and category_names[1] != category_names[2]:
                ser.write(b'3')
                ser.write(category_names[0])
                deleteArray(category_names)
            elif category_names[1] == category_names[2] and category_names[0] != category_names[1]:
                ser.write(b'3')
                ser.write(category_names[1])
                deleteArray(category_names)
            else:
                ser.write(b'3')
                ser.write(category_names[0])
                deleteArray(category_names)
         
               
