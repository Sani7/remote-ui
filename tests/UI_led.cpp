#include "Test.hpp"

TEST(UI_led, to_from_json)
{
    UI_led led("led_text", Color(0, 0, 0), Color(255, 255, 255), Color(255, 0, 0));
    json j = led.to_json(0);
    UI_led led2(j);
    EXPECT_EQ(led.text(), led2.text());
    EXPECT_EQ(led.fg_color(), led2.fg_color());
    EXPECT_EQ(led.bg_color(), led2.bg_color());
    EXPECT_EQ(led.led_color(), led2.led_color());
}

TEST(UI_led, set_led_color)
{
    UI_led led("led_text", Color(0, 0, 0), Color(255, 255, 255), Color(255, 0, 0));
    led.set_led_color(Color(0, 255, 0));
    EXPECT_EQ(led.led_color(), Color(0, 255, 0));
}