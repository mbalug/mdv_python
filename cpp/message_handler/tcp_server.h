
#pragma once
#include <atomic>
#include <cstdlib>
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <memory>

using boost::asio::ip::tcp;

namespace mdv
{
class MessageHandler;
class Session
{
public:
  Session(boost::asio::io_context &ioc, MessageHandler &msg_handler);
  tcp::socket &socket();
  void start();
  void stopSession();
  void write(std::string msg);

private:
  void handle_read(const boost::system::error_code &error, size_t bytes_transferred);

  tcp::socket m_socket;
  enum
  {
    max_length = 1024
  };
  char m_data[max_length];
  MessageHandler &m_msg_handler;
};

class TcpServer
{
public:
  TcpServer(boost::asio::io_context &ioc, int port, MessageHandler &msg_handler);
  void writeTcp(std::string msg);

private:
  void start_accept();

  void handle_accept(Session *new_session, const boost::system::error_code &error);

  boost::asio::io_context &m_ioc;
  tcp::acceptor acceptor_;
  MessageHandler &m_msg_handler;
  Session *my_session;
};

} // namespace mdv