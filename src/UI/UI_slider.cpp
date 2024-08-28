#include "UI_slider.hpp"

UI_slider::UI_slider(std::string id, std::string text, Color fg_color, Color bg_color, double min, double max, double value,  std::function<void(std::string, double)> on_change)
    : UI_item(id, UI_SLIDER_TYPE, text, 12, fg_color, bg_color), m_min(min), m_max(max), m_value(value)
{
    this->m_on_change = on_change;
}

UI_slider::UI_slider(const json& j)
: UI_item(UI_SLIDER_TYPE)
{
    from_json(j);
}

void UI_slider::set_value(double value)
{
    if (value < m_min || value > m_max)
        return;
    if (value == m_value)
        return;

    this->m_value = value;
    if (m_on_change != nullptr)
    {
        std::thread([this] { m_on_change(m_id, m_value); }).detach();
    }
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

void UI_slider::from_json(const json& j)
{
    UI_item::from_json(j);

    this->m_min = j.at("min");
    this->m_max = j.at("max");
    this->m_value = j.at("value");
}

json UI_slider::to_json() const
{
    json j = UI_item::to_json();
    j["min"] = this->m_min;
    j["max"] = this->m_max;
    j["value"] = this->m_value;
    return j;
}