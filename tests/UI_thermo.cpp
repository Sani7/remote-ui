#include "Test.hpp"

TEST(UI_thermo, to_from_json)
{
    UI_thermo thermo("thermo_text", Color(255, 255, 255), Color(0, 0, 0), 0, 100, 50);
    json j = thermo.to_json(0);
    UI_thermo thermo2;
    thermo2.from_json(j);
    EXPECT_EQ(thermo.text(), thermo2.text());
    EXPECT_EQ(thermo.fg_color(), thermo2.fg_color());
    EXPECT_EQ(thermo.bg_color(), thermo2.bg_color());
    EXPECT_EQ(thermo.value(), thermo2.value());
}

TEST(UI_thermo, set_value)
{
    UI_thermo thermo("thermo_text", Color(255, 255, 255), Color(0, 0, 0), 0, 100, 50);
    thermo.set_value(75);
    EXPECT_EQ(thermo.value(), 75);
}
