#include "Test.hpp"

TEST(UI_combobox, to_from_json)
{
    UI_combobox combobox(Color(255, 255, 255), Color(0, 0, 0), {"option1", "option2"});
    json j = combobox.to_json(0);
    UI_combobox combobox2;
    combobox2.from_json(j);
    EXPECT_EQ(combobox.text(), combobox2.text());
    EXPECT_EQ(combobox.fg_color(), combobox2.fg_color());
    EXPECT_EQ(combobox.bg_color(), combobox2.bg_color());
    EXPECT_EQ(combobox.selected(), combobox2.selected());
}

TEST(UI_combobox, signal)
{
    UI_combobox combobox(Color::Default, Color::Default, {"option1", "option2"});
    QSignalSpy spy(&combobox, &UI_combobox::changed);
    combobox.set_selected(1);
    EXPECT_EQ(spy.count(), 1);
}
