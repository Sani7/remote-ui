#ifndef UI_SPINBOX_HPP
#define UI_SPINBOX_HPP

#include "ui_item.hpp"

#define UI_SPINBOX_TYPE std::string("ui_spinbox")

class UI_spinbox : public UI_item
{
    Q_OBJECT
  public:
    explicit UI_spinbox(QObject *parent = nullptr);
    UI_spinbox(double min, double max, double value, QObject *parent = nullptr);

    void set_value(double value) override;
    double value() const;

    double min() const;
    double max() const;

    void from_json(const json &j) override;
    json to_json(size_t id) const override;
  signals:
    void changed(double value);

  private:
    double m_min;
    double m_max;
    double m_value;
};

#endif // UI_SPINBOX_HPP
