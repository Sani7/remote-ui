#include "UI_item.hpp"

UI_item::UI_item(std::string id, std::string type,  std::string text, uint8_t text_size, Color fg_color, Color bg_color)
    : p_id(id), p_type(type), p_text(text), p_text_size(text_size), p_fg_color(fg_color), p_bg_color(bg_color)
{
}

UI_item::UI_item(std::string type)
    : p_id(""), p_type(type), p_text(""), p_text_size(12), p_fg_color(Color::Black), p_bg_color(Color::White)
{
}

UI_item::UI_item(const json& j)
{
    from_json(j);
}

std::string UI_item::id() const
{
    return p_id;
}

std::string UI_item::type() const
{
    return p_type;
}

bool UI_item::is_type(json j)
{
    if (!j.contains("type"))
    {
        return false;
    }
    return j.at("type") == p_type;
}

void UI_item::set_text(std::string text)
{
    if (this->p_text != text)
    {
        this->p_text = text;
    }
}

std::string UI_item::text() const
{
    return p_text;
}

void UI_item::set_text_size(uint8_t text_size)
{
    if (this->p_text_size != text_size)
    {
        this->p_text_size = text_size;
    }
}

uint8_t UI_item::text_size() const
{
    return p_text_size;
}

void UI_item::set_fg_color(Color fg_color)
{
    if (this->p_fg_color != fg_color)
    {
        this->p_fg_color = fg_color;
    }
}

Color UI_item::fg_color() const
{
    return p_fg_color;
}

void UI_item::set_bg_color(Color bg_color)
{
    if (this->p_bg_color != bg_color)
    {
        this->p_bg_color = bg_color;
    }
}

Color UI_item::bg_color() const
{
    return p_bg_color;
}


void UI_item::from_json(const json& j)
{
    if (!is_type(j))
        return;
    j.at("id").get_to(p_id);
    j.at("text").get_to(p_text);
    j.at("text_size").get_to(p_text_size);
    std::string color;
    j.at("fg_color").get_to(color);
    p_fg_color = Color(color);
    color.clear();
    j.at("bg_color").get_to(color);
    p_bg_color = Color(color);
}

json UI_item::to_json() const
{
    json j;
    j["id"] = p_id;
    j["type"] = p_type;
    j["text"] = p_text;
    j["text_size"] = p_text_size;
    j["fg_color"] = p_fg_color.to_hex();
    j["bg_color"] = p_bg_color.to_hex();
    return j;
}