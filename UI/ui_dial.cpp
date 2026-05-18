#include "ui_dial.hpp"

UI_dial::UI_dial(QString text, QString unit, double min, double max,
                 double value, QObject *parent)
    : UI_item(UI_typeGadget::UI_type::ui_dial, parent)
{
    m_ui_item.setRange(UI_range_m());
    m_ui_item.range().setText(text);
    m_ui_item.range().setUnit(unit);
    m_ui_item.range().setMin(min);
    m_ui_item.range().setMax(max);
    m_ui_item.range().setValue(value);
}

UI_dial::UI_dial(QString text, double min, double max, double value,
                 QObject *parent)
    : UI_dial(text, "", min, max, value, parent)
{
}

UI_dial::UI_dial(QObject *parent) : UI_item(UI_typeGadget::UI_type::ui_dial, parent)
{
}

void UI_dial::set_value(double value)
{
    if (value < m_ui_item.range().min() || value > m_ui_item.range().max())
        return;
    if (value == m_ui_item.range().value())
        return;

    m_ui_item.range().setValue(value);
    emit ui_changed();
}

double UI_dial::value() const
{
    return m_ui_item.range().value();
}

double UI_dial::min() const
{
    return m_ui_item.range().min();
}

double UI_dial::max() const
{
    return m_ui_item.range().max();
}

QString UI_dial::unit() const &
{
    return m_ui_item.range().unit();
}
