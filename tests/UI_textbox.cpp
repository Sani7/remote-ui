#include "Test.hpp"

TEST(UI_textbox, to_from_json)
{
    UI_textbox textbox(std::string("textbox"), std::string("hint"), Color(0, 0, 0), Color(255, 255, 255));
    json j = textbox.to_json();
    UI_textbox textbox2(j);
    EXPECT_EQ(textbox.id(), textbox2.id());
    EXPECT_EQ(textbox.text(), textbox2.text());
    EXPECT_EQ(textbox.fg_color(), textbox2.fg_color());
    EXPECT_EQ(textbox.bg_color(), textbox2.bg_color());
    EXPECT_EQ(textbox.text(), textbox2.text());
}

TEST(UI_textbox, set_text)
{
    UI_textbox textbox(std::string("textbox"), std::string("hint"), Color(0, 0, 0), Color(255, 255, 255));
    EXPECT_EQ(textbox.text(), "");
    textbox.set_text("random text");
    EXPECT_EQ(textbox.text(), "random text");
}