#include <gtest/gtest.h>
#include <QSignalSpy>
#include "ui_spinbox.hpp"

TEST(UI_spinbox, to_from_json)
{
    UI_spinbox spinbox(0, 10, 5);
    json j = spinbox.to_json(0);
    UI_spinbox spinbox2;
    spinbox2.from_json(j);
    EXPECT_EQ(spinbox.value(), spinbox2.value());
    EXPECT_EQ(spinbox.min(), spinbox2.min());
    EXPECT_EQ(spinbox.max(), spinbox2.max());
}

TEST(UI_spinbox, signal)
{
    UI_spinbox spinbox(0, 10, 5);
    QSignalSpy spy_ui(&spinbox, &UI_spinbox::ui_changed);
    QSignalSpy spy_val(&spinbox, &UI_spinbox::changed);
    spinbox.set_value(2);
    EXPECT_EQ(spy_ui.count(), 1);
    EXPECT_EQ(spy_val.count(), 1);
}
