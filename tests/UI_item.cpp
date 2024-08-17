#include "Test.hpp"

TEST(UI_item, is_type)
{
    UI_item item(std::string("item"));
    json j = {{"type", "item"}};
    EXPECT_TRUE(item.is_type(j));
}

TEST(UI_item, set_text)
{
    UI_item item(std::string("item"));
    item.set_text("text");
    EXPECT_EQ(item.text(), "text");
}

TEST(UI_item, text_size)
{
    UI_item item(std::string("item"));
    item.set_text_size(12);
    EXPECT_EQ(item.text_size(), 12);
}

TEST(UI_item, set_fg_color)
{
    UI_item item(std::string("item"));
    item.set_fg_color(Color(0, 0, 0));
    EXPECT_EQ(item.fg_color(), Color(0, 0, 0));

    item.set_fg_color(Color(255, 255, 255));
    EXPECT_EQ(item.fg_color(), Color(255, 255, 255));
}

TEST(UI_item, set_bg_color)
{
    UI_item item(std::string("item"));
    item.set_bg_color(Color(0, 0, 0));
    EXPECT_EQ(item.bg_color(), Color(0, 0, 0));

    item.set_bg_color(Color(255, 255, 255));
    EXPECT_EQ(item.bg_color(), Color(255, 255, 255));
}

TEST(UI_item, from_json)
{
    UI_item item(std::string("item"));
    json j;
    j["type"] = "item";
    j["id"] = "item_id";
    j["text"] = "item_text";
    j["text_size"] = 12;
    j["fg_color"] = "0xFFFFFF00";
    j["bg_color"] = "0x00000000";

    item.from_json(j);
    EXPECT_EQ(item.id(), "item_id");
    EXPECT_EQ(item.text(), "item_text");
    EXPECT_EQ(item.text_size(), 12);
    EXPECT_EQ(item.fg_color(), Color(255, 255, 255));
    EXPECT_EQ(item.bg_color(), Color(0, 0, 0));
}

TEST(UI_item, to_json)
{
    UI_item item("item_id", "item", "item_text", 12, Color(255, 255, 255), Color(0, 0, 0));
    json j = item.to_json();
    EXPECT_EQ(j.at("type"), "item");
    EXPECT_EQ(j.at("id"), "item_id");
    EXPECT_EQ(j.at("text"), "item_text");
    EXPECT_EQ(j.at("text_size"), 12);
    EXPECT_EQ(j.at("fg_color"), Color(255, 255, 255).to_hex());
    EXPECT_EQ(j.at("bg_color"), Color(0, 0, 0).to_hex());
}