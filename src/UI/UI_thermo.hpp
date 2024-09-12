#pragma once
#include "UI_item.hpp"

#define UI_THERMO_TYPE std::string("UI_thermo")

// TODO: Add color scale
class UI_thermo : public UI_item
{
    Q_OBJECT
  public:
    UI_thermo(std::string id, std::string text, Color fg_color, Color bg_color, double min, double max, double value, QObject* parrent = nullptr);
    UI_thermo(const json &j, QObject* parrent = nullptr);

    void set_value(double value) override;
    double value() const;

    double min() const;
    double max() const;

    void set_start_color(Color color);
    void set_end_color(Color color);
    void add_color_stop(double value, Color color);
    void add_color_stop_normalized(double value, Color color);
    void remove_color_stop(double value);
    void remove_color_stop_normalized(double value);

    void from_json(const json &j) override;
    json to_json() const override;

  private:
    double m_min;
    double m_max;
    double m_value;
    std::unordered_map<double, Color> m_color_map;
};
