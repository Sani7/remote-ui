#pragma once

#include "simulators.hpp"

enum class Command
{
    get_UI_element,
    get_UI_elements,
    switch_simulator,
    get_active_simulator_name,
    get_simulators,
    end,
    begin = 0
};

enum class Event
{
    clicked,
    value_changed,
    text_changed,
    selected,
    ui_changed,
    end,
    begin = 0
};

std::string message_parser(std::string message);
json command_parser(json command);
void event_handler(json event);