#include "message_parser.hpp"
#include <magic_enum.hpp>

extern Simulators g_simulators;

std::string message_parser(std::string message)
{
    json j = message;
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
    Command cmd = magic_enum::enum_cast<Command>(command["type"]).value_or(Command::end);

    switch (cmd)
    {
        case Command::get_UI_element:
        {
            std::string id = command["id"];
            return g_simulators.invoke_active_simulator()->get_UI_item(id)->to_json();
        }
        case Command::get_UI_elements:
        {
            return g_simulators.invoke_active_simulator()->get_UI_items();
        }
        case Command::switch_simulator:
        {
            std::string name = command["name"];
            g_simulators.switch_simulator(name);
            return "";
        }
        case Command::get_active_simulator_name:
        {
            json j;
            j["name"] = g_simulators.active_simulator_name();
            return j;
        }
        case Command::get_simulators:
        {
            json j;
            j["simulators"] = g_simulators.list_simulators();
            return j;
        }
        default:
        {
            return "";
        }
    }
}

void event_handler(json event)
{
    Event e = magic_enum::enum_cast<Event>(event["type"]).value_or(Event::end);

    switch(e)
    {
        case Event::clicked:
        {
            std::string id = event["id"];
            g_simulators.invoke_active_simulator()->get_UI_item(id)->click();
            break;
        }
        case Event::value_changed:
        {
            std::string id = event["id"];
            int value = event["value"];
            g_simulators.invoke_active_simulator()->get_UI_item(id)->set_value(value);
            break;
        }
        case Event::text_changed:
        {
            std::string id = event["id"];
            std::string text = event["text"];
            g_simulators.invoke_active_simulator()->get_UI_item(id)->set_text(text);
            break;
        }
        case Event::selected:
        {
            std::string id = event["id"];
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