#include <gtest/gtest.h>
#include <QSignalSpy>
#include "ui_slider.hpp"

TEST(UI_slider, to_from_json)
{
    UI_slider slider("slider_text", Color(255, 255, 255), Color(0, 0, 0), 0, 100, 50, nullptr);
    json j = slider.to_json(0);
    UI_slider slider2;
    slider2.from_json(j);
    EXPECT_EQ(slider.text(), slider2.text());
    EXPECT_EQ(slider.fg_color(), slider2.fg_color());
    EXPECT_EQ(slider.bg_color(), slider2.bg_color());
    EXPECT_EQ(slider.value(), slider2.value());
}

TEST(UI_slider, value)
{
    UI_slider slider("slider_text", Color(255, 255, 255), Color(0, 0, 0), 0, 100, 50);
    QSignalSpy spy_ui(&slider, &UI_slider::ui_changed);
    QSignalSpy spy_value(&slider, &UI_slider::changed);
    EXPECT_EQ(slider.value(), 50);
    slider.set_value(10);
    EXPECT_EQ(spy_ui.count(), 1);
    EXPECT_EQ(spy_value.count(), 1);
}
