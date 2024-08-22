#include "simulators.hpp"
#include "test_sim.hpp"

Simulators::Simulators()
{
    
}

void Simulators::run()
{
    if (this->m_simulators.empty())
    {
        return;
    }
    if (this->m_current_simulator.empty())
    {
        return;
    }

    this->m_simulators.at(m_current_simulator)->run();
}

void Simulators::stop()
{
    if (this->m_simulators.empty())
    {
        return;
    }
    if (this->m_current_simulator.empty())
    {
        return;
    }

    this->m_simulators.at(m_current_simulator)->stop();
}

void Simulators::switch_simulator(std::string name)
{
    if (this->m_simulators.empty())
    {
        return;
    }
    if (!this->m_current_simulator.empty())
    {
        this->m_simulators.at(m_current_simulator)->stop();
    }

    this->m_current_simulator = name;
    this->m_simulators.at(m_current_simulator)->run();
}

std::vector<std::string> Simulators::list_simulators() const
{
    std::vector<std::string> names;
    for (const auto& simulator : this->m_simulators)
    {
        names.push_back(simulator.first);
    }
    return names;
}

Simulator_base* Simulators::invoke_active_simulator() const
{
    if (this->m_simulators.empty())
    {
        return nullptr;
    }
    if (this->m_current_simulator.empty())
    {
        return nullptr;
    }
    
    return this->m_simulators.at(m_current_simulator).get();
}