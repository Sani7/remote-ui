#include "simulator_base.hpp"

Simulator_base::Simulator_base(std::string name)
    : p_name(name), p_timer([this]{ timer(); })
{
    
}

std::string Simulator_base::name() const {
    // Get the name of the simulator
    return p_name;
}

bool Simulator_base::is_ui_id_unique(std::string id) const {
    // Check if the UI name is unique
    for (UI_item* item : p_UI_items) {
        if (item->id() == id) {
            return false;
        }
    }

    return true;
}

void Simulator_base::add_UI_item(UI_item* item) {
    // Add a UI item to the simulator
    if (is_ui_id_unique(item->id()) == false) {
        throw std::runtime_error("UI item id " + item->id() + " is not unique");
    }
    p_UI_items_map[item->id()] = item;
    p_UI_items.push_back(item);
}

json Simulator_base::get_UI_items() const {
    // Get the UI items
    json items;
    items["name"] = p_name;
    for (UI_item* item : p_UI_items) {
        items["UI_items"].push_back(item->to_json());
    }

    return items;
}

UI_item* Simulator_base::get_UI_item(std::string id) const {
    // Get a UI item by id
    return p_UI_items_map.at(id);
}

void Simulator_base::run() {
    // Start the timer
    p_timer.setInterval(std::chrono::milliseconds(100));
}

void Simulator_base::stop() {
    // Stop the simulator
    p_timer.stop();
}