#include "can_debugger.hpp"
#include "extensions/CAN_interface.hpp"

EXPORT_SIM(Can_Debugger);

Can_Debugger::Can_Debugger(Communication *com, QObject *parent)
    : Simulator_base("Can Debugger", com, std::chrono::milliseconds(100), parent), m_if(com->can_if[0].get()),
      m_can_ui(Color::Black, Color::White)
{
    PUSH_UI_ITEM(m_can_ui);

    connect(&m_can_ui, &UI_can::send, this, [=, this](QCanBusFrame frame) { m_if->send_frame(frame); });
}

void Can_Debugger::timer()
{
    // Timer function
}

void Can_Debugger::run_at_startup()
{
    if (!m_if)
        return;
    connect(m_if, &CAN_Interface::frame_received, this,
            [=, this](QCanBusFrame frame) { this->m_can_ui.add_received_message(frame); });
    connect(m_if, &CAN_Interface::frame_send, this,
            [=, this](QCanBusFrame frame) { this->m_can_ui.add_send_message(frame); });
}

void Can_Debugger::run_at_stop()
{
    if (!m_if)
        return;
    disconnect(m_if, nullptr, nullptr, nullptr);
}
