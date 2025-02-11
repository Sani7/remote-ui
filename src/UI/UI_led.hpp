#pragma once
#include "UI_item.hpp"
#include <string>

#define UI_LED_TYPE std::string("UI_led")

class UI_led : public UI_item
{
    Q_OBJECT
  public:
    UI_led(std::string text, Color fg_color, Color bg_color, QObject *parent = nullptr);
    UI_led(const json &j, QObject *parent = nullptr);

    Color led_color() const;
    void set_led_color(Color led_color);

    void from_json(const json &j) override;
    json to_json(size_t id) const override;
};
