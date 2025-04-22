#include "can_debugger.hpp"

EXPORT_SIM(Can_Debugger);

Can_Debugger::Can_Debugger(Communication *com, QObject *parent)
    : Simulator_base("Can Debugger", com, std::chrono::milliseconds(100), parent), m_wrapper(com->c_if1),
      m_can_ui(Color::Black, Color::White)
{
    PUSH_UI_ITEM(m_can_ui);

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
