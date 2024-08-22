#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

#include "simulators.hpp"
#include "websocket.hpp"
#include "message_parser.hpp"

Simulators g_simulators;
Websocket* g_web_socket;

void int_handler(int s)
{
    UNUSED(s);
    std::cout << "\nCaught Ctrl + C\nShutting down." << std::endl;
    if (g_web_socket != nullptr)
        g_web_socket->stop();
    g_simulators.stop();
    // delay to allow the server to stop gracefully
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    exit(1);
}

int main(void)
{
    // Setup the signal handler
    struct sigaction sigIntHandler;

    sigIntHandler.sa_handler = int_handler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;

    sigaction(SIGINT, &sigIntHandler, NULL);


    // The server starts in this thread
    Websocket web_socket(9002, [](std::string message) { return message_parser(message); }, []() { return g_simulators.changed_UI_items().dump(); });
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