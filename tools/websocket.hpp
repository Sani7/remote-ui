#pragma once

#include <functional>
#include <set>

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

typedef websocketpp::server<websocketpp::config::asio> Server;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;

/* on_open insert connection_hdl into channel
 * on_close remove connection_hdl from channel
 * on_message queue send to all channels
 */

enum action_type
{
    SUBSCRIBE,
    UNSUBSCRIBE,
    MESSAGE,
    BROADCAST
};

struct action
{
    action(action_type t, websocketpp::connection_hdl h) : type(t), hdl(h)
    {
    }
    action(action_type t, websocketpp::connection_hdl h, Server::message_ptr m) : type(t), hdl(h), msg(m)
    {
    }

    action_type type;
    websocketpp::connection_hdl hdl;
    Server::message_ptr msg;
};

class Websocket
{
  public:
    Websocket(uint16_t port, std::ostream *out, std::ostream *err, std::function<std::string(std::string)> on_message,
              std::function<std::string()> on_update);
    void run();
    void stop();
    void process_messages();
    void process_broadcast();

  private:
    void on_open(websocketpp::connection_hdl hdl);
    void on_close(websocketpp::connection_hdl hdl);
    void on_message(websocketpp::connection_hdl hdl, Server::message_ptr msg);

  private:
    std::function<std::string(std::string)> m_on_message;
    std::function<std::string()> m_on_update;
    Server m_server;
    uint16_t m_port;
    typedef std::set<websocketpp::connection_hdl, std::owner_less<websocketpp::connection_hdl>> con_list;
    con_list m_connections;

    std::queue<action> m_actions;

    std::mutex m_action_lock;
    std::mutex m_connection_lock;
    std::condition_variable m_action_cond;
};