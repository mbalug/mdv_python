import sys
import threading
import time

class Uart():
    def __init__(self,eng):
        self.engine = eng
        self.data_to_send = []
    
    def sndMessage(self,msg):
        print("uart-----"+msg)

    def sendData(self,data):
        self.data_to_send.append(data)