#include "ui_stacked_widget.hpp"
#include <QSignalSpy>
#include <gtest/gtest.h>

TEST(UI_stacked_widget, to_from_json)
{
    UI_stacked_widget stacked_widget(3, 0);
    json j = stacked_widget.to_json(0);
    UI_stacked_widget stacked_widget2;
    stacked_widget2.from_json(j);
    EXPECT_EQ(stacked_widget.selected(), stacked_widget2.selected());
}

TEST(UI_stacked_widget, set_selected)
{
    UI_stacked_widget stacked_widget(3, 0);
    QSignalSpy spy_ui(&stacked_widget, &UI_stacked_widget::ui_changed);
    EXPECT_EQ(stacked_widget.selected(), 0);
    stacked_widget.set_selected(1);
    EXPECT_EQ(stacked_widget.selected(), 1);
    stacked_widget.set_selected(3);
    EXPECT_EQ(stacked_widget.selected(), 1);
    EXPECT_EQ(spy_ui.count(), 1);
}

TEST(UI_stacked_widget, advance_tab)
{
    UI_stacked_widget stacked_widget(3, 0);
    QSignalSpy spy_ui(&stacked_widget, &UI_stacked_widget::ui_changed);
    EXPECT_EQ(stacked_widget.selected(), 0);
    stacked_widget.advance_tab();
    EXPECT_EQ(stacked_widget.selected(), 1);
    stacked_widget.advance_tab();
    EXPECT_EQ(stacked_widget.selected(), 2);
    stacked_widget.advance_tab();
    EXPECT_EQ(stacked_widget.selected(), 0);
    EXPECT_EQ(spy_ui.count(), 3);
}
