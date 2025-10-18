#include <gtest/gtest.h>
#include <QSignalSpy>
#include "ui_status_bar.hpp"

TEST(UI_status_bar, to_from_json)
{
    UI_status_bar statusbar("message");
    json j = statusbar.to_json(0);
    UI_status_bar statusbar2;
    statusbar2.from_json(j);
    EXPECT_EQ(statusbar.text(), statusbar2.text());
}

TEST(UI_status_bar, show_message)
{
    UI_status_bar statusbar("message");
    QSignalSpy spy_ui(&statusbar, &UI_status_bar::ui_changed);
    EXPECT_EQ(statusbar.text(), "message");
    EXPECT_EQ(statusbar.timeout(), 0);
    statusbar.show_message("random text", 3);
    EXPECT_EQ(statusbar.text(), "random text");
    EXPECT_EQ(statusbar.timeout(), 3);
    EXPECT_EQ(spy_ui.count(), 1);
}
