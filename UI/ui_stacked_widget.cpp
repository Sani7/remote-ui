#include "ui_stacked_widget.hpp"

UI_stacked_widget::UI_stacked_widget(QObject *parent) : UI_item(UI_STACKED_WIDGET_TYPE, parent)
{
}

UI_stacked_widget::UI_stacked_widget(size_t tab_count, size_t current_tab, QObject *parent)
    : UI_item(UI_STACKED_WIDGET_TYPE, parent), m_tab_count(tab_count), m_current_tab(current_tab)
{
}

void UI_stacked_widget::set_selected(size_t selected)
{
    if (selected >= m_tab_count)
        return;
    m_current_tab = selected;
    emit ui_changed();
}
size_t UI_stacked_widget::selected() const
{
    return m_current_tab;
}

void UI_stacked_widget::advance_tab()
{
    if (m_tab_count == 0)
        return;
    m_current_tab = (m_current_tab + 1) % m_tab_count;
    emit ui_changed();
}

void UI_stacked_widget::from_json(const json &j)
{
    UI_item::from_json(j);
    m_tab_count = j.at("tab_count");
    m_current_tab = j.at("current_tab");
}
json UI_stacked_widget::to_json(size_t id) const
{
    json j = UI_item::to_json(id);
    j["tab_count"] = m_tab_count;
    j["current_tab"] = m_current_tab;
    return j;
}
