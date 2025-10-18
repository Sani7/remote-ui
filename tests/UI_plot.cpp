#include <gtest/gtest.h>
#include <QSignalSpy>
#include "ui_plot.hpp"

TEST(UI_plot, to_from_json)
{
    UI_plot plot("Plot", "X-Axis", "Y-Axis");
    json j = plot.to_json(0);
    UI_plot plot2;
    plot2.from_json(j);
    EXPECT_EQ(plot.x_label(), plot2.x_label());
    EXPECT_EQ(plot.y_label(), plot2.y_label());
    EXPECT_EQ(plot.x_vals(), plot2.x_vals());
    EXPECT_EQ(plot.y_vals(), plot2.y_vals());
}

TEST(UI_plot, set_label)
{
    UI_plot plot("Plot", "X-Axis", "Y-Axis");
    QSignalSpy spy_ui(&plot, &UI_plot::ui_changed);
    plot.set_x_label("New X-Axis");
    EXPECT_EQ(plot.x_label(), "New X-Axis");
    plot.set_y_label("New Y-Axis");
    EXPECT_EQ(plot.y_label(), "New Y-Axis");
    EXPECT_EQ(spy_ui.count(), 2);
}

TEST(UI_plot, add_value)
{
    UI_plot plot("Plot", "X-Axis", "Y-Axis");
    QSignalSpy spy_ui(&plot, &UI_plot::ui_changed);
    plot.add_value(1.0);
    plot.add_value(2.0);
    EXPECT_EQ(plot.x_vals().size(), 2);
    EXPECT_EQ(plot.y_vals().size(), 2);
    EXPECT_EQ(plot.x_vals()[0], 0.0);
    EXPECT_EQ(plot.y_vals()[0], 1.0);
    EXPECT_EQ(plot.x_vals()[1], 1.0);
    EXPECT_EQ(plot.y_vals()[1], 2.0);
    plot.add_value(3.0, 4.0);
    EXPECT_EQ(plot.x_vals().size(), 3);
    EXPECT_EQ(plot.y_vals().size(), 3);
    EXPECT_EQ(plot.x_vals()[2], 3.0);
    EXPECT_EQ(plot.y_vals()[2], 4.0);
    EXPECT_EQ(spy_ui.count(), 3);
}

TEST(UI_plot, clean_vals)
{
    UI_plot plot("Plot", "X-Axis", "Y-Axis");
    plot.add_value(1.0);
    plot.add_value(2.0);
    QSignalSpy spy_ui(&plot, &UI_plot::ui_changed);
    plot.clean_vals();
    EXPECT_EQ(plot.x_vals().size(), 0);
    EXPECT_EQ(plot.y_vals().size(), 0);
    EXPECT_EQ(spy_ui.count(), 1);
}
