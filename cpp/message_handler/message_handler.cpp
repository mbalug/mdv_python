#include "message_handler.h"
#include "uart.h"
#include "tcp_server.h"
namespace mdv
{
MessageHandler::MessageHandler(std::string &id) : m_id(id) {}

void MessageHandler::activate(TcpServer &tcp_server, Uart &uart)
{
    m_tcp_server = &tcp_server;
    m_uart = &uart;
}

void MessageHandler::sendMessageToUart(std::string &msg)
{
    m_uart->writeString(msg);
}

void MessageHandler::sendMessageToTcp(std::string &msg)
{
    m_tcp_server->writeTcp(msg);
}

} // namespace mdv