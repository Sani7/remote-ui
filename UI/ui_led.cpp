#include "ui_led.hpp"

UI_led::UI_led(QObject *parent) : UI_item(UI_typeGadget::UI_type::ui_led, parent)
{
    m_ui_item.setLabel(UI_label_m());
}

UI_led::UI_led(QString text, Color fg_color, Color bg_color, QObject *parent)
    : UI_item(UI_typeGadget::UI_type::ui_led, parent)
{
    m_ui_item.setLabel(UI_label_m());
    m_ui_item.label().setText(text);
    m_ui_item.label().mutColor().setFgColor(fg_color.color());
    m_ui_item.label().mutColor().setBgColor(bg_color.color());
}

void UI_led::set_text(QString text)
{
    m_ui_item.label().setText(text);
}

QString UI_led::text() const
{
    return m_ui_item.label().text();
}

void UI_led::set_fg_color(Color color)
{
    m_ui_item.label().mutColor().setFgColor(color.color());
}

Color UI_led::fg_color() const
{
    return Color(m_ui_item.label().color().fgColor());
}

void UI_led::set_bg_color(Color color)
{
    m_ui_item.label().mutColor().setBgColor(color.color());
}

Color UI_led::bg_color() const
{
    return Color(m_ui_item.label().color().bgColor());
}

bool UI_led::set_led(bool state, Color on, Color off)
{
    set_bg_color(state ? on : off);
    return state;
}
