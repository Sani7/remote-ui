#include "UI_can.hpp"

template <class T> T ByteArrayToStdArray(const QByteArray &byteArray)
{
    T stdArray;

    // qDebug() << "std::array size" <<stdArray.size();

    for (unsigned int i = 0; i < stdArray.size() && i < (unsigned int)byteArray.size(); i++)
    {
        stdArray[i] = (unsigned char)byteArray.at(i);
    }

    return stdArray;
}

UI_can::UI_can(std::string text, Color fg_color, Color bg_color)
    : UI_item(UI_CAN_TYPE, text, 12, fg_color, bg_color)
{
}

UI_can::UI_can(const json &j) : UI_item(UI_CAN_TYPE)
{
    from_json(j);
}

void UI_can::add_can_message(QCanBusFrame& msg)
{
    m_can_messages.push_back(msg);
}

void UI_can::from_json(const json &j)
{
    UI_item::from_json(j);
    m_can_messages.clear();
    for (const auto &msg : j.at("can_messages"))
    {
        QCanBusFrame can_msg;
        can_msg.setFrameId(msg.at("id"));
        can_msg.setPayload(QByteArray(msg.at("bytes"), msg.at("dlc")));
        m_can_messages.push_back(can_msg);
    }
}

json UI_can::to_json(size_t id) const
{
    json j = UI_item::to_json(id);
    json can_messages = json::array();
    for (const auto &msg : m_can_messages)
    {
        json can_msg;
        can_msg["id"] = (uint32_t)msg.frameId();
        can_msg["dlc"] = msg.payload().size();
        can_msg["bytes"] = ByteArrayToStdArray<std::array<uint8_t, 8>>(msg.payload());

        can_messages.push_back(can_msg);
    }
    j["can_messages"] = can_messages;
    return j;
}

void UI_can::can_send(uint32_t id, uint8_t dlc, std::array<uint8_t, 8> payload)
{
    QCanBusFrame frame;
    frame.setFrameId(id);
    frame.setPayload(QByteArray::fromRawData((const char *)(payload.data()), dlc));

    emit on_send(frame);
}