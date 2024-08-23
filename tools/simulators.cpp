#include "simulators.hpp"
#include "test_sim.hpp"

Simulators::Simulators()
{
    INSERT_SIMULATOR(Test_Sim);
}

std::string Simulators::active_simulator_name() const
{
    return this->m_current_simulator;
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
        return "{}"_json;
    }
    if (this->m_current_simulator.empty())
    {
        return "{}"_json;
    }

    json after = this->m_simulators.at(m_current_simulator)->get_UI_items();
    json changed;
    changed["event"]["type"] = "UI_changed";
    
    json diff = json::diff(m_before, after);

    if (diff.empty())
    {
        return "{}"_json;
    }

    // diff to ui items list
    // get index out of each item in diff /UI_items/0/** 
    // Then push the ui_item at that index to the changed json
    for (auto& item : diff)
    {
        std::string path = item["path"];
        std::string index_s = path.substr(10, path.find("/", 10) - 10);
        changed["event"]["UI_items"].push_back(after["UI_items"][std::stoul(index_s)]);
    }

    m_before = after;
    return changed;
}