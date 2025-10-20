#include "simulator_base.hpp"
#include "simulators.hpp"
#include <magic_enum/magic_enum.hpp>
#include <spdlog/spdlog.h>

QString Simulators::message_parser(QString message)
{
    json j = json::parse(message.toStdString());
    if (j.contains("command"))
    {
        return QString::fromStdString(command_parser(j["command"]).dump());
    }
    else if (j.contains("event"))
    {
        event_handler(j["event"]);
    }
    return "{}";
}

json Simulators::command_parser(json command)
{
    json j = "{}"_json;
    try
    {
        std::string type = command.at("type");
        Command cmd = magic_enum::enum_cast<Command>(type).value_or(Command::end);
        switch (cmd)
        {
        case Command::get_UI_element:
            SPDLOG_INFO("Command {}: {}", type, (size_t)command.at("id"));
            if (is_not_active())
            {
                SPDLOG_ERROR("Command got called but there is no active simulator");
                return j;
            }
            j["response"]["type"] = type;
            j["response"]["UI_item"] =
                invoke_active_simulator()->get_UI_item(command.at("id"))->to_json(command.at("id"));
            break;
        case Command::get_UI_elements:
            SPDLOG_INFO("Command {}", type);
            if (is_not_active())
            {
                SPDLOG_ERROR("Command got called but there is no active simulator");
                return j;
            }
            j["response"]["type"] = type;
            j["response"]["UI_items"] = invoke_active_simulator()->get_UI_items()["UI_items"];
            break;
        case Command::switch_simulator:
            SPDLOG_INFO("Command {} to {}", type, std::string(command.at("name")));
            switch_simulator(command.at("name"));
            j["response"]["type"] = type;
            break;
        case Command::get_active_simulator_name:
            SPDLOG_INFO("Command {}", type);
            j["response"]["type"] = type;
            j["response"]["name"] = active_simulator_name();
            break;
        case Command::get_simulators:
            SPDLOG_INFO("Command {}", type);
            j["response"]["type"] = type;
            j["response"]["simulators"] = list_simulators();
            break;
        default:
            break;
        }
    }
    catch (const json::exception &e)
    {
        SPDLOG_ERROR("{} {}", __FUNCTION__, e.what());
    }

    return j;
}

void Simulators::event_handler(json event)
{
    try
    {
        std::string type = event.at("type");
        Event e = magic_enum::enum_cast<Event>(type).value_or(Event::end);
        if (invoke_active_simulator() == nullptr)
        {
            SPDLOG_ERROR("Event got called but there is no active simulator");
            return;
        }

        switch (e)
        {
        case Event::clicked: {
            SPDLOG_DEBUG("Event {}: {}", type, (size_t)event.at("id"));
            invoke_active_simulator()->get_UI_item(event.at("id"))->click();
            break;
        }
        case Event::value_changed: {
            SPDLOG_DEBUG("Event {}: {} {}", type, (size_t)event.at("id"), (double)(event["value"]));
            invoke_active_simulator()->get_UI_item(event.at("id"))->set_value((double)event.at("value"));
            break;
        }
        case Event::text_changed: {
            SPDLOG_DEBUG("Event {}: {} {}", type, (size_t)event.at("id"), std::string(event.at("text")));
            invoke_active_simulator()->get_UI_item(event.at("id"))->set_text(std::string(event.at("text")));
            break;
        }
        case Event::selected: {
            SPDLOG_DEBUG("Event {}: {} {}", type, (size_t)event.at("id"), std::string(event.at("selected")));
            invoke_active_simulator()->get_UI_item(event.at("id"))->set_selected(event.at("selected"));
            break;
        }
        case Event::can_send: {
            SPDLOG_DEBUG("Event {}: {}", type, (size_t)event.at("id"));
            invoke_active_simulator()
                ->get_UI_item(event.at("id"))
                ->can_send((uint32_t)event.at("sid"), (uint8_t)event.at("dlc"),
                           (std::array<uint8_t, 8>)event.at("payload"));
            break;
        }
        case Event::clear: {
            SPDLOG_DEBUG("Event {}: {}", type, (size_t)event.at("id"));
            invoke_active_simulator()->get_UI_item(event.at("id"))->clear();
            break;
        }
        default: {
            break;
        }
        }
    }
    catch (const json::exception &e)
    {
        SPDLOG_ERROR("{} {}", __FUNCTION__, e.what());
    }
}
