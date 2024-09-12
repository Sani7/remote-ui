#pragma once
#include "UI_item.hpp"

#define UI_SLIDER_TYPE std::string("UI_slider")

class UI_slider : public UI_item
{
    Q_OBJECT
  public:
    UI_slider(std::string id, std::string text, Color fg_color, Color bg_color, double min, double max, double value, QObject* parrent = nullptr);
    UI_slider(const json &j, QObject* parrent = nullptr);

    void set_value(double value) override;
    double value() const;

    double min() const;
    double max() const;

    void from_json(const json &j) override;
    json to_json() const override;
  signals:
    void on_change(std::string id, double value);

  private:
    double m_min;
    double m_max;
    double m_value;
};
