#include "message_parser.hpp"
#include <magic_enum.hpp>
#include <iostream>

extern Simulators* g_simulators;

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
    json j;
    try
    {
        std::string type = command.at("type");
        Command cmd = magic_enum::enum_cast<Command>(type).value_or(Command::end);
        switch (cmd)
        {
            case Command::get_UI_element:
                spdlog::info("Command {}: {}", type, std::string(command.at("id")));
                j["response"]["type"] = type;
                j["response"]["UI_item"] = g_simulators->invoke_active_simulator()->get_UI_item(command.at("id"))->to_json();
                break;
            case Command::get_UI_elements:
                spdlog::info("Command {}", type);
                j["response"]["type"] = type;
                j["response"]["UI_items"] = g_simulators->invoke_active_simulator()->get_UI_items()["UI_items"];
                break;
            case Command::switch_simulator:
                spdlog::info("Command {} to {}", type, std::string(command.at("name")));
                g_simulators->switch_simulator(command.at("name"));
                j["response"]["type"] = type;
                break;
            case Command::get_active_simulator_name:
                spdlog::info("Command {}", type);
                j["response"]["type"] = type;
                j["response"]["name"] = g_simulators->active_simulator_name();
                break;
            case Command::get_simulators:
                spdlog::info("Command {}", type);
                j["response"]["type"] = type;
                j["response"]["simulators"] = g_simulators->list_simulators();
                break;
            default:
                break;
        }
    }
    catch(const json::exception& e)
    {
        spdlog::error(e.what());
    }

    return j;
}

void event_handler(json event)
{
    try
    {
        std::string type = event.at("type");
        Event e = magic_enum::enum_cast<Event>(type).value_or(Event::end);
        if (g_simulators->invoke_active_simulator() == nullptr)
        {
            spdlog::error("Event got called but there is no active simulator");
            return;
        }

        switch(e)
        {
            case Event::clicked:
            {
                spdlog::debug("Event {}: {}", type, std::string(event.at("id")));
                g_simulators->invoke_active_simulator()->get_UI_item(std::string(event.at("id")))->click();
                break;
            }
            case Event::value_changed:
            {
                spdlog::debug("Event {}: {} {}", type, std::string(event.at("id")), (double)(event["value"]));
                g_simulators->invoke_active_simulator()->get_UI_item(std::string(event.at("id")))->set_value((double)event.at("value"));
                break;
            }
            case Event::text_changed:
            {
                spdlog::debug("Event {}: {} {}", type, std::string(event.at("id")), std::string(event.at("text")));
                g_simulators->invoke_active_simulator()->get_UI_item(std::string(event.at("id")))->set_text(std::string(event.at("text")));
                break;
            }
            case Event::selected:
            {
                spdlog::debug("Event {}: {} {}", type, std::string(event.at("id")), std::string(event.at("selected")));
                g_simulators->invoke_active_simulator()->get_UI_item(std::string(event.at("id")))->set_selected(std::string(event.at("selected")));
                break;
            }
            default:
            {
                break;
            }
        }
    }
    catch(const json::exception& e)
    {
        spdlog::error(e.what());
    }
}