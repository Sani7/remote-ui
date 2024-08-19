#pragma once

#include <functional>

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

typedef websocketpp::server<websocketpp::config::asio> Server;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

class Websocket
{
public:
    Websocket(uint16_t port, std::function<std::string(std::string)> on_message);
    void run();
    void stop();
private:
    void on_message(Server* s, websocketpp::connection_hdl hdl, Server::message_ptr msg);   
    Server server; 
    std::function<std::string(std::string)> p_on_message;
};