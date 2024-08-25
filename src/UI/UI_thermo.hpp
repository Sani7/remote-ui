#pragma once
#include "UI_item.hpp"

#define UI_THERMO_TYPE std::string("UI_thermo")

// TODO: Add color scale
class UI_thermo : public UI_item
{
    public:
        UI_thermo(std::string id, std::string text, Color fg_color, Color bg_color, double min, double max, double value);
        UI_thermo(const json& j);

        void set_value(double value) override;
        double value() const;

        void from_json(const json& j) override;
        json to_json() const override;
    private:
        double m_min;
        double m_max;
        double m_value;
};