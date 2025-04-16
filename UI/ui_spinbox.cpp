#include "ui_spinbox.hpp"

UI_spinbox::UI_spinbox(QObject *parent) : UI_item(UI_SPINBOX_TYPE, parent)
{
}

UI_spinbox::UI_spinbox(double min, double max, double value, QObject *parent) : UI_item(UI_SPINBOX_TYPE, parent)
{
    m_min = min;
    m_max = max;
    m_value = value;
}

void UI_spinbox::set_value(double value)
{
    if (value < m_min || value > m_max)
        return;
    if (value == m_value)
        return;

    this->m_value = value;
    emit ui_changed();
    emit changed(m_value);
}
double UI_spinbox::value() const
{
    return m_value;
}

double UI_spinbox::min() const
{
    return m_min;
}
double UI_spinbox::max() const
{
    return m_max;
}

void UI_spinbox::from_json(const json &j)
{
    UI_item::from_json(j);

    this->m_min = j.at("min");
    this->m_max = j.at("max");
    this->m_value = j.at("value");
}

json UI_spinbox::to_json(size_t id) const
{
    json j = UI_item::to_json(id);
    j["min"] = this->m_min;
    j["max"] = this->m_max;
    j["value"] = this->m_value;
    return j;
}
