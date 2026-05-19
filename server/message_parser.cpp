#include "simulator_base.hpp"
#include "simulators.hpp"
#include <magic_enum/magic_enum.hpp>
#include <spdlog/spdlog.h>

// TODO: Fix parsing messages
QByteArray Simulators::message_parser(QByteArray message)
{
    Event_m event_m;
    bool event = m_serializer.deserialize(&event_m, message);
    // if (j.contains("command"))
    // {
    //     return QString::fromStdString(command_parser(j["command"]).dump());
    // }
    /*else*/ if (event)
    {
        event_handler(event_m);
    }
    return QByteArray();
}

// QByteArray Simulators::command_parser(json command)
// {
//     json j = "{}"_json;
//     try
//     {
//         std::string type = command.at("type");
//         Command cmd = magic_enum::enum_cast<Command>(type).value_or(Command::end);
//         switch (cmd)
//         {
//         case Command::get_UI_element:
//             SPDLOG_INFO("Command {}: {}", type, (size_t)command.at("id"));
//             if (is_not_active())
//             {
//                 SPDLOG_ERROR("Command got called but there is no active simulator");
//                 return j;
//             }
//             j["response"]["type"] = type;
//             j["response"]["UI_item"] =
//                 invoke_active_simulator()->get_UI_item(command.at("id"))->to_json(command.at("id"));
//             break;
//         case Command::get_UI_elements:
//             SPDLOG_INFO("Command {}", type);
//             if (is_not_active())
//             {
//                 SPDLOG_ERROR("Command got called but there is no active simulator");
//                 return j;
//             }
//             j["response"]["type"] = type;
//             j["response"]["UI_items"] = invoke_active_simulator()->get_UI_items()["UI_items"];
//             break;
//         case Command::switch_simulator:
//             SPDLOG_INFO("Command {} to {}", type, std::string(command.at("name")));
//             switch_simulator(command.at("name"));
//             j["response"]["type"] = type;
//             break;
//         case Command::get_active_simulator_name:
//             SPDLOG_INFO("Command {}", type);
//             j["response"]["type"] = type;
//             j["response"]["name"] = active_simulator_name();
//             break;
//         case Command::get_simulators:
//             SPDLOG_INFO("Command {}", type);
//             j["response"]["type"] = type;
//             j["response"]["simulators"] = list_simulators();
//             break;
//         default:
//             break;
//         }
//     }
//     catch (const json::exception &e)
//     {
//         SPDLOG_ERROR("{} {}", __FUNCTION__, e.what());
//     }

//     return j;
// }

void Simulators::event_handler(Event_m event)
{
    if (invoke_active_simulator() == nullptr)
    {
        SPDLOG_ERROR("Event got called but there is no active simulator");
        return;
    }

    QString type; // = QString::fromStdString(magic_enum::enum_cast<Event_m::KindFields>(event.kindField()).value_or(Event_m::KindFields::UninitializedField));

    switch (event.kindField())
    {
    case Event_m::KindFields::Clicked: {
        SPDLOG_DEBUG("Event {}: {}", type, (size_t)event.id_proto());
        invoke_active_simulator()->get_UI_item(event.id_proto())->click();
        break;
    }
    case Event_m::KindFields::ValueChanged: {
        SPDLOG_DEBUG("Event {}: {} {}", type, event.id_proto(), event.valueChanged().value());
        invoke_active_simulator()->get_UI_item(event.id_proto())->set_value(event.valueChanged().value());
        break;
    }
    case Event_m::KindFields::TextChanged: {
        SPDLOG_DEBUG("Event {}: {} {}", type, event.id_proto(), event.textChanged().text());
        invoke_active_simulator()->get_UI_item(event.id_proto())->set_text(event.textChanged().text());
        break;
    }
    case Event_m::KindFields::Selected: {
        SPDLOG_DEBUG("Event {}: {} {}", type, event.id_proto(), std::to_string(event.selected().selected()));
        invoke_active_simulator()->get_UI_item(event.id_proto())->set_selected(event.selected().selected());
        break;
    }
    case Event_m::KindFields::CanSend: {
        SPDLOG_DEBUG("Event {}: {}", type, event.id_proto());
        invoke_active_simulator()
            ->get_UI_item(event.id_proto())
            ->can_send(event.canSend().frame().sid(), event.canSend().frame().dlc(),
                       event.canSend().frame().payload());
        break;
    }
    case Event_m::KindFields::Clear: {
        SPDLOG_DEBUG("Event {}: {}", type, event.id_proto());
        invoke_active_simulator()->get_UI_item(event.id_proto())->clear();
        break;
    }
    default: {
        break;
    }
    }
}
