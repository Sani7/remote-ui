#ifndef UI_DOUBLE_SPINBOX_H
#define UI_DOUBLE_SPINBOX_H

#include "ui_item.hpp"

#define UI_DOUBLE_SPINBOX_TYPE std::string("ui_double_spinbox")

class UI_double_spinbox : public UI_item
{
    Q_OBJECT
  public:
    explicit UI_double_spinbox(QObject *parent = nullptr);
    UI_double_spinbox(double min, double max, double value, QObject *parent = nullptr);

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

#endif // UI_DOUBLE_SPINBOX_H
