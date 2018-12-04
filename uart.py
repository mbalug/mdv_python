import sys
import threading
import time
import serial

class Uart():
    def __init__(self,eng):

        self.engine = eng
        self.ser = serial.Serial('/dev/ttyS0', 9600)
        self.data_to_send = []
        self.stop_rcv_thread = True
        self.uart_active = True
        self.thread = threading.Thread(target=Uart.rcvThread,args=(self,))
        self.thread3 = threading.Thread(target=Uart.threadSender,args=(self,))
    
    def startUart(self):
        self.thread.start()
        self.thread3.start()
    
    def stopUart(self):
        self.stop_rcv_thread = True
        self.stop_rcv_thread = False
        self.thread.join()
        self.thread3.join()

    def rcvThread(self):
        while self.stop_rcv_thread: 
            line = self.ser.readline().decode()
            #self.ser.write(line.encode())
            self.engine.snd_uart_to_tcp(line)
            # if line != '':
            print("UART RCV" + line)
        print("UART THREAD RCV DONE")   

    def threadSender(self):
        print("Thread uart send is started")
        time.sleep(1)
        while self.uart_active:
            time.sleep(0.1)
            if len(self.data_to_send) > 0:
                msg=self.data_to_send[0]
                self.data_to_send.remove(self.data_to_send[0])
                print("Sending over UART", msg.encode())
                self.ser.write(msg.encode())
        print("UART THREAD SENDER DONE")   

    def sendData(self,data):
        self.data_to_send.append(data)