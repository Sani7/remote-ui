#include "simulator_base.hpp"

Simulator_base::Simulator_base(std::string name, CAN_Wrapper *can_wrapper, QSerialPort *serial,
                               std::chrono::milliseconds interval, QObject *parent)
    : QObject(parent), m_name(name), m_timer(new QTimer(this)), m_interval(interval), m_can_wrapper(can_wrapper),
      m_serial(serial)
{
    connect(m_timer, &QTimer::timeout, this, [this] { timer(); });
    m_UI_items.reserve(50);
}

std::string Simulator_base::name() const
{
    // Get the name of the simulator
    return m_name;
}

void Simulator_base::push_ui_item(UI_item *item)
{
    // Add a UI item to the simulator
    connect(item, &UI_item::ui_changed, this, [this] { emit sim_changed(); });
    m_UI_items.push_back(item);
}

json Simulator_base::get_UI_items() const
{
    // Get the UI items
    json items;
    items["name"] = m_name;
    for (size_t i = 0; i < m_UI_items.size(); i++)
    {
        items["UI_items"].push_back(m_UI_items[i]->to_json(i));
    }

    return items;
}

UI_item *Simulator_base::get_UI_item(size_t id) const
{
    // Get a UI item by id using the list
    if (id < m_UI_items.size())
    {
        return m_UI_items[id];
    }
    return nullptr;
}

void Simulator_base::run()
{
    // Start the timer
    m_timer->start(m_interval);
    run_at_startup();
}

void Simulator_base::stop()
{
    // Stop the simulator
    run_at_stop();
    m_timer->stop();
}

void Simulator_base::shutdown()
{
    // Shutdown the simulator
    system("shutdown -h now");
}

void Simulator_base::reboot()
{
    // Reboot the simulator
    system("reboot");
}

bool Simulator_base::check_input_high(uint32_t data, uint16_t pinnummer)
{
    return (data & (1 << pinnummer));
}
