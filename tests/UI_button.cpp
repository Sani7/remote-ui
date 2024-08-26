#include "Test.hpp"

TEST(UI_button, to_from_json)
{
    UI_button button("button_id", "button_text", Color(255, 255, 255), Color(0, 0, 0), nullptr);
    json j = button.to_json();
    UI_button button2(j);
    EXPECT_EQ(button.id(), button2.id());
    EXPECT_EQ(button.text(), button2.text());
    EXPECT_EQ(button.fg_color(), button2.fg_color());
    EXPECT_EQ(button.bg_color(), button2.bg_color());
}

void on_click(std::string id)
{
    UNUSED(id);
    EXPECT_EQ(id, "button_id");
}

TEST(UI_button, on_click)
{
    UI_button button("button_id", "button_text", Color(255, 255, 255), Color(0, 0, 0), on_click);
    button.click();
}