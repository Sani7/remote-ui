#pragma once
#include <memory>
#include <map>

#include "simulator_base.hpp"

#define INSERT_SIMULATOR(type) m_simulators.insert(std::make_pair(type().name(), std::make_unique<type>()));

class Simulators {
    public:
        Simulators();
        std::string active_simulator_name() const;
        void run();
        void stop();
        void switch_simulator(std::string name);
        std::vector<std::string> list_simulators() const;
        Simulator_base* invoke_active_simulator();
        json changed_UI_items();
    private:
        std::map<std::string, std::unique_ptr<Simulator_base>> m_simulators;
        std::string m_current_simulator;
        json m_before;
};