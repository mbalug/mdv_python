#include "tcp_server.h"
#include "message_handler.h"
#include <boost/bind.hpp>

using boost::asio::ip::tcp;
namespace mdv
{

TcpServer::TcpServer(boost::asio::io_context &ioc, int port, MessageHandler &msg_handler)
    : m_ioc(ioc), acceptor_(ioc, tcp::endpoint(tcp::v4(), port)), m_msg_handler(msg_handler)
{
    start_accept();
}

void TcpServer::start_accept()
{
    Session *new_session = new Session(m_ioc, m_msg_handler);
    acceptor_.async_accept(new_session->socket(),
                           boost::bind(&TcpServer::handle_accept,
                                       this,
                                       new_session,
                                       boost::asio::placeholders::error));
}

void TcpServer::handle_accept(Session *new_session, const boost::system::error_code &error)
{
    if (!error)
    {
        my_session = new_session;
        std::cout << "Starting session\n";
        new_session->start();
    }
    else
    {
        new_session->stopSession();
        delete new_session;
    }
    start_accept();
}

void TcpServer::writeTcp(std::string msg)
{
    my_session->write(msg);
}

Session::Session(boost::asio::io_context &ioc, MessageHandler &msg_handler)
    : m_socket(ioc), m_msg_handler(msg_handler)
{
}
tcp::socket &Session::socket()
{
    return m_socket;
}

void Session::start()
{
    m_socket.async_read_some(boost::asio::buffer(m_data, max_length),
                             boost::bind(&Session::handle_read,
                                         this,
                                         boost::asio::placeholders::error,
                                         boost::asio::placeholders::bytes_transferred));
}

void Session::handle_read(const boost::system::error_code &error, size_t bytes_transferred)
{
    if (!error)
    {

        std::string str(m_data, bytes_transferred);
        m_msg_handler.sendMessageToUart(str);
        m_socket.async_read_some(boost::asio::buffer(m_data, max_length),
                                 boost::bind(&Session::handle_read,
                                             this,
                                             boost::asio::placeholders::error,
                                             boost::asio::placeholders::bytes_transferred));
    }
    else
    {
        std::cout << "handle read fail" << error << std::endl;
        stopSession();
        //start_accept();
    }
}

void Session::write(std::string msg)
{
    std::cout << "Writing to tcp\n";
    boost::asio::write(m_socket, boost::asio::buffer(msg.c_str(), msg.size()));
}

void Session::stopSession()
{
    delete this;
}
} // namespace mdv