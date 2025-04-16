#ifndef UI_PLOT_H
#define UI_PLOT_H

#include "ui_item.hpp"
#define UI_PLOT_TYPE std::string("ui_plot")

class UI_plot : public UI_item
{
    Q_OBJECT
  public:
    explicit UI_plot(QObject *parent = nullptr);
    UI_plot(std::string text, std::string x_label, std::string y_label, QObject *parent = nullptr);
    UI_plot(const json &j, QObject *parent = nullptr);

    void set_x_label(std::string name);
    std::string x_label() const;
    void set_y_label(std::string name);
    std::string y_label() const;
    void set_x_label_enabled(bool enabled);
    bool x_label_enabled() const;
    void set_y_label_enabled(bool enabled);
    bool y_label_enabled() const;
    void add_value(double y);
    void add_value(double x, double y);
    void clean_vals();

    void from_json(const json &j) override;
    json to_json(size_t id) const override;

  private:
    std::vector<double> m_x_vals;
    std::vector<double> m_y_vals;
    std::string m_x_label;
    std::string m_y_label;
    bool m_x_label_enabled;
    bool m_y_label_enabled;
};

#endif // UI_PLOT_H
