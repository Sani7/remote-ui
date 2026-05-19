#define INTERNAL
#include "simulator_base.hpp"
#include "ui_item.hpp"

Simulator_base::Simulator_base(QString name, Communication *com, std::chrono::milliseconds interval,
                               QObject *parent)
    : QObject(parent), m_name(name), m_timer(new QTimer(this)), m_interval(interval), m_com(com)
{
    connect(m_timer, &QTimer::timeout, this, [this] { timer(); });
    m_UI_items.reserve(50);
}

QString Simulator_base::name() const
{
    // Get the name of the simulator
    return m_name;
}

void Simulator_base::push_ui_item(UI_item *item)
{
    // Add a UI item to the simulator
    connect(item, &UI_item::ui_changed, this, [this] { emit sim_changed(); });
    item->set_id(m_UI_items.size());
    m_UI_items.push_back(item);
}

QList<UI_item_m> Simulator_base::get_UI_items() const
{
    // Get the UI items
    QList<UI_item_m> items;
    for (size_t i = 0; i < m_UI_items.size(); i++)
    {
        items.push_back(m_UI_items[i]->to_proto());
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

void Simulator_base::start()
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
