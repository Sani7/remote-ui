#pragma once
#include "UI_item.hpp"

#define UI_DIAL_TYPE std::string("UI_dial")

class UI_dial : public UI_item
{
    Q_OBJECT
  public:
    UI_dial(std::string text, std::string unit, Color fg_color, Color bg_color, double min, double max, double value,
            QObject *parent = nullptr);
    UI_dial(std::string text, std::string unit, double min, double max, double value, QObject *parent = nullptr);
    UI_dial(std::string text, Color fg_color, Color bg_color, double min, double max, double value,
            QObject *parent = nullptr);
    UI_dial(std::string text, double min, double max, double value, QObject *parent = nullptr);
    UI_dial(const json &j, QObject *parent = nullptr);

    void set_value(double value) override;
    double value() const;

    double min() const;
    double max() const;

    void from_json(const json &j) override;
    json to_json(size_t id) const override;

  private:
    double m_min;
    double m_max;
    double m_value;
    std::string m_unit;
};
