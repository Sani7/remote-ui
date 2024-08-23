#include "message_parser.hpp"
#include <magic_enum.hpp>
#include <iostream>

extern Simulators g_simulators;

std::string message_parser(std::string message)
{
    json j = json::parse(message);
    if (j.contains("command"))
    {
        return command_parser(j["command"]).dump();
    }
    else if (j.contains("event"))
    {
        event_handler(j["event"]);
        return "";
    }
    else
    {
        return "";
    }
}

json command_parser(json command)
{
    std::string type = command["type"];
    Command cmd = magic_enum::enum_cast<Command>(type).value_or(Command::end);
    json j;
    switch (cmd)
    {
        case Command::get_UI_element:
            std::cout << type << ": " << command["id"] << std::endl;
            j["response"]["type"] = type;
            j["response"]["UI_item"] = g_simulators.invoke_active_simulator()->get_UI_item(command["id"])->to_json();
            break;
        case Command::get_UI_elements:
            std::cout << type << std::endl;
            j["response"]["type"] = type;
            j["response"]["UI_items"] = g_simulators.invoke_active_simulator()->get_UI_items()["UI_items"];
            break;
        case Command::switch_simulator:
            std::cout << type << " to " << command["name"] << std::endl;
            g_simulators.switch_simulator(command["name"]);
            j["response"]["type"] = type;
            break;
        case Command::get_active_simulator_name:
            std::cout << type << std::endl;
            j["response"]["type"] = type;
            j["response"]["name"] = g_simulators.active_simulator_name();
            break;
        case Command::get_simulators:
            std::cout << type << std::endl;
            j["response"]["type"] = type;
            j["response"]["simulators"] = g_simulators.list_simulators();
            break;
        default:
            break;
    }
    return j;
}

void event_handler(json event)
{
    std::string type = event["type"];
    Event e = magic_enum::enum_cast<Event>(type).value_or(Event::end);
    std::string id;

    switch(e)
    {
        case Event::clicked:
        {
            id = event["id"];
            g_simulators.invoke_active_simulator()->get_UI_item(id)->click();
            break;
        }
        case Event::value_changed:
        {
            id = event["id"];
            double value = event["value"];
            g_simulators.invoke_active_simulator()->get_UI_item(id)->set_value(value);
            break;
        }
        case Event::text_changed:
        {
            id = event["id"];
            std::string text = event["text"];
            g_simulators.invoke_active_simulator()->get_UI_item(id)->set_text(text);
            break;
        }
        case Event::selected:
        {
            id = event["id"];
            std::string selected = event["selected"];
            g_simulators.invoke_active_simulator()->get_UI_item(id)->set_selected(selected);
            break;
        }
        default:
        {
            break;
        }
    }
}