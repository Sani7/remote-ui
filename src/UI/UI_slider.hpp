#pragma once
#include "UI_item.hpp"

#define UI_SLIDER_TYPE std::string("UISlider")
typedef void(*UI_slider_on_change)(std::string, double value);

class UI_slider : public UI_item
{
    public:
        UI_slider(std::string id, std::string text, Color fg_color, Color bg_color, double min, double max, double value, UI_slider_on_change on_change);
        UI_slider(const json& j);

        void set_value(double value);
        double value() const;

        void from_json(const json& j) override;
        json to_json() const override;
    private:
        double p_min;
        double p_max;
        double p_value;

        UI_slider_on_change p_on_change;
};