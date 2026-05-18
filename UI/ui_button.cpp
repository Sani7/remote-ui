#include "ui_button.hpp"

UI_button::UI_button(QString text, Color fg_color, Color bg_color, QObject *parent)
    : UI_item(UI_typeGadget::UI_type::ui_button, parent)
{
    m_ui_item.setLabel(UI_label_m());
    m_ui_item.label().setText(text);
    m_ui_item.label().mutColor().setFgColor(fg_color.color());
    m_ui_item.label().mutColor().setBgColor(bg_color.color());
}

UI_button::UI_button(QObject *parent) : UI_item(UI_typeGadget::UI_type::ui_button, parent)
{
    m_ui_item.setLabel(UI_label_m());
}

void UI_button::set_text(QString text)
{
    m_ui_item.label().setText(text);
}

QString UI_button::text() const
{
    return m_ui_item.label().text();
}

void UI_button::set_fg_color(Color color)
{
    m_ui_item.label().mutColor().setFgColor(color.color());
}

Color UI_button::fg_color() const
{
    return Color(m_ui_item.label().color().fgColor());
}

void UI_button::set_bg_color(Color color)
{
    m_ui_item.label().mutColor().setBgColor(color.color());
}

Color UI_button::bg_color() const
{
    return Color(m_ui_item.label().color().bgColor());
}

void UI_button::click()
{
    emit clicked();
}
