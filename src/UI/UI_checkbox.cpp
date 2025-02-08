#include "UI_checkbox.hpp"

UI_checkbox::UI_checkbox(QObject *parent) : UI_item(UI_CHECKBOX_TYPE, parent), m_selected()
{
}

UI_checkbox::UI_checkbox(std::string text, Color fg_color, Color bg_color, QObject *parent)
    : UI_item(UI_CHECKBOX_TYPE, text, 0, fg_color, bg_color, parent), m_selected(false)
{
}

UI_checkbox::UI_checkbox(const json &j, QObject *parent) : UI_item(UI_CHECKBOX_TYPE, parent), m_selected()
{
    from_json(j);
}

void UI_checkbox::toggle_selected()
{
    m_selected = !m_selected;
    emit value_changed();
    emit on_change(m_text);
}

bool UI_checkbox::selected() const
{
    return m_selected;
}

std::string UI_checkbox::selected_text() const
{
    if (m_selected)
    {
        return m_text;
    }
    return "";
}

void UI_checkbox::from_json(const json &j)
{
    UI_item::from_json(j);

    if (j.contains("selected"))
    {
        m_selected = j["selected"];
    }
}

json UI_checkbox::to_json(size_t id) const
{
    json j = UI_item::to_json(id);
    j["selected"] = m_selected;
    return j;
}
