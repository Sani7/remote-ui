#pragma once
#include <string>
#include "UI_item.hpp"

#define UI_LED_TYPE std::string("UIled")

class UI_led : public UI_item
{
public:
    UI_led(std::string id, std::string text, Color fg_color, Color bg_color, Color led_color);
    UI_led(const json& j);

    Color led_color() const;
    void set_led_color(Color led_color);

    void from_json(const json& j) override;
    json to_json() const override;
private:
    Color m_led_color;
};