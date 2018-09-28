import tcpclient
import time
import engine
import uart


eng = engine.Engine()
uart = uart.Uart(eng)
uart.startUart()
srv = tcpclient.TCPClient(eng,'192.168.0.28',8081)
srv.startServer()

while True:

    to_uart = eng.get_to_uart_messages()
    if len(to_uart)>0:
        for item in to_uart:
            uart.sendData(item)
    
    to_tcp = eng.get_to_tcp_messages()
    if len(to_tcp)>0:
        for item in to_tcp:
            srv.sendData(item)
    time.sleep(0.1)






