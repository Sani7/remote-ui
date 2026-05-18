#include "ui_label.hpp"

UI_label::UI_label(QString text, Color fg_color, Color bg_color, QObject *parent)
    : UI_item(UI_typeGadget::UI_type::ui_label, parent)
{
    m_ui_item.setLabel(UI_label_m());
    m_ui_item.label().setText(text);
    m_ui_item.label().mutColor().setFgColor(fg_color.color());
    m_ui_item.label().mutColor().setBgColor(bg_color.color());
}

UI_label::UI_label(QObject *parent) : UI_item(UI_typeGadget::UI_type::ui_label, parent)
{
    m_ui_item.setLabel(UI_label_m());
}

void UI_label::set_text(QString text)
{
    m_ui_item.label().setText(text);
}

void UI_label::append_text(QString text)
{
    QString prev = m_ui_item.label().text();
    prev.append(text);
    m_ui_item.label().setText(prev);
}

QString UI_label::text() const
{
    return m_ui_item.label().text();
}

void UI_label::set_fg_color(Color color)
{
    m_ui_item.label().mutColor().setFgColor(color.color());
}

Color UI_label::fg_color() const
{
    return Color(m_ui_item.label().color().fgColor());
}

void UI_label::set_bg_color(Color color)
{
    m_ui_item.label().mutColor().setBgColor(color.color());
}

Color UI_label::bg_color() const
{
    return Color(m_ui_item.label().color().bgColor());
}