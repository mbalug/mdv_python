import tcpclient
import time
import engine
import uart
import subprocess
import signal
import os
import sys


#process = subprocess.Popen(['/usr/local/bin/mjpg_streamer', '-o', 'output_http.so', '-i', 'input_raspicam.so -fps 15'])
#/usr/local/bin/mjpg_streamer -o output_http.so -i "input_raspicam.so -fps 15"

eng = engine.Engine()
uart = uart.Uart(eng)
uart.startUart()

srv = tcpclient.TCPClient(eng,'192.168.5.1',8081)
srv.startServer()

done = False

def signal_handler(sig, frame):
        print('You pressed Ctrl+C!')
        sys.exit()
        global done
        srv.stopServer()
        uart.stopUart()
        done = True
        #process.terminate()

signal.signal(signal.SIGINT, signal_handler)

while done == False:

    to_uart = eng.get_to_uart_messages()
    if len(to_uart)>0:
        for item in to_uart:
            uart.sendData(item)
    
    to_tcp = eng.get_to_tcp_messages()
    if len(to_tcp)>0:
        for item in to_tcp:
            srv.sendData(item)
    time.sleep(0.1)





