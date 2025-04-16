#include "ui_can.hpp"

template <class T> T ByteArrayToStdArray(const QByteArray &byteArray)
{
    T stdArray;

    for (unsigned int i = 0; i < stdArray.size() && i < (unsigned int)byteArray.size(); i++)
    {
        stdArray[i] = (unsigned char)byteArray.at(i);
    }

    return stdArray;
}

UI_can::UI_can(Color fg_color, Color bg_color, QObject *parent)
    : UI_item(UI_CAN_TYPE, "", 12, fg_color, bg_color, parent)
{
}

UI_can::UI_can(const json &j, QObject *parent) : UI_item(UI_CAN_TYPE, parent)
{
    from_json(j);
}

void UI_can::add_send_message(QCanBusFrame msg)
{
    CanFrame frame(msg.frameId(), (uint8_t)msg.payload().size(),
                   ByteArrayToStdArray<std::array<uint8_t, 8>>(msg.payload()));
    m_can_send_messages.push_back(frame);
    emit ui_changed();
}

void UI_can::add_received_message(QCanBusFrame msg)
{
    CanFrame frame(msg.frameId(), (uint8_t)msg.payload().size(),
                   ByteArrayToStdArray<std::array<uint8_t, 8>>(msg.payload()));
    m_can_received_messages.push_back(frame);
    emit ui_changed();
}

void UI_can::clear_send_buffer()
{
    m_can_send_messages.clear();
    emit ui_changed();
}

void UI_can::clear_receive_buffer()
{
    m_can_received_messages.clear();
    emit ui_changed();
}

void UI_can::can_clear()
{
    clear_send_buffer();
    clear_receive_buffer();
}

void UI_can::from_json(const json &j)
{
    UI_item::from_json(j);
    m_can_send_messages.clear();
    m_can_received_messages.clear();
    for (const auto &msg : j.at("send_msgs"))
    {
        CanFrame can_msg(msg.at("id"), msg.at("dlc"), msg.at("payload"));
        m_can_send_messages.push_back(can_msg);
    }
    for (const auto &msg : j.at("rcvd_msgs"))
    {
        CanFrame can_msg(msg.at("id"), msg.at("dlc"), msg.at("payload"));
        m_can_received_messages.push_back(can_msg);
    }
}

json UI_can::to_json(size_t id) const
{
    json j = UI_item::to_json(id);
    json can_send_messages = json::array();
    json can_received_messages = json::array();
    for (const auto &msg : m_can_send_messages)
    {
        json can_msg;
        can_msg["id"] = (uint32_t)msg.m_SID;
        can_msg["dlc"] = msg.m_DLC;
        can_msg["payload"] = msg.m_payload;

        can_send_messages.push_back(can_msg);
    }
    j["send_msgs"] = can_send_messages;
    for (const auto &msg : m_can_received_messages)
    {
        json can_msg;
        can_msg["id"] = (uint32_t)msg.m_SID;
        can_msg["dlc"] = msg.m_DLC;
        can_msg["payload"] = msg.m_payload;

        can_received_messages.push_back(can_msg);
    }
    j["rcvd_msgs"] = can_received_messages;
    return j;
}

void UI_can::can_send(uint32_t id, uint8_t dlc, std::array<uint8_t, 8> payload)
{
    QCanBusFrame frame;
    frame.setFrameId(id);
    frame.setPayload(QByteArray::fromRawData((const char *)(payload.data()), dlc));

    emit on_send(frame);
}
