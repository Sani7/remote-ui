#include <gtest/gtest.h>
#include <QSignalSpy>
#include "ui_tab_widget.hpp"

TEST(UI_tab_widget, to_from_json)
{
    UI_tab_widget stacked_widget({"Tab 0", "Tab 1", "Tab 2"}, 0);
    json j = stacked_widget.to_json(0);
    UI_tab_widget stacked_widget2;
    stacked_widget2.from_json(j);
    EXPECT_EQ(stacked_widget.selected(), stacked_widget2.selected());
}

TEST(UI_tab_widget, set_selected)
{
    UI_tab_widget stacked_widget({"Tab 0", "Tab 1", "Tab 2"}, 0);
    QSignalSpy spy_ui(&stacked_widget, &UI_tab_widget::ui_changed);
    EXPECT_EQ(stacked_widget.selected(), 0);
    stacked_widget.set_selected(1);
    EXPECT_EQ(stacked_widget.selected(), 1);
    stacked_widget.set_selected(3);
    EXPECT_EQ(stacked_widget.selected(), 1);
    EXPECT_EQ(spy_ui.count(), 1);
}

TEST(UI_tab_widget, advance_tab)
{
    UI_tab_widget stacked_widget({"Tab 0", "Tab 1", "Tab 2"}, 0);
    QSignalSpy spy_ui(&stacked_widget, &UI_tab_widget::ui_changed);
    EXPECT_EQ(stacked_widget.selected(), 0);
    stacked_widget.advance_tab();
    EXPECT_EQ(stacked_widget.selected(), 1);
    stacked_widget.advance_tab();
    EXPECT_EQ(stacked_widget.selected(), 2);
    stacked_widget.advance_tab();
    EXPECT_EQ(stacked_widget.selected(), 0);
    EXPECT_EQ(spy_ui.count(), 3);
}

TEST(UI_tab_widget, set_tab_name)
{
    UI_tab_widget stacked_widget({"Tab 0", "Tab 1", "Tab 2"}, 0);
    QSignalSpy spy_ui(&stacked_widget, &UI_tab_widget::ui_changed);
    EXPECT_EQ(stacked_widget.tab_name(1), "Tab 1");
    stacked_widget.set_tab_name(1, "New Tab 1");
    EXPECT_EQ(stacked_widget.tab_name(1), "New Tab 1");
    stacked_widget.set_tab_name(3, "Invalid Tab");
    EXPECT_EQ(stacked_widget.tab_name(3), "");
    EXPECT_EQ(spy_ui.count(), 1);
}

TEST(UI_tab_widget, set_visible)
{
    UI_tab_widget stacked_widget({"Tab 0", "Tab 1", "Tab 2"}, 0);
    QSignalSpy spy_ui(&stacked_widget, &UI_tab_widget::ui_changed);
    EXPECT_EQ(stacked_widget.visible(1), true);
    stacked_widget.set_visible(1, false);
    EXPECT_EQ(stacked_widget.visible(1), false);
    stacked_widget.set_visible(3, true);
    EXPECT_EQ(stacked_widget.visible(3), false);
    EXPECT_EQ(spy_ui.count(), 1);
}

TEST(UI_tab_widget, signal)
{
    UI_tab_widget stacked_widget({"Tab 0", "Tab 1", "Tab 2"}, 0);
    QSignalSpy spy_ui(&stacked_widget, &UI_tab_widget::ui_changed);
    QSignalSpy spy_changed(&stacked_widget, &UI_tab_widget::changed);
    stacked_widget.set_selected(1);
    EXPECT_EQ(spy_ui.count(), 1);
    EXPECT_EQ(spy_changed.count(), 1);
}
