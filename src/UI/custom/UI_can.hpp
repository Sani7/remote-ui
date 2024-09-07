#pragma once

#include "UI_item.hpp"
#include <ctime>
#include <chrono>
#include "CAN_interface.hpp"

#define UI_CAN_TYPE std::string("UI_can")

class UI_can : public UI_item
{
  public:
    UI_can(std::string id, std::string text, Color fg_color, Color bg_color,
              std::function<void(can_message_t msg)> on_send = nullptr);
    UI_can(const json &j);

    void add_can_message(can_message_t msg);

    void from_json(const json &j) override;
    json to_json() const override;

    void can_send(can_message_t msg) override;

  private:
    std::function<void(can_message_t msg)> m_on_can_send = nullptr;
    std::vector<can_message_t> m_can_messages;
};