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

static double val = 0;
void on_change(std::string id, double value)
{
    EXPECT_EQ(id, "slider_id");
    val = value;
}

TEST(UI_slider, set_value)
{
    UI_slider slider("slider_id", "slider_text", Color(255, 255, 255), Color(0, 0, 0), 0, 100, 50, on_change);
    slider.set_value(75);
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    EXPECT_EQ(val, 75);
}

TEST(UI_slider, value)
{
    UI_slider slider("slider_id", "slider_text", Color(255, 255, 255), Color(0, 0, 0), 0, 100, 50, on_change);
    EXPECT_EQ(slider.value(), 50);
}
