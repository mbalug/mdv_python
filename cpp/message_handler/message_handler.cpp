#include "message_handler.h"
#include "uart.h"
#include "tcp_server.h"
#include <string>
//#define RAPIDJSON_HAS_STDSTRING
#include "rapidjson/document.h"
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
    std::vector<std::string> messages;
    if (msg.find("FR") != std::string::npos || msg.find("FL") != std::string::npos || msg.find("RR") != std::string::npos || msg.find("RL") != std::string::npos)
    {
        if (msg.find("FR") != std::string::npos)
        {
            auto result = parseSteeringControl(msg);
            messages.push_back("FL" + std::to_string(std::get<0>(result)) + "FR" + std::to_string(std::get<1>(result)));
        }
        else if (msg.find("FL") != std::string::npos)
        {
            auto result = parseSteeringControl(msg);
            messages.push_back("FL" + std::to_string(std::get<1>(result)) + "FR" + std::to_string(std::get<0>(result)));
        }
        else if (msg.find("RR") != std::string::npos)
        {
            auto result = parseSteeringControl(msg);
            messages.push_back("RL" + std::to_string(std::get<0>(result)) + "RR" + std::to_string(std::get<1>(result)));
        }
        else if (msg.find("RL") != std::string::npos)
        {
            auto result = parseSteeringControl(msg);
            messages.push_back("RL" + std::to_string(std::get<1>(result)) + "RR" + std::to_string(std::get<0>(result)));
        }
    }
    for (const auto &message : messages)
    {
        std::cout << message << std::endl;
        m_uart->writeString(message + '\n');
    }
}

void MessageHandler::sendMessageToTcp(std::string &msg)
{
    m_tcp_server->writeTcp(msg);
}

std::tuple<int, int> MessageHandler::parseSteeringControl(std::string &msg)
{
    int angle = 0;
    int strength = 0;
    rapidjson::Document document;
    rapidjson::ParseResult res = document.Parse(msg.c_str());
    if (!res || !document.IsObject())
    {
        return std::make_tuple(0, 0);
    }
    if (document.HasMember("angle"))
    {
        if (document["angle"].IsInt())
        {
            angle = document["angle"].GetInt();
        }
    }
    if (document.HasMember("strength"))
    {
        if (document["strength"].IsInt())
        {
            strength = document["strength"].GetInt();
        }
    }

    constexpr double MAX_SPEED = 4095 / 100;
    double speed = 0;
    double steering_speed = 0;
    if (strength)
    {
        speed = MAX_SPEED * strength;
        steering_speed = speed - (speed / 90) * angle;
    }
    return std::make_tuple(speed, steering_speed);
}

} // namespace mdv