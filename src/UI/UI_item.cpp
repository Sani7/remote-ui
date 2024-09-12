#include "UI_item.hpp"

UI_item::UI_item(std::string id, std::string type, std::string text, uint8_t text_size, Color fg_color, Color bg_color, QObject* parrent)
    : QObject(parrent),
      m_id(id), m_type(type), m_text(text), m_text_size(text_size), m_fg_color(fg_color), m_bg_color(bg_color),
      enabled(true), visible(true)
{
}

UI_item::UI_item(std::string type, QObject* parrent)
    : QObject(parrent),
      m_id(""), m_type(type), m_text(""), m_text_size(12), m_fg_color(Color::Black), m_bg_color(Color::White),
      enabled(true), visible(true)
{
}

UI_item::UI_item(const json &j, QObject* parrent) :
    QObject(parrent)
{
    from_json(j);
}

UI_item::~UI_item()
{
}

std::string UI_item::id() const
{
    return m_id;
}

std::string UI_item::type() const
{
    return m_type;
}

bool UI_item::is_type(json j)
{
    if (!j.contains("type"))
    {
        return false;
    }
    return j.at("type") == m_type;
}

void UI_item::set_text(std::string text)
{
    if (this->m_text != text)
    {
        this->m_text = text;
        emit value_changed();
    }
}

std::string UI_item::text() const
{
    return m_text;
}

void UI_item::set_text_size(uint8_t text_size)
{
    if (this->m_text_size != text_size)
    {
        this->m_text_size = text_size;
        emit value_changed();
    }
}

uint8_t UI_item::text_size() const
{
    return m_text_size;
}

void UI_item::set_fg_color(Color fg_color)
{
    if (this->m_fg_color != fg_color)
    {
        this->m_fg_color = fg_color;
        emit value_changed();
    }
}

Color UI_item::fg_color() const
{
    return m_fg_color;
}

void UI_item::set_bg_color(Color bg_color)
{
    if (this->m_bg_color != bg_color)
    {
        this->m_bg_color = bg_color;
        emit value_changed();
    }
}

Color UI_item::bg_color() const
{
    return m_bg_color;
}

void UI_item::set_enabled(bool enabled)
{
    if (this->enabled != enabled)
    {
        this->enabled = enabled;
        emit value_changed();
    }
}

bool UI_item::is_enabled() const
{
    return enabled;
}

void UI_item::set_visible(bool visible)
{
    if (this->visible != visible)
    {
        this->visible = visible;
        emit value_changed();
    }
}

bool UI_item::is_visible() const
{
    return visible;
}

void UI_item::from_json(const json &j)
{
    if (!is_type(j))
        return;
    j.at("id").get_to(m_id);
    j.at("text").get_to(m_text);
    j.at("text_size").get_to(m_text_size);
    std::string color;
    j.at("fg_color").get_to(color);
    m_fg_color = Color(color);
    color.clear();
    j.at("bg_color").get_to(color);
    m_bg_color = Color(color);
    j.at("enabled").get_to(enabled);
    j.at("visible").get_to(visible);
}

json UI_item::to_json() const
{
    json j;
    j["id"] = m_id;
    j["type"] = m_type;
    j["text"] = m_text;
    j["text_size"] = m_text_size;
    j["fg_color"] = m_fg_color.to_hex();
    j["bg_color"] = m_bg_color.to_hex();
    j["enabled"] = enabled;
    j["visible"] = visible;
    return j;
}

void UI_item::click() {};

void UI_item::set_selected(std::string selected)
{
    UNUSED(selected);
};
void UI_item::set_value(double value)
{
    UNUSED(value);
};
