#include "Test.hpp"

TEST(UI_label, to_from_json)
{
    UI_label label(std::string("label"), std::string("label_text"), Color(0, 0, 0), Color(255, 255, 255));
    json j = label.to_json();
    EXPECT_EQ(j["type"], "UI_label");
    EXPECT_EQ(j["id"], "label");
    EXPECT_EQ(j["text"], "label_text");
    EXPECT_EQ(j["text_size"], 12);
    EXPECT_EQ(j["fg_color"], "#000000FF");
    EXPECT_EQ(j["bg_color"], "#FFFFFFFF");

    UI_label label2(j);
    EXPECT_EQ(label2.id(), "label");
    EXPECT_EQ(label2.text(), "label_text");
    EXPECT_EQ(label2.text_size(), 12);
    EXPECT_EQ(label2.fg_color(), Color(0, 0, 0));
    EXPECT_EQ(label2.bg_color(), Color(255, 255, 255));
}