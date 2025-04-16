#include "ui_label.hpp"

UI_label::UI_label(std::string text, Color fg_color, Color bg_color, QObject *parent)
    : UI_item(UI_LABEL_TYPE, text, 12, fg_color, bg_color, parent)
{
}

UI_label::UI_label(const json &j, QObject *parent) : UI_item(UI_LABEL_TYPE, parent)
{
    from_json(j);
}

void UI_label::from_json(const json &j)
{
    UI_item::from_json(j);
}

json UI_label::to_json(size_t id) const
{
    return UI_item::to_json(id);
}
