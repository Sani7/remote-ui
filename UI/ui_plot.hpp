#pragma once
#include "ui_item.hpp"
#define UI_PLOT_TYPE std::string("ui_plot")

class UI_plot : public UI_item
{
    Q_OBJECT
  public:
    explicit UI_plot(QObject *parent = nullptr);
    UI_plot(std::string text, std::string x_label, std::string y_label, QObject *parent = nullptr);

    void set_x_label(std::string name);
    std::string x_label() const;
    void set_y_label(std::string name);
    std::string y_label() const;
    void add_value(double y);
    void add_value(double x, double y);
    const std::vector<double> &x_vals() const;
    const std::vector<double> &y_vals() const;
    void clean_vals();

    void from_json(const json &j) override;
    json to_json(size_t id) const override;

  private:
    std::vector<double> m_x_vals;
    std::vector<double> m_y_vals;
    std::string m_x_label;
    std::string m_y_label;
};
