#include "Test.hpp"

TEST(UI_slider, to_from_json)
{
    UI_slider slider("slider_id", "slider_text", Color(255, 255, 255), Color(0, 0, 0), 0, 100, 50, nullptr);
    json j = slider.to_json();
    UI_slider slider2(j);
    EXPECT_EQ(slider.id(), slider2.id());
    EXPECT_EQ(slider.text(), slider2.text());
    EXPECT_EQ(slider.fg_color(), slider2.fg_color());
    EXPECT_EQ(slider.bg_color(), slider2.bg_color());
    EXPECT_EQ(slider.value(), slider2.value());
}

TEST(UI_slider, value)
{
    UI_slider slider("slider_id", "slider_text", Color(255, 255, 255), Color(0, 0, 0), 0, 100, 50);
    EXPECT_EQ(slider.value(), 50);
}
