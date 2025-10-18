#include "ui_status_bar.hpp"

UI_status_bar::UI_status_bar(QObject *parent) : UI_item(UI_STATUS_BAR_TYPE, parent), m_timeout(0)
{
    setup_item(true, false, false);
}

UI_status_bar::UI_status_bar(std::string message, QObject *parent) : UI_item(UI_STATUS_BAR_TYPE, parent), m_timeout(0)
{
    m_text = message;
    setup_item(true, false, false);
}

void UI_status_bar::show_message(std::string message, size_t timeout)
{
    // This should work because the message gets only send once when it changes
    m_text = message;
    m_timeout = timeout;
    emit ui_changed();
}

std::string UI_status_bar::message() const
{
    return m_text;
}

size_t UI_status_bar::timeout() const
{
    return m_timeout;
}

void UI_status_bar::from_json(const json &j)
{
    UI_item::from_json(j);
    m_timeout = j.at("timeout");
}

json UI_status_bar::to_json(size_t id) const
{
    json j = UI_item::to_json(id);
    j["timeout"] = m_timeout;
    return j;
}
