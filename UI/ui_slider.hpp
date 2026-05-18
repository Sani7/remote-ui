#pragma once
#include "ui_item.hpp"

/**
 * @brief Class representing a UI slider
 *
 */
class UI_slider : public UI_item
{
    Q_OBJECT
  public:
    /**
     * @brief Construct a UI slider object
     *
     * @param text The text to display
     * @param unit The unit of the slider
     * @param min The minimum value
     * @param max The maximum value
     * @param value The initial value
     * @param parent The parent QObject
     */
    UI_slider(QString text, QString unit, double min, double max, double value,
              QObject *parent = nullptr);
    /**
     * @brief Construct a UI slider object
     *
     * @param text The text to display
     * @param fg_color The foreground color
     * @param bg_color The background color
     * @param min The minimum value
     * @param max The maximum value
     * @param value The initial value
     * @param parent The parent QObject
     */
    UI_slider(QString text, double min, double max, double value,
              QObject *parent = nullptr);

    /**
     * @brief Construct a UI slider object
     *
     * @param parent The parent QObject
     */
    UI_slider(QObject *parent = nullptr);

    /**
     * @brief Set the value of the slider
     *
     * @param value The value to set
     */
    void set_value(double value) override;
    /**
     * @brief Get the value of the slider
     *
     * @return double The current value
     */
    double value() const;

    /**
     * @brief Get the minimum value of the slider
     *
     * @return double The minimum value
     */
    double min() const;
    /**
     * @brief Get the maximum value of the slider
     *
     * @return double The maximum value
     */
    double max() const;
  signals:
    /**
     * @brief Signal emitted when the slider value changes
     *
     * @param value The new value of the slider
     */
    void changed(double value);
};
