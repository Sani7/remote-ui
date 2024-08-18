#pragma once
#include <chrono>
#include <thread>

class Timer {
public:
    Timer(void (*function)());
    void setTimeout(std::chrono::milliseconds delay);
    void setInterval(std::chrono::milliseconds interval);
    void stop();
private:
    bool p_clear = false;
    std::mutex p_clear_mutex;
    void (*function)();
};