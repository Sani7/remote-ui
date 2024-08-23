#include "UI_led.hpp"

UI_led::UI_led(std::string id, std::string text, Color fg_color, Color bg_color) :
UI_item(id, UI_LED_TYPE, text, 12, fg_color, bg_color)
{
    
}

UI_led::UI_led(const json& j) : UI_item(j)
{
    from_json(j);
}

void UI_led::from_json(const json& j)
{
    UI_item::from_json(j);
}

json UI_led::to_json() const
{
    return  UI_item::to_json();
}