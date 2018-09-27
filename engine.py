class Engine:
    def __init__(self):
        self.uart_buffer = []
        self.tcp_buffer = []

    def snd_uart_to_tcp(self, msg):
        self.uart_buffer.append(msg)

    def snd_tcp_to_uart(self, msg):
        self.tcp_buffer.append(msg)

    def get_to_uart_messages(self):
        buffer = self.tcp_buffer
        self.tcp_buffer = []
        return buffer
    
    def get_to_tcp_messages(self):
        buffer = self.uart_buffer
        self.uart_buffer = []
        return buffer

