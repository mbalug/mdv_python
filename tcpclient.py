import socket
import sys
import threading
import time

class TCPClient:
    def __init__(self, engine, host, port):
        self.eng = engine
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        print ('Socket created')

        #Bind socket to local host and port
        try:
            self.socket.bind((host, port))
        except socket.error as msg:
            print ('Bind failed. Error Code : ' + str(msg[0]) + ' Message ' + msg[1])
            sys.exit()
        print ('Socket bind complete')
        self.socket.listen(10)
        print ('Socket now listening')
        self.tcp_active=False
        self.data_to_send = []

    def startServer(self):
        thread = threading.Thread(target=TCPClient.waitConnection,args=(self,))
        thread.start()

    def waitConnection(self):
        while True:
            conn, addr = self.socket.accept()
            print ('Connected with ' + addr[0] + ':' + str(addr[1]))

            thread1 = threading.Thread(target=TCPClient.clientthread,args=(self,conn,))
            thread1.start()
        self.socket.close()
        thread1.join()

    def clientthread(self, conn):

        thread3 = threading.Thread(target=TCPClient.threadSender,args=(self,conn,))
        thread3.start()
        self.tcp_active= True
        while True:
            #Receiving from client
            data = conn.recv(1024)
            print(data)
            reply = 'OK...' + data.decode()
            if not data:
                self.tcp_active= False
                thread3.join()
                break
            self.eng.snd_tcp_to_uart(data.decode())
            print("Sending:" + reply)
            conn.sendall(reply.encode())

        conn.close()

    def threadSender(self,conn):
        print("Thread is startd")
        time.sleep(1)
        while self.tcp_active:
            time.sleep(0.1)
            if len(self.data_to_send) > 0:
                msg=self.data_to_send[0]
                self.data_to_send.remove(self.data_to_send[0])
                print("Sending over tcp")
                conn.sendall(msg.encode())

    def sendData(self,data):
        self.data_to_send.append(data)