#include "ui_dial.hpp"
#include <QSignalSpy>
#include <gtest/gtest.h>

TEST(UI_dial, to_from_json)
{
    UI_dial dial("slider_text", Color(255, 255, 255), Color(0, 0, 0), 0, 100, 50);
    json j = dial.to_json(0);
    UI_dial dial2;
    dial2.from_json(j);
    EXPECT_EQ(dial.text(), dial2.text());
    EXPECT_EQ(dial.fg_color(), dial2.fg_color());
    EXPECT_EQ(dial.bg_color(), dial2.bg_color());
    EXPECT_EQ(dial.value(), dial2.value());
    EXPECT_EQ(dial.unit(), dial2.unit());
}

TEST(UI_dial, set_value)
{
    UI_dial dial("slider_text", Color(255, 255, 255), Color(0, 0, 0), 0, 100, 50);
    QSignalSpy spy(&dial, &UI_dial::ui_changed);
    dial.set_value(75);
    EXPECT_EQ(dial.value(), 75);
    EXPECT_EQ(spy.count(), 1);
}

TEST(UI_dial, unit)
{
    UI_dial dial("slider_text", "unit", Color(255, 255, 255), Color(0, 0, 0), 0, 100, 50);
    EXPECT_EQ(dial.unit(), "unit");
}
