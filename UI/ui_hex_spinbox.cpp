#include "ui_hex_spinbox.hpp"

UI_hex_spinbox::UI_hex_spinbox(QObject *parent) : UI_item(UI_typeGadget::UI_type::ui_hex_spinbox, parent)
{
    m_ui_item.setSpinbox(UI_spinbox_m());
}

UI_hex_spinbox::UI_hex_spinbox(double min, double max, double value, QObject *parent)
    : UI_item(UI_typeGadget::UI_type::ui_hex_spinbox, parent)
{
    m_ui_item.setSpinbox(UI_spinbox_m());
    m_ui_item.spinbox().setMin(min);
    m_ui_item.spinbox().setMax(max);
    m_ui_item.spinbox().setValue(value);
}

void UI_hex_spinbox::set_value(double value)
{
    if (value < m_ui_item.spinbox().min() || value > m_ui_item.spinbox().max())
        return;
    if (value == m_ui_item.spinbox().value())
        return;

    m_ui_item.spinbox().setValue(value);
    emit ui_changed();
    emit changed(m_ui_item.spinbox().value());
}
double UI_hex_spinbox::value() const
{
    return m_ui_item.spinbox().value();
}

double UI_hex_spinbox::min() const
{
    return m_ui_item.spinbox().min();
}
double UI_hex_spinbox::max() const
{
    return m_ui_item.spinbox().max();
}
