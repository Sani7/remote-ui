#include "simulators.hpp"
#include "test_sim.hpp"

Simulators::Simulators()
{
    INSERT_SIMULATOR(Test_Sim);
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

    m_before = this->m_simulators.at(m_current_simulator)->get_UI_items();
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
    this->run();
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

Simulator_base* Simulators::invoke_active_simulator()
{
    if (this->m_simulators.empty())
    {
        return nullptr;
    }
    if (this->m_current_simulator.empty())
    {
        return nullptr;
    }
    
    m_before = this->m_simulators.at(m_current_simulator)->get_UI_items();

    return this->m_simulators.at(m_current_simulator).get();
}

json Simulators::changed_UI_items()
{
    if (this->m_simulators.empty())
    {
        return json();
    }
    if (this->m_current_simulator.empty())
    {
        return json();
    }

    json after = this->m_simulators.at(m_current_simulator)->get_UI_items();
    json changed;
    changed["event"] = "UI_changed";
    
    changed["diff"] = json::diff(m_before, after);

    if (changed["diff"].empty())
    {
        return "{}"_json;
    }

    m_before = after;
    return changed;
}