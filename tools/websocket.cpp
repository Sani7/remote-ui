#include "websocket.hpp"


Websocket::Websocket(uint16_t port, std::function<std::string(std::string)> on_message)
    : p_on_message(on_message)
{
        try {
        // Set logging settings
        server.set_access_channels(websocketpp::log::alevel::all);
        server.clear_access_channels(websocketpp::log::alevel::frame_payload);

        // Initialize Asio
        server.init_asio();

        // Register our message handler
        server.set_message_handler(bind(&Websocket::on_message, this, &server, ::_1, ::_2));

        // Listen on port 9002
        server.listen(port);
    } catch (websocketpp::exception const & e) {
        std::cout << e.what() << std::endl;
    } catch (...) {
        std::cout << "other exception" << std::endl;
    }
}

void Websocket::run()
{
    try
    {
        // Start the server accept loop
        server.start_accept();

        // Start the ASIO io_service run loop
        server.run();
    } catch (websocketpp::exception const & e) {
        std::cout << e.what() << std::endl;
    } catch (...) {
        std::cout << "other exception" << std::endl;
    }
}

void Websocket::stop()
{
    server.stop_listening();
    server.stop();
}

void Websocket::on_message(Server* s, websocketpp::connection_hdl hdl, Server::message_ptr msg) {
    std::cout << "on_message called with hdl: " << hdl.lock().get()
              << " and message: " << msg->get_payload()
              << std::endl;
    std::string response = p_on_message(msg->get_payload());
    try {
        s->send(hdl, response, msg->get_opcode());
    } catch (websocketpp::exception const & e) {
        std::cout << "Response failed because: "
                  << "(" << e.what() << ")" << std::endl;
    }
}