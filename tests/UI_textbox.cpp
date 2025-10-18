#include "Test.hpp"

TEST(UI_textbox, to_from_json)
{
    UI_textbox textbox("hint", Color(0, 0, 0), Color(255, 255, 255));
    json j = textbox.to_json(0);
    UI_textbox textbox2;
    textbox2.from_json(j);
    EXPECT_EQ(textbox.text(), textbox2.text());
    EXPECT_EQ(textbox.fg_color(), textbox2.fg_color());
    EXPECT_EQ(textbox.bg_color(), textbox2.bg_color());
    EXPECT_EQ(textbox.hint(), textbox2.hint());
}

TEST(UI_textbox, set_text)
{
    UI_textbox textbox("hint", Color(0, 0, 0), Color(255, 255, 255));
    QSignalSpy spy_ui(&textbox, &UI_textbox::ui_changed);
    QSignalSpy spy_value(&textbox, &UI_textbox::changed);
    EXPECT_EQ(textbox.text(), "");
    textbox.set_text("random text");
    EXPECT_EQ(textbox.text(), "random text");
    EXPECT_GE(spy_ui.count(), 0);
    EXPECT_GE(spy_value.count(), 0);
}

TEST(UI_textbox, set_hint)
{
    UI_textbox textbox("hint", Color(0, 0, 0), Color(255, 255, 255));
    QSignalSpy spy_ui(&textbox, &UI_textbox::ui_changed);
    EXPECT_EQ(textbox.hint(), "hint");
    textbox.set_hint("random text");
    EXPECT_EQ(textbox.hint(), "random text");
    EXPECT_EQ(spy_ui.count(), 1);
}
