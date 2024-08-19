#pragma once
#include "UI_item.hpp"

#define UI_SLIDER_TYPE std::string("UISlider")

class UI_slider : public UI_item
{
    public:
        UI_slider(std::string id, std::string text, Color fg_color, Color bg_color, double min, double max, double value, std::function<void(std::string, double)> on_change = nullptr);
        UI_slider(const json& j);

        void set_value(double value) override;
        double value() const;

        void from_json(const json& j) override;
        json to_json() const override;
    private:
        double p_min;
        double p_max;
        double p_value;

        std::function<void(std::string, double)> p_on_change;
};