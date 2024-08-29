#pragma once
#include "UI_item.hpp"

#define UI_DIAL_TYPE std::string("UI_dial")

class UI_dial : public UI_item
{
  public:
    UI_dial(std::string id, std::string text, Color fg_color, Color bg_color, double min, double max, double value);
    UI_dial(const json &j);

    void set_value(double value) override;
    double value() const;

    double min() const;
    double max() const;

    void from_json(const json &j) override;
    json to_json() const override;

  private:
    double m_min;
    double m_max;
    double m_value;
};