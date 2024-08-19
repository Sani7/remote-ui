#include "simulators.hpp"
#include "test_sim.hpp"

Simulators::Simulators()
{
    
}

void Simulators::run()
{
    if (this->p_simulators.empty())
    {
        return;
    }
    if (this->p_current_simulator.empty())
    {
        return;
    }

    this->p_simulators.at(p_current_simulator)->run();
}

void Simulators::stop()
{
    if (this->p_simulators.empty())
    {
        return;
    }
    if (this->p_current_simulator.empty())
    {
        return;
    }

    this->p_simulators.at(p_current_simulator)->stop();
}

void Simulators::switch_simulator(std::string name)
{
    if (this->p_simulators.empty())
    {
        return;
    }
    if (!this->p_current_simulator.empty())
    {
        this->p_simulators.at(p_current_simulator)->stop();
    }

    this->p_current_simulator = name;
    this->p_simulators.at(p_current_simulator)->run();
}

Simulator_base* Simulators::invoke_active_simulator() const
{
    if (this->p_simulators.empty())
    {
        return nullptr;
    }
    if (this->p_current_simulator.empty())
    {
        return nullptr;
    }

    return this->p_simulators.at(p_current_simulator).get();
}