import serial
import time

def send_message(messages):
    message = ''
    for i in range(len(messages)):
      message = message + ' ' + messages[i] 
    ser.write((message + '\n').encode())

ser = serial.Serial('COM10', 115200, timeout=1)
line = ser.readline().decode('utf-8').strip()

while(True):
   line = ser.readline().decode('utf-8').strip()
   print(line)
   send_message('goodbye')
   time.sleep(1)