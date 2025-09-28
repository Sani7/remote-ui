#include "ui_textbox.hpp"

UI_textbox::UI_textbox(std::string hint, Color fg_color, Color bg_color, QObject *parent)
    : UI_item(UI_TEXTBOX_TYPE, "", fg_color, bg_color, parent), m_hint(hint)
{
}

UI_textbox::UI_textbox(const json &j, QObject *parent) : UI_item(UI_TEXTBOX_TYPE, parent)
{
    from_json(j);
}

void UI_textbox::set_text(std::string text)
{
    if (text == m_text)
        return;
    m_text = text;
    emit ui_changed();
    emit on_change(m_text);
}

void UI_textbox::set_hint(std::string hint)
{
    m_hint = hint;
    emit ui_changed();
}

std::string UI_textbox::hint() const
{
    return m_hint;
}

void UI_textbox::from_json(const json &j)
{
    UI_item::from_json(j);
    m_hint = j.at("hint");
}

json UI_textbox::to_json(size_t id) const
{
    json j = UI_item::to_json(id);
    j["hint"] = m_hint;
    return j;
}
