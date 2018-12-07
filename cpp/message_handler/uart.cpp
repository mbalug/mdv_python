#include "uart.h"
#include "message_handler.h"
#include <iostream>
namespace mdv
{
Uart::Uart(std::string port, unsigned int baud_rate, MessageHandler &msg_handler)
    : m_ioc(), m_serial(m_ioc, port), m_done(false), m_message_handler(msg_handler)
{
    m_serial.set_option(boost::asio::serial_port_base::baud_rate(baud_rate));
    std::string msg = "Hello";
    writeString(msg);
    //std::cout << readLine();
    m_receive_thread = std::thread(&Uart::receiveThread, this);
}

void Uart::writeString(std::string msg)
{
    boost::asio::write(m_serial, boost::asio::buffer(msg.c_str(), msg.size()));
}

std::string Uart::readLine()
{
    char c;
    std::string result;
    for (;;)
    {
        boost::asio::read(m_serial, boost::asio::buffer(&c, 1));
        switch (c)
        {
        case '\r':
            break;
        case '\n':
            return result += c;
        default:
            result += c;
        }
    }
}

void Uart::receiveThread()
{
    while (!m_done)
    {
        std::string msg = readLine();
        std::cout << msg;
        m_message_handler.sendMessageToTcp(msg);
    }
    // socket_.async_read_some(boost::asio::buffer(data_, max_length),
    //                         boost::bind(&Session::handle_read,
    //                                     this,
    //                                     boost::asio::placeholders::error,
    //                                     boost::asio::placeholders::bytes_transferred));
    std::cout << "In thread";
    // while(!m_done){
    // }
    //delete this;
}

} // namespace mdv