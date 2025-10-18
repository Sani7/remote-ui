#include "ui_checkbox.hpp"
#include <QSignalSpy>
#include <gtest/gtest.h>

TEST(UI_checkbox, to_from_json)
{
    UI_checkbox checkbox("checkbox_text", Color(255, 255, 255), Color(0, 0, 0));
    json j = checkbox.to_json(0);
    UI_checkbox checkbox2;
    checkbox2.from_json(j);
    EXPECT_EQ(checkbox.text(), checkbox2.text());
    EXPECT_EQ(checkbox.fg_color(), checkbox2.fg_color());
    EXPECT_EQ(checkbox.bg_color(), checkbox2.bg_color());
    EXPECT_EQ(checkbox.state(), checkbox2.state());
}

TEST(UI_checkbox, signal)
{
    UI_checkbox checkbox("checkbox_text", Color::Default, Color::Default);
    QSignalSpy spy(&checkbox, &UI_checkbox::changed);
    checkbox.click();
    EXPECT_EQ(spy.count(), 1);
}
