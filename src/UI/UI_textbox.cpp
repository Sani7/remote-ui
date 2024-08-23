#include "UI_textbox.hpp"

UI_textbox::UI_textbox(std::string id, std::string hint, Color fg_color, Color bg_color)
    : UI_item(id, UI_TEXTBOX_TYPE, "", 0, fg_color, bg_color), m_hint(hint)
{
}

UI_textbox::UI_textbox(const json& j)
    : UI_item(UI_TEXTBOX_TYPE)
{
    from_json(j);
}

void UI_textbox::set_hint(std::string hint)
{
    m_hint = hint;
}

std::string UI_textbox::hint() const
{
    return m_hint;
}

void UI_textbox::from_json(const json& j)
{
    UI_item::from_json(j);
    m_hint = j["hint"];
}

json UI_textbox::to_json() const
{
    json j = UI_item::to_json();
    j["hint"] = m_hint;
    return j;
}