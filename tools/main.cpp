#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

#include "spdlog/spdlog.h"
#include "spdlog/async.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/rotating_file_sink.h"

#include "simulators.hpp"
#include "websocket.hpp"
#include "message_parser.hpp"

class Spdlog_buffer : public std::stringbuf
{
public:
    int sync() {
        std::string str = this->str();
        str.pop_back();
        spdlog::info(str);
        this->str("");
        return 0;
    }
};

Simulators g_simulators;
Websocket* g_web_socket;
Spdlog_buffer g_buffer;
std::ostream g_out(&g_buffer);

void int_handler(int s)
{
    UNUSED(s);
    g_out << "Caught Ctrl + C" << std::endl;
    if (g_web_socket != nullptr)
        g_web_socket->stop();
    g_simulators.stop();
    g_out << "Shutting down." << std::endl;
    // delay to allow the server to stop gracefully
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    exit(1);
}

void init_logger()
{
    spdlog::init_thread_pool(8192, 1);
    auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt >();
    auto rotating_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("logs/sim_us.txt", 1024*1024*10, 3);
    std::vector<spdlog::sink_ptr> sinks {stdout_sink, rotating_sink};
    auto logger = std::make_shared<spdlog::async_logger>("sim_us", sinks.begin(), sinks.end(), spdlog::thread_pool(), spdlog::async_overflow_policy::block);
    spdlog::set_default_logger(logger);
    spdlog::set_pattern("[%H:%M:%S %z] [%n] [%^---%L---%$] [thread %t] %v");
    spdlog::flush_every(std::chrono::seconds(3));
}

int main(void)
{
    // Setup the signal handler
    struct sigaction sigIntHandler;

    sigIntHandler.sa_handler = int_handler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;

    sigaction(SIGINT, &sigIntHandler, NULL);

    // Initialize the logger
    init_logger();

    // The server starts in this thread
    spdlog::info("Starting server");
    Websocket web_socket(9002, &g_out, [](std::string message) { return message_parser(message); }, []() { return g_simulators.changed_UI_items().dump(); });
    g_web_socket = &web_socket;

    // Start the message processor and broadcast processor in separate threads
    std::thread message_processor_thread(std::bind(&Websocket::process_messages, &web_socket));
    std::thread broadcast_processor_thred(std::bind(&Websocket::process_broadcast, &web_socket));
    
    // Run the asio loop with the main thread
    web_socket.run();

    // Wait for the threads to finish (should never happen)
    message_processor_thread.join();
    broadcast_processor_thred.join();
}