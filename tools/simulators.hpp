#pragma once
#include <memory>
#include <map>

#include "simulator_base.hpp"

#define INSERT_SIMULATOR(type) p_simulators.insert(std::make_pair(type().name(), std::make_unique<type>()));

class Simulators {
    public:
        Simulators();
        void run();
        void stop();
        void switch_simulator(std::string name);
        Simulator_base* invoke_active_simulator() const;
    private:
        std::map<std::string, std::unique_ptr<Simulator_base>> p_simulators;
        std::string p_current_simulator;
};