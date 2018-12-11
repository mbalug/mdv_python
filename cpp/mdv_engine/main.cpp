#include "tcp_server.h"
#include <boost/asio.hpp>

int main(int argc, char* argv[])
{
  try
  {
    // if (argc != 2)
    // {
    //   std::cerr << "Usage: async_tcp_echo_server <port>\n";
    //   return 1;
    // }

    boost::asio::io_context io_service;

    // namespace std; // For atoi.
    mdv::TcpServer s(io_service, 8081);

    io_service.run();
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
