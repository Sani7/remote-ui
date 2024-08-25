#include "UI_thermo.hpp"

UI_thermo::UI_thermo(std::string id, std::string text, Color fg_color, Color bg_color, double min, double max, double value)
    : UI_item(id, UI_THERMO_TYPE, text, 12, fg_color, bg_color), m_min(min), m_max(max), m_value(value)
{
}

UI_thermo::UI_thermo(const json& j)
: UI_item(UI_THERMO_TYPE)
{
    from_json(j);
}

void UI_thermo::set_value(double value)
{
    if (value < m_min || value > m_max)
        return;
    if (value == m_value)
        return;

    this->m_value = value;
}

double UI_thermo::value() const
{
    return m_value;
}

void UI_thermo::from_json(const json& j)
{
    UI_item::from_json(j);

    this->m_min = j["min"];
    this->m_max = j["max"];
    this->m_value = j["value"];
}

json UI_thermo::to_json() const
{
    json j = UI_item::to_json();
    j["min"] = this->m_min;
    j["max"] = this->m_max;
    j["value"] = this->m_value;
    return j;
}