#include "UI_button.hpp"

UI_button::UI_button(std::string id, std::string text, Color fg_color, Color bg_color, QObject *parrent)
    : UI_item(id, UI_BUTTON_TYPE, text, 12, fg_color, bg_color, parrent)
{
}

UI_button::UI_button(const json &j, QObject *parrent) : UI_item(UI_BUTTON_TYPE, parrent)
{
    from_json(j);
}

void UI_button::from_json(const json &j)
{
    UI_item::from_json(j);
}

json UI_button::to_json() const
{
    json j = UI_item::to_json();
    return j;
}

void UI_button::click()
{
    emit on_click(m_id);
}
