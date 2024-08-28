#include "websocket.hpp"
#include "spdlog/spdlog.h"

Websocket::Websocket(uint16_t port, std::ostream* out, std::ostream* err,
                     std::function<std::string(std::string)> on_message,
                     std::function<std::string()> on_update)
    : m_on_message(on_message), m_on_update(on_update), m_port(port)
{
    // Set logging settings
    m_server.get_alog().set_custom(out != &std::cout);
    m_server.get_elog().set_custom(err != &std::cerr);
    m_server.get_alog().set_ostream(out);
    m_server.get_elog().set_ostream(err);
    m_server.set_access_channels(websocketpp::log::alevel::connect +
                                websocketpp::log::alevel::disconnect);
    m_server.clear_access_channels(websocketpp::log::alevel::frame_payload +
                                websocketpp::log::alevel::frame_header);

    // Initialize Asio
    m_server.init_asio();

    // Register our message handler
    m_server.set_open_handler(std::bind(&Websocket::on_open, this, ::_1));
    m_server.set_close_handler(std::bind(&Websocket::on_close, this, ::_1));
    m_server.set_message_handler(std::bind(&Websocket::on_message, this, ::_1, ::_2));
}

void Websocket::run()
{
    try
    {
        // listen on specified port
        m_server.listen(m_port);
        // Start the server accept loop
        m_server.start_accept();
        // Start the ASIO io_service run loop
        m_server.run();
    } catch (websocketpp::exception const& e) {
        spdlog::critical("{}", e.what());
        exit(1);
    } catch (...) {
        spdlog::critical("other exception");
        exit(1);
    }
}

void Websocket::stop()
{
    m_server.stop_listening();
    m_server.stop();
}

void Websocket::on_open(websocketpp::connection_hdl hdl)
{
    {
        std::lock_guard<std::mutex> guard(m_action_lock);
        m_actions.push(action(SUBSCRIBE, hdl));
    }
    m_action_cond.notify_one();
}

void Websocket::on_close(websocketpp::connection_hdl hdl)
{
    {
        std::lock_guard<std::mutex> guard(m_action_lock);
        m_actions.push(action(UNSUBSCRIBE, hdl));
    }
    m_action_cond.notify_one();
}

void Websocket::on_message(websocketpp::connection_hdl hdl, Server::message_ptr msg)
{
    // queue message up for sending by processing thread
    {
        std::lock_guard<std::mutex> guard(m_action_lock);
        m_actions.push(action(MESSAGE, hdl, msg));
    }
    m_action_cond.notify_one();
}

void Websocket::process_messages()
{
    while (1)
    {
        std::unique_lock<std::mutex> lock(m_action_lock);

        while (m_actions.empty())
        {
            m_action_cond.wait(lock);
        }

        action a = m_actions.front();
        m_actions.pop();

        lock.unlock();

        switch (a.type) {
            case SUBSCRIBE:
            {
                std::lock_guard<std::mutex> guard(m_connection_lock);
                m_connections.insert(a.hdl);
            }
            break;
            case UNSUBSCRIBE:
            {
                std::lock_guard<std::mutex> guard(m_connection_lock);
                m_connections.erase(a.hdl);
            }
            break;
            case MESSAGE:
            {
                std::string response = m_on_message(a.msg->get_payload());
                if (response.empty() || response == "{}")
                {
                    continue;
                }
                m_server.send(a.hdl, response, websocketpp::frame::opcode::TEXT);
            }
            break;
            default:
                break;
        }
    }
}

void Websocket::process_broadcast()
{
    while (1)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        std::string update = m_on_update();
        if (update.empty() || update == "{}")
        {
            continue;
        }
        {
            std::lock_guard<std::mutex> guard(m_connection_lock);
            for (auto it : m_connections)
            {
                m_server.send(it, update, websocketpp::frame::opcode::TEXT);
            }
        }
    }
}