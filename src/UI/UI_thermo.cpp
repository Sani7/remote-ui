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

void UI_thermo::set_start_color(Color color)
{
    this->m_color_map[0] = color;
}

void UI_thermo::set_end_color(Color color)
{
    this->m_color_map[1] = color;
}

void UI_thermo::add_color_stop(double value, Color color)
{
    if (value <= 0 || value >= 1)
        return;
    
    this->m_color_map[value] = color;
}

void UI_thermo::add_color_stop_normalized(double value, Color color)
{
    if (value < m_min || value > m_max)
        return;

    double normalized = (value - m_min) / (m_max - m_min);
    this->m_color_map[normalized] = color;
}

void UI_thermo::remove_color_stop(double value)
{
    if (value <= 0 || value >= 1)
        return;

    this->m_color_map.erase(value);
}

void UI_thermo::remove_color_stop_normalized(double value)
{
    if (value < m_min || value > m_max)
        return;

    double normalized = (value - m_min) / (m_max - m_min);
    this->m_color_map.erase(normalized);
}

void UI_thermo::from_json(const json& j)
{
    UI_item::from_json(j);

    this->m_min = j["min"];
    this->m_max = j["max"];
    this->m_value = j["value"];
    this->m_color_map.clear();
    for (auto& i : j["color_map"])
    {
        double k = i["key"];
        std::string c;
        i["value"].get_to(c);
        this->m_color_map[k] = Color(c);
    }
}

json UI_thermo::to_json() const
{
    json j = UI_item::to_json();
    j["min"] = this->m_min;
    j["max"] = this->m_max;
    j["value"] = this->m_value;
    j["color_map"] = json::array();
    for (auto& [key, value] : this->m_color_map)
    {
        json item = json::object();
        item["key"] = key;
        item["value"] = value.to_hex();
        j["color_map"].push_back(item);
    }
    return j;
}