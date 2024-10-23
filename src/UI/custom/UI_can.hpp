#pragma once

#include "UI_item.hpp"
#include <QCanBusFrame>
#include <chrono>
#include <ctime>

#define UI_CAN_TYPE std::string("UI_can")

class UI_can : public UI_item
{
  public:
    UI_can(std::string text, Color fg_color, Color bg_color);
    UI_can(const json &j);

    void add_can_message(QCanBusFrame& msg);

    void from_json(const json &j) override;
    json to_json(size_t id) const override;

    void can_send(uint32_t id, uint8_t dlc, std::array<uint8_t, 8> payload) override;
  signals:
    void on_send(QCanBusFrame msg);
  private:
    std::vector<QCanBusFrame> m_can_messages;
};
