#pragma once
#include "ui_item.hpp"

#define UI_SLIDER_TYPE std::string("ui_slider")

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
     * @param fg_color The foreground color
     * @param bg_color The background color
     * @param min The minimum value
     * @param max The maximum value
     * @param value The initial value
     * @param parent The parent QObject
     */
    UI_slider(std::string text, std::string unit, Color fg_color, Color bg_color, double min, double max, double value,
              QObject *parent = nullptr);
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
    UI_slider(std::string text, std::string unit, double min, double max, double value, QObject *parent = nullptr);
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
    UI_slider(std::string text, Color fg_color, Color bg_color, double min, double max, double value,
              QObject *parent = nullptr);
    /**
     * @brief Construct a UI slider object
     *
     * @param text The text to display
     * @param min The minimum value
     * @param max The maximum value
     * @param value The initial value
     * @param parent The parent QObject
     */
    UI_slider(std::string text, double min, double max, double value, QObject *parent = nullptr);
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

    /**
     * @brief Deserialize the slider from JSON
     *
     * @param j The JSON object
     */
    void from_json(const json &j) override;
    /**
     * @brief Serialize the slider to JSON
     *
     * @param id The id of the slider
     * @return json The JSON representation of the slider
     */
    json to_json(size_t id) const override;
  signals:
    /**
     * @brief Signal emitted when the slider value changes
     *
     * @param value The new value of the slider
     */
    void changed(double value);

  private:
    double m_min;
    double m_max;
    double m_value;
    std::string m_unit;
};
