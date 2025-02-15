#include "UI_led.hpp"

UI_led::UI_led(std::string text, Color fg_color, Color bg_color, QObject *parent)
    : UI_item(UI_LED_TYPE, text, 12, fg_color, bg_color, parent)
{
}

UI_led::UI_led(const json &j, QObject *parent) : UI_item(UI_LED_TYPE, parent)
{
    from_json(j);
}

void UI_led::set_led(bool state, Color on, Color off)
{
    set_bg_color(state ? on : off);
}

void UI_led::from_json(const json &j)
{
    UI_item::from_json(j);
}

json UI_led::to_json(size_t id) const
{
    json j = UI_item::to_json(id);
    return j;
}
