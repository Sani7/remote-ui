#include "ui_slider.hpp"

UI_slider::UI_slider(QString text, QString unit, double min, double max,
                     double value, QObject *parent)
    : UI_item(UI_typeGadget::UI_type::ui_slider, parent)
{
    m_ui_item.setRange(UI_range_m());
    m_ui_item.range().setText(text);
    m_ui_item.range().setUnit(unit);
    m_ui_item.range().setMax(max);
    m_ui_item.range().setMin(min);
    m_ui_item.range().setValue(value);
}

UI_slider::UI_slider(QString text, double min, double max, double value,
                     QObject *parent)
    : UI_slider(text, "", min, max, value, parent)
{
}

UI_slider::UI_slider(QObject *parent) : UI_item(UI_typeGadget::UI_type::ui_slider, parent)
{
    m_ui_item.setRange(UI_range_m());
}

void UI_slider::set_value(double value)
{
    if (value < m_ui_item.range().min() || value > m_ui_item.range().max())
        return;
    if (value == m_ui_item.range().value())
        return;

    m_ui_item.range().setValue(value);
    emit ui_changed();
    emit changed(m_ui_item.range().value());
}

double UI_slider::value() const
{
    return m_ui_item.range().value();
}

double UI_slider::min() const
{
    return m_ui_item.range().min();
}

double UI_slider::max() const
{
    return m_ui_item.range().max();
}
