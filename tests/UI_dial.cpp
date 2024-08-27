#include "Test.hpp"

TEST(UI_dial, to_from_json)
{
    UI_dial dial("slider_id", "slider_text", Color(255, 255, 255), Color(0, 0, 0), 0, 100, 50);
    json j = dial.to_json();
    UI_dial dial2(j);
    EXPECT_EQ(dial.id(), dial2.id());
    EXPECT_EQ(dial.text(), dial2.text());
    EXPECT_EQ(dial.fg_color(), dial2.fg_color());
    EXPECT_EQ(dial.bg_color(), dial2.bg_color());
    EXPECT_EQ(dial.value(), dial2.value());
}

TEST(UI_dial, set_value)
{
    UI_dial dial("slider_id", "slider_text", Color(255, 255, 255), Color(0, 0, 0), 0, 100, 50);
    dial.set_value(75);
    EXPECT_EQ(dial.value(), 75);
}
