#include "simulators.hpp"
#include <unordered_set>
#include "test_sim.hpp"
#include "can_debugger.hpp"

Simulators::Simulators()
{
    INSERT_SIMULATOR(Test_Sim);
    INSERT_SIMULATOR(Can_Debugger);
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

    if (this->m_simulators.find(name) == this->m_simulators.end())
    {
        return;
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
    std::lock_guard<std::mutex> lock(m_mutex);
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
    std::lock_guard<std::mutex> lock(m_mutex);
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
    // but the index can be the same for multiple items
    // Then push the ui_item at that index to the changed json
    std::unordered_set<size_t> addedIndices;

    for (auto& item : diff)
    {
        std::string path = item["path"];
        size_t index = std::stoul(path.substr(10, path.find("/", 10) - 10));
        if (addedIndices.find(index) != addedIndices.end())
            continue;
        
        changed["event"]["UI_items"].push_back(after["UI_items"][index]);
        addedIndices.insert(index);
    }

    m_before = after;
    return changed;
}