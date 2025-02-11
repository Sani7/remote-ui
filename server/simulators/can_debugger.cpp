#include "can_debugger.hpp"

Can_Debugger::Can_Debugger(CAN_Wrapper *can_wrapper, QObject *parent)
    : Simulator_base("Can_Debugger", can_wrapper, std::chrono::milliseconds(100), parent), m_wrapper(can_wrapper),
      m_can_ui(Color::Black, Color::White)
{
    this->push_ui_item(&this->m_can_ui);

    connect(&m_can_ui, &UI_can::on_send, this, [=, this](QCanBusFrame frame) { m_wrapper->send_can_message(frame); });
}

void Can_Debugger::timer()
{
    // Timer function
}

void Can_Debugger::run_at_startup()
{
    connect(m_wrapper, &CAN_Wrapper::on_can_message, this,
            [=, this](QCanBusFrame frame) { this->m_can_ui.add_received_message(frame); });
    connect(m_wrapper, &CAN_Wrapper::on_can_send, this,
            [=, this](QCanBusFrame frame) { this->m_can_ui.add_send_message(frame); });
}

void Can_Debugger::run_at_stop()
{
    disconnect(m_wrapper, nullptr, nullptr, nullptr);
}
