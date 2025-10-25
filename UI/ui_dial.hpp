#pragma once
#include "ui_item.hpp"

#define UI_DIAL_TYPE std::string("ui_dial")

/**
 * @brief Class representing a UI dial
 *
 */
class UI_dial : public UI_item
{
    Q_OBJECT
  public:
    /**
     * @brief Construct a UI dial object
     *
     * @param text The text to display
     * @param unit The unit of the dial
     * @param fg_color The foreground color
     * @param bg_color The background color
     * @param min The minimum value
     * @param max The maximum value
     * @param value The initial value
     * @param parent The parent QObject
     */
    UI_dial(std::string text, std::string unit, Color fg_color, Color bg_color, double min, double max, double value,
            QObject *parent = nullptr);
    /**
     * @brief Construct a UI dial object
     *
     * @param text The text to display
     * @param unit The unit of the dial
     * @param min The minimum value
     * @param max The maximum value
     * @param value The initial value
     * @param parent The parent QObject
     */
    UI_dial(std::string text, std::string unit, double min, double max, double value, QObject *parent = nullptr);
    /**
     * @brief Construct a UI dial object
     *
     * @param text The text to display
     * @param fg_color The foreground color
     * @param bg_color The background color
     * @param min The minimum value
     * @param max The maximum value
     * @param value The initial value
     * @param parent The parent QObject
     */
    UI_dial(std::string text, Color fg_color, Color bg_color, double min, double max, double value,
            QObject *parent = nullptr);
    /**
     * @brief Construct a UI dial object
     *
     * @param text The text to display
     * @param min The minimum value
     * @param max The maximum value
     * @param value The initial value
     * @param parent The parent QObject
     */
    UI_dial(std::string text, double min, double max, double value, QObject *parent = nullptr);
    /**
     * @brief Construct a UI dial object
     *
     * @param parent The parent QObject
     */
    UI_dial(QObject *parent = nullptr);

    /**
     * @brief Set the value of the dial
     *
     * @param value The value to set
     */
    void set_value(double value) override;
    /**
     * @brief Get the value of the dial
     *
     * @return double The current value
     */
    double value() const;

    /**
     * @brief Get the minimum value of the dial
     *
     * @return double The minimum value
     */
    double min() const;
    /**
     * @brief Get the maximum value of the dial
     *
     * @return double The maximum value
     */
    double max() const;
    /**
     * @brief Get the unit of the dial
     *
     * @return std::string The unit of the dial
     */
    std::string unit() const;

    /**
     * @brief Deserialize the dial from JSON
     *
     * @param j The JSON object
     */
    void from_json(const json &j) override;
    /**
     * @brief Serialize the dial to JSON
     *
     * @param id The id of the dial
     * @return json The JSON representation of the dial
     */
    json to_json(size_t id) const override;

  private:
    double m_min;
    double m_max;
    double m_value;
    std::string m_unit;
};
