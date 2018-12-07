
#include <boost/asio.hpp>
#include <string>
#include <thread>
#include <atomic>

namespace mdv
{
class MessageHandler;
class Uart
{
  public:
    Uart(std::string port, unsigned int baud_rate, MessageHandler &msg_handler);
    void writeString(std::string msg);
    std::string readLine();
    void receiveThread();

  private:
    boost::asio::io_context m_ioc;
    boost::asio::serial_port m_serial;
    std::atomic<bool> m_done;
    MessageHandler &m_message_handler;
    std::thread m_receive_thread;
};
} // namespace mdv