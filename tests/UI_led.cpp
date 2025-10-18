#include "ui_led.hpp"
#include <QSignalSpy>
#include <gtest/gtest.h>

TEST(UI_led, to_from_json)
{
    UI_led led("led_text", Color(0, 0, 0), Color(255, 255, 255));
    json j = led.to_json(0);
    UI_led led2;
    led2.from_json(j);
    EXPECT_EQ(led.text(), led2.text());
    EXPECT_EQ(led.fg_color(), led2.fg_color());
    EXPECT_EQ(led.bg_color(), led2.bg_color());
}

TEST(UI_led, set_led_color)
{
    UI_led led("led_text", Color(0, 0, 0), Color(255, 255, 255));
    led.set_bg_color(Color(0, 255, 0));
    EXPECT_EQ(led.bg_color(), Color(0, 255, 0));
}

TEST(UI_led, set_led)
{
    UI_led led("led_text", Color(0, 0, 0), Color(255, 255, 255));
    led.set_led(false, Color::Green, Color::Red);
    EXPECT_EQ(led.bg_color(), Color(Color::Red));
    led.set_led(true, Color::Green, Color::Red);
    EXPECT_EQ(led.bg_color(), Color(Color::Green));
}
