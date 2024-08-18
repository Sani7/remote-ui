#include "UI_slider.hpp"

UI_slider::UI_slider(std::string id, std::string text, Color fg_color, Color bg_color, double min, double max, double value,  std::function<void(std::string, double)> on_change)
    : UI_item(id, UI_SLIDER_TYPE, text, 12, fg_color, bg_color), p_min(min), p_max(max), p_value(value)
{
    this->p_on_change = on_change;
}

UI_slider::UI_slider(const json& j)
: UI_item(UI_SLIDER_TYPE)
{
    from_json(j);
}

void UI_slider::set_value(double value)
{
    if (value < p_min || value > p_max)
        return;
    if (value == p_value)
        return;

    if (p_on_change != nullptr)
    {
        this->p_value = value;
        p_on_change(p_id, value);
    }
}

double UI_slider::value() const
{
    return p_value;
}

void UI_slider::from_json(const json& j)
{
    UI_item::from_json(j);

    this->p_min = j["min"];
    this->p_max = j["max"];
    this->p_value = j["value"];
}

json UI_slider::to_json() const
{
    json j = UI_item::to_json();
    j["min"] = this->p_min;
    j["max"] = this->p_max;
    j["value"] = this->p_value;
    return j;
}