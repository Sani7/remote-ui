#include "UI_can.hpp"

UI_can::UI_can(std::string id, std::string text, Color fg_color, Color bg_color,
               std::function<void(can_message_t msg)> on_send)
    : UI_item(id, UI_CAN_TYPE, text, 12, fg_color, bg_color), m_on_can_send(on_send)
{
}

UI_can::UI_can(const json &j) : UI_item(UI_CAN_TYPE)
{
    from_json(j);
}

void UI_can::add_can_message(can_message_t msg)
{
    m_can_messages.push_back(msg);
}

void UI_can::from_json(const json &j)
{
    UI_item::from_json(j);
    m_can_messages.clear();
    for (const auto &msg : j.at("can_messages"))
    {
        can_message_t can_msg;
        can_msg.timestamp = msg.at("timestamp").get<time_t>();
        can_msg.id = msg.at("id").get<uint16_t>();
        can_msg.dlc = msg.at("dlc").get<uint8_t>();
        for (size_t i = 0; i < can_msg.dlc; i++)
        {
            can_msg.data[i] = msg.at("data").at(i).get<uint8_t>();
        }
        m_can_messages.push_back(can_msg);
    }
}

json UI_can::to_json() const
{
    json j = UI_item::to_json();
    json can_messages = json::array();
    for (const auto &msg : m_can_messages)
    {
        json can_msg;
        can_msg["timestamp"] = msg.timestamp;
        can_msg["id"] = msg.id;
        can_msg["dlc"] = msg.dlc;
        json data = json::array();
        for (int i = 0; i < msg.dlc; i++)
        {
            data.push_back(msg.data[i]);
        }
        can_msg["data"] = data;
        can_messages.push_back(can_msg);
    }
    j["can_messages"] = can_messages;
    return j;
}

void UI_can::can_send(can_message_t msg)
{
    if (m_on_can_send)
    {
        m_on_can_send(msg);
    }
}