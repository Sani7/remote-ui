#include "simulator_base.hpp"

Simulator_base::Simulator_base(std::string name, std::chrono::milliseconds interval, QObject* parrent)
    : QObject(parrent), m_name(name), m_timer(new QTimer(this)), m_interval(interval)
{
    connect(m_timer, &QTimer::timeout, this, [this]{timer();});
}

std::string Simulator_base::name() const
{
    // Get the name of the simulator
    return m_name;
}

bool Simulator_base::is_ui_id_unique(std::string id) const
{
    // Check if the UI name is unique
    for (UI_item *item : m_UI_items)
    {
        if (item->id() == id)
        {
            return false;
        }
    }

    return true;
}

void Simulator_base::add_UI_item(UI_item *item)
{
    // Add a UI item to the simulator
    if (is_ui_id_unique(item->id()) == false)
    {
        throw std::runtime_error("UI item id " + item->id() + " is not unique");
    }
    connect(item, &UI_item::value_changed, this, [this]{emit sim_changed();});
    m_UI_items_map[item->id()] = item;
    m_UI_items.push_back(item);
}

json Simulator_base::get_UI_items() const
{
    // Get the UI items
    json items;
    items["name"] = m_name;
    for (UI_item *item : m_UI_items)
    {
        items["UI_items"].push_back(item->to_json());
    }

    return items;
}

UI_item *Simulator_base::get_UI_item(std::string id) const
{
    // Get a UI item by id
    return m_UI_items_map.at(id);
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
