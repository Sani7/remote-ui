#include "color.hpp"

Color::Color() : p_color(0) {
}

Color::Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
    : Color((red << 24) | (green << 16) | (blue << 8) | alpha) {
}
Color::Color(uint32_t color) : p_color(color) {
}

Color::Color(std::string hex) {
    if (hex.length() == 6) {
        hex = "FF" + hex;
    }
    p_color = std::stoul(hex, nullptr, 16);
}

Color::Color(Colors color) {
    switch (color) {
        case Colors::Aquamarine:
            Color(0x70, 0xDB, 0x93);
            break;
            case Colors::Baker_s_Chocolate:
                Color(0x5C, 0x33, 0x17);
            break;
            case Colors::Black:
                Color(0x00, 0x00, 0x00);
            break;
            case Colors::Blue:
                Color(0x00, 0x00, 0xFF);
            break;
            case Colors::Blue_Violet:
                Color(0x9F, 0x5F, 0x9F);
            break;
            case Colors::Brass:
                Color(0xB5, 0xA6, 0x42);
            break;
            case Colors::Bright_Gold:
                Color(0xD9, 0xD9, 0x19);
            break;
            case Colors::Brown:
                Color(0xA6, 0x2A, 0x2A);
            break;
            case Colors::Bronze:
                Color(0x8C, 0x78, 0x53);
            break;
            case Colors::Bronze_II:
                Color(0xA6, 0x7D, 0x3D);
            break;
            case Colors::Cadet_Blue:
                Color(0x5F, 0x9F, 0x9F);
            break;
            case Colors::Cool_Copper:
                Color(0xD9, 0x87, 0x19);
            break;
            case Colors::Copper:
                Color(0xB8, 0x73, 0x33);
            break;
            case Colors::Coral:
                Color(0xFF, 0x7F, 0x00);
            break;
            case Colors::Corn_Flower_Blue:
                Color(0x42, 0x42, 0x6F);
            break;
            case Colors::Cyan:
                Color(0x00, 0xFF, 0xFF);
            break;
            case Colors::Dark_Brown:
                Color(0x5C, 0x40, 0x33);
            break;
            case Colors::Dark_Green:
                Color(0x2F, 0x4F, 0x2F);
            break;
            case Colors::Dark_Green_Copper:
                Color(0x4A, 0x76, 0x6E);
            break;
            case Colors::Dark_Olive_Green:
                Color(0x4F, 0x4F, 0x2F);
            break;
            case Colors::Dark_Orchid:
                Color(0x99, 0x32, 0xCD);
            break;
            case Colors::Dark_Purple:
                Color(0x87, 0x1F, 0x78);
            break;
            case Colors::Dark_Slate_Blue:
                Color(0x6B, 0x23, 0x8E);
            break;
            case Colors::Dark_Slate_Grey:
                Color(0x2F, 0x4F, 0x4F);
            break;
            case Colors::Dark_Tan:
                Color(0x97, 0x69, 0x4F);
            break;
            case Colors::Dark_Turquoise:
                Color(0x70, 0x93, 0xDB);
            break;
            case Colors::Dark_Wood:
                Color(0x85, 0x5E, 0x42);
            break;
            case Colors::Dim_Grey:
                Color(0x54, 0x54, 0x54);
            break;
            case Colors::Dusty_Rose:
                Color(0x85, 0x63, 0x63);
            break;
            case Colors::Feldspar:
                Color(0xD1, 0x92, 0x75);
            break;
            case Colors::Firebrick:
                Color(0x8E, 0x23, 0x23);
            break;
            case Colors::Forest_Green:
                Color(0x23, 0x8E, 0x23);
            break;
            case Colors::Gold:
                Color(0xCD, 0x7F, 0x32);
            break;
            case Colors::Goldenrod:
                Color(0xDB, 0xDB, 0x70);
            break;
            case Colors::Grey:
                Color(0xC0, 0xC0, 0xC0);
            break;
            case Colors::Green:
                Color(0x00, 0xFF, 0x00);
            break;
            case Colors::Green_Copper:
                Color(0x52, 0x7F, 0x76);
            break;
            case Colors::Green_Yellow:
                Color(0x93, 0xDB, 0x70);
            break;
            case Colors::Hunter_Green:
                Color(0x21, 0x5E, 0x21);
            break;
            case Colors::Indian_Red:
                Color(0x4E, 0x2F, 0x2F);
            break;
            case Colors::Khaki:
                Color(0x9F, 0x9F, 0x5F);
            break;
            case Colors::Light_Blue:
                Color(0xC0, 0xD9, 0xD9);
            break;
            case Colors::Light_Grey:
                Color(0xA8, 0xA8, 0xA8);
            break;
            case Colors::Light_Steel_Blue:
                Color(0x8F, 0x8F, 0xBD);
            break;
            case Colors::Light_Wood:
                Color(0xE9, 0xC2, 0xA6);
            break;
            case Colors::Lime_Green:
                Color(0x32, 0xCD, 0x32);
            break;
            case Colors::Magenta:
                Color(0xFF, 0x00, 0xFF);
            break;
            case Colors::Mandarian_Orange:
                Color(0xE4, 0x78, 0x33);
            break;
            case Colors::Maroon:
                Color(0x8E, 0x23, 0x6B);
            break;
            case Colors::Medium_Aquamarine:
                Color(0x32, 0xCD, 0x99);
            break;
            case Colors::Medium_Blue:
                Color(0x32, 0x32, 0xCD);
            break;
            case Colors::Medium_Forest_Green:
                Color(0x6B, 0x8E, 0x23);
            break;
            case Colors::Medium_Goldenrod:
                Color(0xEA, 0xEA, 0xAE);
            break;
            case Colors::Medium_Orchid:
                Color(0x93, 0x70, 0xDB);
            break;
            case Colors::Medium_Sea_Green:
                Color(0x42, 0x6F, 0x42);
            break;
            case Colors::Medium_Slate_Blue:
                Color(0x7F, 0x00, 0xFF);
            break;
            case Colors::Medium_Spring_Green:
                Color(0x7F, 0xFF, 0x00);
            break;
            case Colors::Medium_Turquoise:
                Color(0x70, 0xDB, 0xDB);
            break;
            case Colors::Medium_Violet_Red:
                Color(0xDB, 0x70, 0x93);
            break;
            case Colors::Medium_Wood:
                Color(0xA6, 0x80, 0x64);
            break;
            case Colors::Midnight_Blue:
                Color(0x2F, 0x2F, 0x4F);
            break;
            case Colors::Navy_Blue:
                Color(0x23, 0x23, 0x8E);
            break;
            case Colors::Neon_Blue:
                Color(0x4D, 0x4D, 0xFF);
            break;
            case Colors::Neon_Pink:
                Color(0xFF, 0x6E, 0xC7);
            break;
            case Colors::New_Midnight_Blue:
                Color(0x00, 0x00, 0x9C);
            break;
            case Colors::New_Tan:
                Color(0xEB, 0xC7, 0x9E);
            break;
            case Colors::Old_Gold:
                Color(0xCF, 0xB5, 0x3B);
            break;
            case Colors::Orange:
                Color(0xFF, 0x7F, 0x00);
            break;
            case Colors::Orange_Red:
                Color(0xFF, 0x24, 0x00);
            break;
            case Colors::Orchid:
                Color(0xDB, 0x70, 0xDB);
            break;
            case Colors::Pale_Green:
                Color(0x8F, 0xBC, 0x8F);
            break;
            case Colors::Pink:
                Color(0xBC, 0x8F, 0x8F);
            break;
            case Colors::Plum:
                Color(0xEA, 0xAD, 0xEA);
            break;
            case Colors::Quartz:
                Color(0xD9, 0xD9, 0xF3);
            break;
            case Colors::Red:
                Color(0xFF, 0x00, 0x00);
            break;
            case Colors::Rich_Blue:
                Color(0x59, 0x59, 0xAB);
            break;
            case Colors::Salmon:
                Color(0x6F, 0x42, 0x42);
            break;
            case Colors::Scarlet:
                Color(0x8C, 0x17, 0x17);
            break;
            case Colors::Sea_Green:
                Color(0x23, 0x8E, 0x68);
            break;
            case Colors::Semi_Sweet_Chocolate:
                Color(0x6B, 0x42, 0x26);
            break;
            case Colors::Sienna:
                Color(0x8E, 0x6B, 0x23);
            break;
            case Colors::Silver:
                Color(0xE6, 0xE8, 0xFA);
            break;
            case Colors::Sky_Blue:
                Color(0x32, 0x99, 0xCC);
            break;
            case Colors::Slate_Blue:
                Color(0x00, 0x7F, 0xFF);
            break;
            case Colors::Spicy_Pink:
                Color(0xFF, 0x1C, 0xAE);
            break;
            case Colors::Spring_Green:
                Color(0x00, 0xFF, 0x7F);
            break;
            case Colors::Steel_Blue:
                Color(0x23, 0x6B, 0x8E);
            break;
            case Colors::Summer_Sky:
                Color(0x38, 0xB0, 0xDE);
            break;
            case Colors::Tan:
                Color(0xDB, 0x93, 0x70);
            break;
            case Colors::Thistle:
                Color(0xD8, 0xBF, 0xD8);
            break;
            case Colors::Turquoise:
                Color(0xAD, 0xEA, 0xEA);
            break;
            case Colors::Very_Dark_Brown:
                Color(0x5C, 0x40, 0x33);
            break;
            case Colors::Very_Light_Grey:
                Color(0xCD, 0xCD, 0xCD);
            break;
            case Colors::Violet:
                Color(0x4F, 0x2F, 0x4F);
            break;
            case Colors::Violet_Red:
                Color(0xCC, 0x32, 0x99);
            break;
            case Colors::Wheat:
                Color(0xD8, 0xD8, 0xBF);
            break;
            case Colors::White:
                Color(0xFF, 0xFF, 0xFF);
            break;
            case Colors::Yellow:
                Color(0xFF, 0xFF, 0x00);
            break;
            case Colors::Yellow_Green:
                Color(0x99, 0xCC, 0x32);
            break;
            case Colors::Dark_Yellow:
                Color(0x99, 0xCC, 0x32);
            break;
    }
}

uint32_t Color::color() const {
    return p_color;
}

uint8_t Color::red() const {
    return (p_color >> 24) & 0xFF;
}

uint8_t Color::green() const {
    return (p_color >> 16) & 0xFF;
}

uint8_t Color::blue() const {
    return (p_color >> 8) & 0xFF;
}

uint8_t Color::alpha() const {
    return p_color & 0xFF;
}

void Color::set_red(uint8_t color) {
    p_color = (color << 24) | (p_color & 0x00FFFFFF);
}

void Color::set_green(uint8_t color) {
    p_color = (color << 16) | (p_color & 0xFF00FFFF);
}

void Color::set_blue(uint8_t color) {
    p_color = (color << 8) | (p_color & 0xFFFF00FF);
}

void Color::set_alpha(uint8_t color) {
    p_color = color | (p_color & 0xFFFFFF00);
}

std::string Color::to_hex() const {
    char buffer[11];
    snprintf(buffer, 11, "0x%08X", p_color);
    return std::string(buffer);
}

bool Color::operator==(const Color &color) const {
    return p_color == color.p_color;
}