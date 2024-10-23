#include "UI_button.hpp"

UI_button::UI_button(std::string text, Color fg_color, Color bg_color, QObject *parent)
    : UI_item(UI_BUTTON_TYPE, text, 12, fg_color, bg_color, parent)
{
}

UI_button::UI_button(const json &j, QObject *parent) : UI_item(UI_BUTTON_TYPE, parent)
{
    from_json(j);
}

void UI_button::from_json(const json &j)
{
    UI_item::from_json(j);
}

json UI_button::to_json(size_t id) const
{
    json j = UI_item::to_json(id);
    return j;
}

void UI_button::click()
{
    emit on_click();
}
