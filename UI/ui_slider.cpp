#include "ui_slider.hpp"

UI_slider::UI_slider(std::string text, std::string unit, Color fg_color, Color bg_color, double min, double max,
                     double value, QObject *parent)
    : UI_item(UI_SLIDER_TYPE, text, fg_color, bg_color, parent), m_min(min), m_max(max), m_value(value), m_unit(unit)
{
}

UI_slider::UI_slider(std::string text, std::string unit, double min, double max, double value, QObject *parent)
    : UI_slider(text, unit, Color::Default, Color::Default, min, max, value, parent)
{
}

UI_slider::UI_slider(std::string text, Color fg_color, Color bg_color, double min, double max, double value,
                     QObject *parent)
    : UI_slider(text, "", fg_color, bg_color, min, max, value, parent)
{
}

UI_slider::UI_slider(std::string text, double min, double max, double value, QObject *parent)
    : UI_slider(text, "", Color::Default, Color::Default, min, max, value, parent)
{
}

UI_slider::UI_slider(QObject *parent) : UI_item(UI_SLIDER_TYPE, parent)
{
}

void UI_slider::set_value(double value)
{
    if (value < m_min || value > m_max)
        return;
    if (value == m_value)
        return;

    this->m_value = value;
    emit ui_changed();
    emit changed(m_value);
}

double UI_slider::value() const
{
    return m_value;
}

double UI_slider::min() const
{
    return m_min;
}

double UI_slider::max() const
{
    return m_max;
}

void UI_slider::from_json(const json &j)
{
    UI_item::from_json(j);

    this->m_min = j.at("min");
    this->m_max = j.at("max");
    this->m_value = j.at("value");
    this->m_unit = j.at("unit");
}

json UI_slider::to_json(size_t id) const
{
    json j = UI_item::to_json(id);
    j["min"] = this->m_min;
    j["max"] = this->m_max;
    j["value"] = this->m_value;
    j["unit"] = this->m_unit;
    return j;
}
