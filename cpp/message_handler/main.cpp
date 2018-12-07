#include "message_handler.h"
#include "tcp_server.h"
#include "uart.h"
#include <boost/asio.hpp>
#include <iostream>
// compile:
// arm-linux-gnueabihf-g++ main.cpp uart.cpp tcp_server.cpp message_handler.cpp -lpthread -L/usr/local/boost/lib -lboost_system -I/usr/local/boost/include -o mdv_message_handler
int main(int argc, char const *argv[])
{
    boost::asio::io_context m_ioc;
    std::string id = "msg_handler";
    mdv::MessageHandler msg_handler(id);
    mdv::TcpServer s(m_ioc, 8081, msg_handler);
    mdv::Uart uart("/dev/ttyS0", 9600, msg_handler);
    msg_handler.activate(s, uart);
    m_ioc.run();

    return 0;
}
