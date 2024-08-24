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
    std::string type = command["type"];
    Command cmd = magic_enum::enum_cast<Command>(type).value_or(Command::end);
    json j;
    switch (cmd)
    {
        case Command::get_UI_element:
            spdlog::info("Command {}: {}", type, std::string(command["id"]));
            j["response"]["type"] = type;
            j["response"]["UI_item"] = g_simulators->invoke_active_simulator()->get_UI_item(command["id"])->to_json();
            break;
        case Command::get_UI_elements:
            spdlog::info("Command {}", type);
            j["response"]["type"] = type;
            j["response"]["UI_items"] = g_simulators->invoke_active_simulator()->get_UI_items()["UI_items"];
            break;
        case Command::switch_simulator:
            spdlog::info("Command {} to {}", type, std::string(command["name"]));
            g_simulators->switch_simulator(command["name"]);
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
    return j;
}

void event_handler(json event)
{
    std::string type = event["type"];
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
            spdlog::debug("Event {}: {}", type, std::string(event["id"]));
            g_simulators->invoke_active_simulator()->get_UI_item(std::string(event["id"]))->click();
            break;
        }
        case Event::value_changed:
        {
            spdlog::debug("Event {}: {} {}", type, std::string(event["id"]), (double)(event["value"]));
            g_simulators->invoke_active_simulator()->get_UI_item(std::string(event["id"]))->set_value((double)event["value"]);
            break;
        }
        case Event::text_changed:
        {
            spdlog::debug("Event {}: {} {}", type, std::string(event["id"]), std::string(event["text"]));
            g_simulators->invoke_active_simulator()->get_UI_item(std::string(event["id"]))->set_text(std::string(event["text"]));
            break;
        }
        case Event::selected:
        {
            spdlog::debug("Event {}: {} {}", type, std::string(event["id"]), std::string(event["selected"]));
            g_simulators->invoke_active_simulator()->get_UI_item(std::string(event["id"]))->set_selected(std::string(event["selected"]));
            break;
        }
        default:
        {
            break;
        }
    }
}