#include "ui_checkbox.hpp"

UI_checkbox::UI_checkbox(QObject *parent) : UI_item(UI_typeGadget::UI_type::ui_checkbox, parent)
{
    m_ui_item.setCheckbox(UI_checkbox_m());
}

UI_checkbox::UI_checkbox(QString text, Color fg_color, Color bg_color, QObject *parent)
    : UI_item(UI_typeGadget::UI_type::ui_checkbox, parent)
{
    m_ui_item.setCheckbox(UI_checkbox_m());
    m_ui_item.checkbox().setText(text);
    m_ui_item.checkbox().setChecked(false);
    m_ui_item.label().mutColor().setFgColor(fg_color.color());
    m_ui_item.label().mutColor().setBgColor(bg_color.color());
}

void UI_checkbox::set_text(QString text)
{
    m_ui_item.label().setText(text);
}

QString UI_checkbox::text() const
{
    return m_ui_item.label().text();
}

void UI_checkbox::set_fg_color(Color color)
{
    m_ui_item.label().mutColor().setFgColor(color.color());
}

Color UI_checkbox::fg_color() const
{
    return Color(m_ui_item.label().color().fgColor());
}

void UI_checkbox::set_bg_color(Color color)
{
    m_ui_item.label().mutColor().setBgColor(color.color());
}

Color UI_checkbox::bg_color() const
{
    return Color(m_ui_item.label().color().bgColor());
}

void UI_checkbox::click()
{
    m_ui_item.checkbox().setChecked(!m_ui_item.checkbox().checked());
    emit ui_changed();
    emit changed(m_ui_item.checkbox().checked());
}

void UI_checkbox::set_state(bool state)
{
    m_ui_item.checkbox().setChecked(state);
    emit ui_changed();
    emit changed(m_ui_item.checkbox().checked());
}

bool UI_checkbox::state() const
{
    return m_ui_item.checkbox().checked();
}
