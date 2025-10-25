#pragma once
#include "ui_item.hpp"

#define UI_THERMO_TYPE std::string("ui_thermo")

// TODO: Add color scale
/**
 * @brief Class representing a UI thermometer
 *
 */
class UI_thermo : public UI_item
{
    Q_OBJECT
  public:
    /**
     * @brief Construct a UI thermometer object
     *
     * @param text The text to display
     * @param unit The unit of the thermometer
     * @param fg_color The foreground color
     * @param bg_color The background color
     * @param min The minimum value
     * @param max The maximum value
     * @param value The initial value
     * @param parent The parent QObject
     */
    UI_thermo(std::string text, std::string unit, Color fg_color, Color bg_color, double min, double max, double value,
              QObject *parent = nullptr);
    /**
     * @brief Construct a UI thermometer object
     *
     * @param text The text to display
     * @param unit The unit of the thermometer
     * @param min The minimum value
     * @param max The maximum value
     * @param value The initial value
     * @param parent The parent QObject
     */
    UI_thermo(std::string text, std::string unit, double min, double max, double value, QObject *parent = nullptr);
    /**
     * @brief Construct a UI thermometer object
     *
     * @param text The text to display
     * @param fg_color The foreground color
     * @param bg_color The background color
     * @param min The minimum value
     * @param max The maximum value
     * @param value The initial value
     * @param parent The parent QObject
     */
    UI_thermo(std::string text, Color fg_color, Color bg_color, double min, double max, double value,
              QObject *parent = nullptr);
    /**
     * @brief Construct a UI thermometer object
     *
     * @param text The text to display
     * @param min The minimum value
     * @param max The maximum value
     * @param value The initial value
     * @param parent The parent QObject
     */
    UI_thermo(std::string text, double min, double max, double value, QObject *parent = nullptr);
    /**
     * @brief Construct a UI thermometer object
     *
     * @param parent The parent QObject
     */
    UI_thermo(QObject *parent = nullptr);

    /**
     * @brief Set the value of the thermometer
     *
     * @param value The value to set
     */
    void set_value(double value) override;
    /**
     * @brief Get the value of the thermometer
     *
     * @return double The current value
     */
    double value() const;

    /**
     * @brief Get the minimum value of the thermometer
     *
     * @return double The minimum value
     */
    double min() const;
    /**
     * @brief Get the maximum value of the thermometer
     *
     * @return double The maximum value
     */
    double max() const;

    /**
     * @brief Set the start color object
     *
     * @param color
     */
    void set_start_color(Color color);
    /**
     * @brief Set the end color object
     *
     * @param color
     */
    void set_end_color(Color color);
    /**
     * @brief Add a color stop at the given value
     *
     * @param value The value at which to add the color stop
     * @param color The color of the stop
     */
    void add_color_stop(double value, Color color);
    /**
     * @brief Add a normalized color stop at the given value
     *
     * @param value The normalized value (0.0 to 1.0) at which to add the color stop
     * @param color The color of the stop
     */
    void add_color_stop_normalized(double value, Color color);
    /**
     * @brief Remove the color stop at the given value
     *
     * @param value The value of the color stop to remove
     */
    void remove_color_stop(double value);
    /**
     * @brief Remove the normalized color stop at the given value
     *
     * @param value The normalized value (0.0 to 1.0) of the color stop to remove
     */
    void remove_color_stop_normalized(double value);

    /**
     * @brief Deserialize the thermometer from JSON
     *
     * @param j The JSON object
     */
    void from_json(const json &j) override;
    /**
     * @brief Serialize the thermometer to JSON
     *
     * @param id The id of the thermometer
     * @return json The JSON representation of the thermometer
     */
    json to_json(size_t id) const override;

  private:
    double m_min;
    double m_max;
    double m_value;
    std::string m_unit;
    std::unordered_map<double, Color> m_color_map;
};
