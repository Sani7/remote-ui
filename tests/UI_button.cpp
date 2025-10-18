#include "Test.hpp"

TEST(UI_button, to_from_json)
{
    UI_button button("button_text", Color(255, 255, 255), Color(0, 0, 0), nullptr);
    json j = button.to_json(0);
    UI_button button2;
    button2.from_json(j);
    EXPECT_EQ(button.text(), button2.text());
    EXPECT_EQ(button.fg_color(), button2.fg_color());
    EXPECT_EQ(button.bg_color(), button2.bg_color());
}
