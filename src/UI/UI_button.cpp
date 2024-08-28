#include "UI_button.hpp"

UI_button::UI_button(std::string id, std::string text, Color fg_color, Color bg_color, std::function<void(std::string)> on_click)
    : UI_item(id, UI_BUTTON_TYPE, text, 12, fg_color, bg_color)
{
    this->m_on_click = on_click;
}

UI_button::UI_button(const json& j)
    : UI_item(UI_BUTTON_TYPE),
    m_on_click(nullptr)
{
    from_json(j);
}

void UI_button::from_json(const json& j)
{
    UI_item::from_json(j);
}

json UI_button::to_json() const
{
    json j = UI_item::to_json();
    return j;
}

void UI_button::click() const
{
    if (m_on_click != nullptr)
    {
        std::thread([this] { m_on_click(m_id); }).detach();
    }
}
