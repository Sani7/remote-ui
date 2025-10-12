#include "ui_status_bar.hpp"

UI_status_bar::UI_status_bar(QObject *parent) : UI_item{UI_STATUS_BAR_TYPE, parent}
{
}

UI_status_bar::UI_status_bar(std::string message, QObject *parent)
    : UI_item(UI_STATUS_BAR_TYPE, parent), m_current_message(message)
{
}

void UI_status_bar::show_message(std::string message, size_t timeout)
{
    m_current_message = message;
    m_timeout = timeout;
    emit ui_changed();
}

std::string UI_status_bar::message() const
{
    return m_current_message;
}

void UI_status_bar::from_json(const json &j)
{
    UI_item::from_json(j);
    m_current_message = j.at("current_message");
    m_timeout = j.at("timeout");
}

json UI_status_bar::to_json(size_t id) const
{
    json j = UI_item::to_json(id);
    j["current_message"] = m_current_message;
    j["timeout"] = m_timeout;
    return j;
}
