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
    if (m_before["UI_items"].empty())
    {
        changed = after;
        return changed;
    }

    for (const json& item : after["UI_items"])
    {
        for (const json& before_item : m_before["UI_items"])
        {
            if (before_item["id"] == item["id"])
            {
                if (before_item != item)
                {
                    changed["UI_items"].push_back(item);
                }
            }
        }
    }
    return changed;
}