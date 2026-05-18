#include "ui_textbox.hpp"

UI_textbox::UI_textbox(QString hint, Color fg_color, Color bg_color, QObject *parent)
    : UI_item(UI_typeGadget::UI_type::ui_textbox, parent)
{
    m_ui_item.setTextbox(UI_textbox_m());
    m_ui_item.textbox().setHint(hint);
    m_ui_item.textbox().mutColor().setFgColor(fg_color.color());
    m_ui_item.textbox().mutColor().setBgColor(bg_color.color());
}

UI_textbox::UI_textbox(QObject *parent) : UI_item(UI_typeGadget::UI_type::ui_textbox, parent)
{
    m_ui_item.setTextbox(UI_textbox_m());
}

void UI_textbox::set_text(QString text)
{
    if (text == m_ui_item.textbox().text())
        return;
    m_ui_item.textbox().setText(text);
    emit ui_changed();
    emit changed(m_ui_item.textbox().text());
}

QString UI_textbox::text() const
{
    return m_ui_item.label().text();
}

void UI_textbox::set_fg_color(Color color)
{
    m_ui_item.label().mutColor().setFgColor(color.color());
}

Color UI_textbox::fg_color() const
{
    return Color(m_ui_item.label().color().fgColor());
}

void UI_textbox::set_bg_color(Color color)
{
    m_ui_item.label().mutColor().setBgColor(color.color());
}

Color UI_textbox::bg_color() const
{
    return Color(m_ui_item.label().color().bgColor());
}

void UI_textbox::set_hint(QString hint)
{
    m_ui_item.textbox().setHint(hint);
    emit ui_changed();
}

QString UI_textbox::hint() const &
{
    return m_ui_item.textbox().hint();
}

