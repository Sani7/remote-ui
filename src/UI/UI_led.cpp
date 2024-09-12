#include "UI_led.hpp"

UI_led::UI_led(std::string id, std::string text, Color fg_color, Color bg_color, Color led_color, QObject* parrent)
    : UI_item(id, UI_LED_TYPE, text, 12, fg_color, bg_color, parrent), m_led_color(led_color)
{
}

UI_led::UI_led(const json &j, QObject* parrent) : UI_item(UI_LED_TYPE, parrent)
{
    from_json(j);
}

Color UI_led::led_color() const
{
    return m_led_color;
}

void UI_led::set_led_color(Color led_color)
{
    if (m_led_color != led_color)
    {
        m_led_color = led_color;
        emit value_changed();
    }
}

void UI_led::from_json(const json &j)
{
    UI_item::from_json(j);
    std::string color;
    j.at("led_color").get_to(color);
    m_led_color = Color(color);
}

json UI_led::to_json() const
{
    json j = UI_item::to_json();
    j["led_color"] = m_led_color.to_hex();
    return j;
}
