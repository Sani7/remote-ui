#include "Test.hpp"

TEST(UI_combobox, to_from_json)
{
    UI_combobox checkbox("checkbox_text", Color(255, 255, 255), Color(0, 0, 0), {"option1", "option2"});
    json j = checkbox.to_json(0);
    UI_combobox checkbox2(j);
    EXPECT_EQ(checkbox.text(), checkbox2.text());
    EXPECT_EQ(checkbox.fg_color(), checkbox2.fg_color());
    EXPECT_EQ(checkbox.bg_color(), checkbox2.bg_color());
    EXPECT_EQ(checkbox.selected(), checkbox2.selected());
}