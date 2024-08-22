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
            json j;
            j["response"] = "get_UI_element";
            j["UI_item"] = g_simulators.invoke_active_simulator()->get_UI_item(id)->to_json();
            return j;
        }
        case Command::get_UI_elements:
        {
            json j;
            j["response"] = "get_UI_elements";
            j["UI_items"] = g_simulators.invoke_active_simulator()->get_UI_items()["UI_items"];
            return j;
        }
        case Command::switch_simulator:
        {
            g_simulators.switch_simulator(command["name"]);
            json j;
            j["response"] = "switch_simulator";
            return j;
        }
        case Command::get_active_simulator_name:
        {
            json j;
            j["response"] = "get_active_simulator_name";
            j["name"] = g_simulators.active_simulator_name();
            return j;
        }
        case Command::get_simulators:
        {
            json j;
            j["response"] = "get_simulators";
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