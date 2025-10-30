#include "ui_item.hpp"

UI_item::UI_item(std::string type, std::string text, Color fg_color, Color bg_color, QObject *parent)
    : QObject(parent), m_id((size_t)-1), m_type(type), m_text(text), m_fg_color(fg_color), m_bg_color(bg_color),
      m_enabled(true), m_visible(true)
{
}

UI_item::UI_item(std::string type, QObject *parent)
    : QObject(parent), m_id((size_t)-1), m_type(type), m_text(""), m_fg_color(Color::Default),
      m_bg_color(Color::Default), m_enabled(true), m_visible(true)
{
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
        emit ui_changed();
    }
}

void UI_item::append_text(std::string text)
{
    m_text.append(text);
    emit ui_changed();
}

std::string UI_item::text() const
{
    return m_text;
}

void UI_item::set_fg_color(Color fg_color)
{
    if (this->m_fg_color != fg_color)
    {
        this->m_fg_color = fg_color;
        emit ui_changed();
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
        emit ui_changed();
    }
}

Color UI_item::bg_color() const
{
    return m_bg_color;
}

void UI_item::set_enabled(bool enabled)
{
    if (this->m_enabled != enabled)
    {
        this->m_enabled = enabled;
        emit ui_changed();
    }
}

bool UI_item::is_enabled() const
{
    return m_enabled;
}

void UI_item::set_visible(bool visible)
{
    if (this->m_visible != visible)
    {
        this->m_visible = visible;
        emit ui_changed();
    }
}

bool UI_item::is_visible() const
{
    return m_visible;
}

void UI_item::from_json(const json &j)
{
    if (!is_type(j))
        return;
    j.at("id").get_to(m_id);
    if (j.contains("text"))
        j.at("text").get_to(m_text);
    std::string color;
    if (j.contains("fg_color"))
    {
        j.at("fg_color").get_to(color);
        m_fg_color = Color(color);
        color.clear();
    }
    if (j.contains("bg_color"))
    {
        j.at("bg_color").get_to(color);
        m_bg_color = Color(color);
    }
    j.at("enabled").get_to(m_enabled);
    j.at("visible").get_to(m_visible);
}

json UI_item::to_json(size_t id) const
{
    json j;
    j["id"] = id;
    j["type"] = m_type;
    if (m_text_enabled)
        j["text"] = m_text;
    if (m_fg_color_enabled)
        j["fg_color"] = m_fg_color.to_hex();
    if (m_bg_color_enabled)
        j["bg_color"] = m_bg_color.to_hex();
    j["enabled"] = m_enabled;
    j["visible"] = m_visible;
    return j;
}

void UI_item::click() {};

void UI_item::set_selected(size_t selected)
{
    UNUSED(selected);
};
void UI_item::set_value(double value)
{
    UNUSED(value);
};

void UI_item::can_send(uint32_t id, uint8_t dlc, std::array<uint8_t, 8> payload)
{
    UNUSED(id);
    UNUSED(dlc);
    UNUSED(payload);
};

void UI_item::clear()
{
}

void UI_item::setup_item(bool text, bool fg_color, bool bg_color)
{
    m_text_enabled = text;
    m_fg_color_enabled = fg_color;
    m_bg_color_enabled = bg_color;
}
