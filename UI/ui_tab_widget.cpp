#include "ui_tab_widget.hpp"

UI_tab_widget::UI_tab_widget(QObject *parent) : UI_item(UI_TAB_WIDGET_TYPE, parent)
{
}

UI_tab_widget::UI_tab_widget(std::vector<std::string> tab_names, size_t selected, QObject *parent)
    : UI_item(UI_TAB_WIDGET_TYPE, parent), m_selected_tab(selected), m_tab_names(tab_names)
{
}

void UI_tab_widget::set_selected(size_t selected)
{
    m_selected_tab = selected;
    emit ui_changed();
    emit this->selected(m_selected_tab);
}
size_t UI_tab_widget::selected() const
{
    return m_selected_tab;
}

void UI_tab_widget::set_visible(size_t index, bool visible)
{
    if (index >= m_tab_visible.size())
    {
        m_tab_visible.resize(index + 1);
    }
    m_tab_visible.at(index) = visible;
    emit ui_changed();
}
bool UI_tab_widget::visible(size_t index) const
{
    return m_tab_visible.at(index);
}

void UI_tab_widget::set_tab_name(size_t index, std::string name)
{
    if (index >= m_tab_names.size())
    {
        m_tab_names.resize(index + 1);
    }
    m_tab_names.at(index) = name;
    emit ui_changed();
}
std::string UI_tab_widget::tab_name(size_t index) const
{
    return m_tab_names.at(index);
}

void UI_tab_widget::from_json(const json &j)
{
    UI_item::from_json(j);
    if (j.contains("tab_names"))
    {
        this->m_tab_names.clear();
        for (auto option : j["tab_names"])
        {
            this->m_tab_names.push_back(option);
        }
    }
    if (j.contains("tab_visible"))
    {
        this->m_tab_visible.clear();
        for (auto option : j["tab_visible"])
        {
            this->m_tab_visible.push_back(option);
        }
    }
    m_selected_tab = j.at("selected");
}
json UI_tab_widget::to_json(size_t id) const
{
    json j = UI_item::to_json(id);
    j["tab_names"] = m_tab_names;
    j["tab_visible"] = m_tab_visible;
    j["selected"] = m_selected_tab;
    return j;
}
