#include <string>
#include <tuple>
namespace mdv
{
class TcpServer;
class Uart;
class MessageHandler
{
public:
  MessageHandler(std::string &id);

  void activate(TcpServer &tcp_server, Uart &uart);

  void sendMessageToUart(std::string &msg);

  void sendMessageToTcp(std::string &msg);

private:
  std::tuple<int, int> parseSteeringControl(std::string &msg);
  std::string m_id;
  TcpServer *m_tcp_server;
  Uart *m_uart;
};
} // namespace mdv