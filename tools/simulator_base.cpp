#include "simulator_base.hpp"

SimulatorBase::SimulatorBase(std::string name)
    : p_name(name), p_timer([this]{ timer(); })
{
    
}

void SimulatorBase::add_UI_item(UI_item* item) {
    // Add a UI item to the simulator
    p_UI_items_map[item->id()] = item;
    p_UI_items.push_back(item);
}

json SimulatorBase::get_UI_items() const {
    // Get the UI items
    json items;
    items["name"] = p_name;
    for (UI_item* item : p_UI_items) {
        items["UI_items"].push_back(item->to_json());
    }

    return items;
}

UI_item* SimulatorBase::get_UI_item(std::string id) const {
    // Get a UI item by id
    return p_UI_items_map.at(id);
}

void SimulatorBase::run() {
    // Run the simulator
    p_timer.setInterval(std::chrono::milliseconds(100));
}