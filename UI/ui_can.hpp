#pragma once
#include "ui_item.hpp"
#include <QCanBusFrame>

#define UI_CAN_TYPE std::string("ui_can")

class CanFrame
{
  public:
    CanFrame(uint32_t sid, uint8_t dlc, std::array<uint8_t, 8> payload) : m_SID(sid), m_DLC(dlc), m_payload(payload)
    {
    }
    uint32_t m_SID;
    uint8_t m_DLC;
    std::array<uint8_t, 8> m_payload;
};

class UI_can : public UI_item
{
    Q_OBJECT
  public:
    UI_can(Color fg_color, Color bg_color, QObject *parent = nullptr);
    UI_can(const json &j, QObject *parent = nullptr);

    void add_send_message(QCanBusFrame msg);
    void add_received_message(QCanBusFrame msg);
    void clear_send_buffer();
    void clear_receive_buffer();

    void from_json(const json &j) override;
    json to_json(size_t id) const override;

    void can_send(uint32_t id, uint8_t dlc, std::array<uint8_t, 8> payload) override;
    void can_clear() override;
  signals:
    void on_send(QCanBusFrame msg);

  private:
    std::vector<CanFrame> m_can_send_messages;
    std::vector<CanFrame> m_can_received_messages;
};
