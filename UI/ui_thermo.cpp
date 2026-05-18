#include "ui_thermo.hpp"

UI_thermo::UI_thermo(QString text, QString unit, double min, double max,
                     double value, QObject *parent)
    : UI_item(UI_typeGadget::UI_type::ui_thermo, parent)
{
    m_ui_item.setRange(UI_range_m());
    m_ui_item.range().setText(text);
    m_ui_item.range().setUnit(unit);
    m_ui_item.range().setMin(min);
    m_ui_item.range().setMax(max);
    m_ui_item.range().setValue(value);
}

UI_thermo::UI_thermo(QString text, double min, double max, double value,
                     QObject *parent)
    : UI_thermo(text, "", min, max, value, parent)
{
}

UI_thermo::UI_thermo(QObject *parent) : UI_item(UI_typeGadget::UI_type::ui_thermo, parent)
{
}

void UI_thermo::set_value(double value)
{
    if (value < m_ui_item.range().min() || value > m_ui_item.range().max())
        return;
    if (value == m_ui_item.range().value())
        return;

    m_ui_item.range().setValue(value);
    emit ui_changed();
}

double UI_thermo::value() const
{
    return m_ui_item.range().value();
}

double UI_thermo::min() const
{
    return m_ui_item.range().min();
}

double UI_thermo::max() const
{
    return m_ui_item.range().max();
}

void UI_thermo::set_start_color(Color color)
{
    Q_UNUSED(color);
    // this->m_color_map[0] = color;
    // emit ui_changed();
}

void UI_thermo::set_end_color(Color color)
{
    Q_UNUSED(color);
    // this->m_color_map[1] = color;
    // emit ui_changed();
}

void UI_thermo::add_color_stop(double value, Color color)
{
    Q_UNUSED(value);
    Q_UNUSED(color);
    // if (value <= 0 || value >= 1)
    //     return;

    // this->m_color_map[value] = color;
    // emit ui_changed();
}

void UI_thermo::add_color_stop_normalized(double value, Color color)
{
    Q_UNUSED(value);
    Q_UNUSED(color);
    // if (value < m_min || value > m_max)
    //     return;

    // double normalized = (value - m_min) / (m_max - m_min);
    // this->m_color_map[normalized] = color;
    // emit ui_changed();
}

void UI_thermo::remove_color_stop(double value)
{
    Q_UNUSED(value);
    // if (value <= 0 || value >= 1)
    //     return;

    // this->m_color_map.erase(value);
    // emit ui_changed();
}

void UI_thermo::remove_color_stop_normalized(double value)
{
    Q_UNUSED(value);
    // if (value < m_min || value > m_max)
    //     return;

    // double normalized = (value - m_min) / (m_max - m_min);
    // this->m_color_map.erase(normalized);
    // emit ui_changed();
}
