#include "ui_checkbox.hpp"

UI_checkbox::UI_checkbox(QObject *parent) : UI_item(UI_CHECKBOX_TYPE, parent), m_checked()
{
}

UI_checkbox::UI_checkbox(std::string text, Color fg_color, Color bg_color, QObject *parent)
    : UI_item(UI_CHECKBOX_TYPE, text, 0, fg_color, bg_color, parent), m_checked(false)
{
}

UI_checkbox::UI_checkbox(const json &j, QObject *parent) : UI_item(UI_CHECKBOX_TYPE, parent), m_checked()
{
    from_json(j);
}

void UI_checkbox::click()
{
    m_checked = !m_checked;
    emit ui_changed();
    emit on_change(m_checked);
}

void UI_checkbox::set_state(bool state)
{
    m_checked = state;
    emit ui_changed();
    emit on_change(m_checked);
}

bool UI_checkbox::state() const
{
    return m_checked;
}

void UI_checkbox::from_json(const json &j)
{
    UI_item::from_json(j);

    if (j.contains("checked"))
    {
        m_checked = j["checked"];
    }
}

json UI_checkbox::to_json(size_t id) const
{
    json j = UI_item::to_json(id);
    j["checked"] = m_checked;
    return j;
}
