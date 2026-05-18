#pragma once
#include "ui_item.hpp"

/**
 * @brief Class representing a UI double spinbox
 *
 */
class UI_double_spinbox : public UI_item
{
    Q_OBJECT
  public:
    /**
     * @brief Construct a UI double spinbox object
     *
     * @param parent The parent QObject
     */
    explicit UI_double_spinbox(QObject *parent = nullptr);
    /**
     * @brief Construct a UI double spinbox object
     *
     * @param min The minimum value
     * @param max The maximum value
     * @param value The initial value
     * @param parent The parent QObject
     */
    UI_double_spinbox(double min, double max, double value, QObject *parent = nullptr);

    /**
     * @brief Set the value of the double spinbox
     *
     * @param value The value to set
     */
    void set_value(double value) override;
    /**
     * @brief Get the value of the double spinbox
     *
     * @return double The current value
     */
    double value() const;

    /**
     * @brief Get the minimum value of the double spinbox
     *
     * @return double The minimum value
     */
    double min() const;
    /**
     * @brief Get the maximum value of the double spinbox
     *
     * @return double The maximum value
     */
    double max() const;
  signals:
    /**
     * @brief Signal emitted when the double spinbox value changes
     *
     * @param value The new value
     */
    void changed(double value);
};
