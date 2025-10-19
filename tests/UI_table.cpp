#include "ui_table.hpp"
#include <QSignalSpy>
#include <gtest/gtest.h>

TEST(UI_table, to_from_json)
{
    UI_table table(3, 3, {"R1", "R2", "R3"}, {"C1", "C2", "C3"});
    json j = table.to_json(0);
    UI_table table2;
    table2.from_json(j);
    EXPECT_EQ(table2.row_count(), 3);
    EXPECT_EQ(table2.column_count(), 3);
    EXPECT_EQ(table2.row_label(0), "R1");
    EXPECT_EQ(table2.row_label(1), "R2");
    EXPECT_EQ(table2.row_label(2), "R3");
    EXPECT_EQ(table2.column_label(0), "C1");
    EXPECT_EQ(table2.column_label(1), "C2");
    EXPECT_EQ(table2.column_label(2), "C3");
}

TEST(UI_table, set_row_column_labels)
{
    UI_table table(2, 2, {"R1", "R2"}, {"C1", "C2"});
    QSignalSpy spy_ui(&table, &UI_table::ui_changed);
    table.set_row_label(0, "New R1");
    EXPECT_EQ(table.row_label(0), "New R1");
    table.set_row_label(1, "New R2");
    EXPECT_EQ(table.row_label(1), "New R2");
    table.set_column_label(0, "New C1");
    EXPECT_EQ(table.column_label(0), "New C1");
    table.set_column_label(1, "New C2");
    EXPECT_EQ(table.column_label(1), "New C2");
    EXPECT_EQ(spy_ui.count(), 4);
}

TEST(UI_table, insert_item)
{
    UI_table table(3, 3, {"R1", "R2", "R3"}, {"C1", "C2", "C3"});
    QSignalSpy spy_ui(&table, &UI_table::ui_changed);
    table.insert_item(0, 0, "Item 1");
    table.insert_item(1, 1, "Item 2");
    table.insert_item(2, 2, "Item 3");
    EXPECT_EQ(spy_ui.count(), 3);
    EXPECT_EQ(table.item(0, 0), "Item 1");
    EXPECT_EQ(table.item(1, 1), "Item 2");
    EXPECT_EQ(table.item(2, 2), "Item 3");
}

TEST(UI_table, empty_item)
{
    UI_table table(2, 2, {"R1", "R2"}, {"C1", "C2"});
    QSignalSpy spy_ui(&table, &UI_table::ui_changed);
    table.insert_item(0, 0, "Item 1");
    table.insert_item(1, 1, "Item 2");
    EXPECT_EQ(spy_ui.count(), 2);
    table.empty_item(0, 0);
    EXPECT_EQ(table.item(0, 0), "");
    table.empty_item(1, 1);
    EXPECT_EQ(table.item(1, 1), "");
    EXPECT_EQ(spy_ui.count(), 4);
}
