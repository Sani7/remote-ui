#include "UI_thermo.hpp"

UI_thermo::UI_thermo(std::string id, std::string text, Color fg_color, Color bg_color, double min, double max,
                     double value, QObject* parrent)
    : UI_item(id, UI_THERMO_TYPE, text, 12, fg_color, bg_color, parrent), m_min(min), m_max(max), m_value(value)
{
}

UI_thermo::UI_thermo(const json &j, QObject* parrent) : UI_item(UI_THERMO_TYPE, parrent)
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
    emit value_changed();
}

double UI_thermo::value() const
{
    return m_value;
}

double UI_thermo::min() const
{
    return m_min;
}

double UI_thermo::max() const
{
    return m_max;
}

void UI_thermo::set_start_color(Color color)
{
    this->m_color_map[0] = color;
    emit value_changed();
}

void UI_thermo::set_end_color(Color color)
{
    this->m_color_map[1] = color;
    emit value_changed();
}

void UI_thermo::add_color_stop(double value, Color color)
{
    if (value <= 0 || value >= 1)
        return;

    this->m_color_map[value] = color;
    emit value_changed();
}

void UI_thermo::add_color_stop_normalized(double value, Color color)
{
    if (value < m_min || value > m_max)
        return;

    double normalized = (value - m_min) / (m_max - m_min);
    this->m_color_map[normalized] = color;
    emit value_changed();
}

void UI_thermo::remove_color_stop(double value)
{
    if (value <= 0 || value >= 1)
        return;

    this->m_color_map.erase(value);
    emit value_changed();
}

void UI_thermo::remove_color_stop_normalized(double value)
{
    if (value < m_min || value > m_max)
        return;

    double normalized = (value - m_min) / (m_max - m_min);
    this->m_color_map.erase(normalized);
    emit value_changed();
}

void UI_thermo::from_json(const json &j)
{
    UI_item::from_json(j);

    this->m_min = j.at("min");
    this->m_max = j.at("max");
    this->m_value = j.at("value");
    this->m_color_map.clear();
    for (auto &i : j["color_map"])
    {
        double k = i.at("key");
        std::string c = i.at("value");
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
    for (auto &[key, value] : this->m_color_map)
    {
        json item = json::object();
        item["key"] = key;
        item["value"] = value.to_hex();
        j["color_map"].push_back(item);
    }
    return j;
}
