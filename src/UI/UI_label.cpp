#include "UI_label.hpp"

UI_label::UI_label(std::string id, std::string text, Color fg_color, Color bg_color)
    : UI_item(id, UI_LABEL_TYPE, text, 12, fg_color, bg_color)
{
}

UI_label::UI_label(const json &j) : UI_item(UI_LABEL_TYPE)
{
    from_json(j);
}

void UI_label::from_json(const json &j)
{
    UI_item::from_json(j);
}

json UI_label::to_json() const
{
    return UI_item::to_json();
}