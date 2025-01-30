#pragma once

#include "UI_item.hpp"
#include <QCanBusFrame>

#define UI_CAN_TYPE std::string("UI_can")

class UI_can : public UI_item
{
    Q_OBJECT
  public:
    UI_can(Color fg_color, Color bg_color, QObject *parent = nullptr);
    UI_can(const json &j, QObject *parent = nullptr);

    void add_send_message(QCanBusFrame& msg);
    void add_received_message(QCanBusFrame& msg);
    void clear_send_buffer();
    void clear_receive_buffer();

    void from_json(const json &j) override;
    json to_json(size_t id) const override;

    void can_send(uint32_t id, uint8_t dlc, std::array<uint8_t, 8> payload) override;
  signals:
    void on_send(QCanBusFrame msg);
  private:
    std::vector<QCanBusFrame> m_can_send_messages;
    std::vector<QCanBusFrame> m_can_received_messages;
};
