#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "simulators.hpp"
#include "websocket.hpp"

Simulators simulators;
Websocket* g_web_socket;

void int_handler(int s)
{
    UNUSED(s);
    std::cout << "\nCaught Ctrl + C\nShutting down." << std::endl;
    g_web_socket->stop();
    exit(1); 

}

int main(void)
{
    struct sigaction sigIntHandler;

   sigIntHandler.sa_handler = int_handler;
   sigemptyset(&sigIntHandler.sa_mask);
   sigIntHandler.sa_flags = 0;

   sigaction(SIGINT, &sigIntHandler, NULL);

    Websocket web_socket(9002, [](std::string message) {
        return message;
    });
    g_web_socket = &web_socket;
    web_socket.run();
    

}