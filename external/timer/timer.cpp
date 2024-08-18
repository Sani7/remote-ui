#include "timer.hpp"

Timer::Timer(void (*function)()) {
    this->function = function;
}

void Timer::setTimeout(std::chrono::milliseconds delay) {

    this->p_clear = false;
    std::thread t([this, delay]() {
        if(this->p_clear) return;
        std::this_thread::sleep_for(delay);
        if(this->p_clear) return;
        function();
    });
    t.detach();
}

void Timer::setInterval(std::chrono::milliseconds interval) {
    this->p_clear = false;
    std::thread t([this, interval]() {
        while(true) {
            if(this->p_clear) return;
            std::this_thread::sleep_for(interval);
            if(this->p_clear) return;
            function();
        }
    });
    t.detach();
}

void Timer::stop() {
    this->p_clear = true;
}