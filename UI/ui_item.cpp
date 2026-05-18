#include "ui_item.hpp"

UI_item::UI_item(UI_typeGadget::UI_type type, QObject *parent)
    : QObject(parent)
{
    m_ui_item.setId_proto((size_t)-1);
    m_ui_item.setType(type);
    m_ui_item.setEnabled(true);
    m_ui_item.setVisible(true);
}

void UI_item::set_id(size_t id)
{
    m_ui_item.setId_proto(id);
}

void UI_item::set_enabled(bool enabled)
{
    if (m_ui_item.enabled() != enabled)
    {
        m_ui_item.setEnabled(enabled);
        emit ui_changed();
    }
}

bool UI_item::is_enabled() const
{
    return m_ui_item.enabled();
}

void UI_item::set_visible(bool visible)
{
    if (m_ui_item.visible() != visible)
    {
        m_ui_item.setVisible(visible);
        emit ui_changed();
    }
}

bool UI_item::is_visible() const
{
    return m_ui_item.visible();
}

UI_item_m UI_item::to_proto() const
{
    return m_ui_item;
}

void UI_item::from_proto(UI_item_m item)
{
    m_ui_item = item;
}

void UI_item::click() {};

void UI_item::set_selected(qsizetype selected)
{
    Q_UNUSED(selected);
}

void UI_item::set_text(QString text)
{
    Q_UNUSED(text);
}

void UI_item::set_value(double value)
{
    Q_UNUSED(value);
}

void UI_item::can_send(uint32_t id, uint64_t dlc, QByteArray payload)
{
    Q_UNUSED(id);
    Q_UNUSED(dlc);
    Q_UNUSED(payload);
}

void UI_item::clear()
{
}
