import sys
import threading
import time
import serial

class Uart():
    def __init__(self,eng):

        self.engine = eng
        self.ser = serial.Serial('/dev/ttyAMA0', 9600, timeout = 1)
        self.data_to_send = []
    
    def startUart(self):
        thread = threading.Thread(target=Uart.rcvThread,args=(self,))
        thread.start()

    def rcvThread(self):
        while True: 
            line = self.ser.readline().decode()
            if line != '':
                print("UART RCV" + line)
    
    def sndMessage(self,msg):
        print("uart-----"+msg)

    def sendData(self,data):
        self.data_to_send.append(data)