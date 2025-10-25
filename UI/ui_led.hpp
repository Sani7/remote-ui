#pragma once
#include "ui_item.hpp"
#include <string>

#define UI_LED_TYPE std::string("ui_led")

/**
 * @brief Class representing a UI led
 *
 */
class UI_led : public UI_item
{
    Q_OBJECT
  public:
    /**
     * @brief Construct a UI led object
     *
     * @param parent The parent QObject
     */
    UI_led(QObject *parent = nullptr);
    /**
     * @brief Construct a UI led object
     *
     * @param text The text to display
     * @param fg_color The foreground color
     * @param bg_color The background color
     * @param parent The parent QObject
     */
    UI_led(std::string text, Color fg_color, Color bg_color, QObject *parent = nullptr);

    /**
     * @brief Set the state of the LED
     *
     * @param state The state to set
     * @param on The color when the LED is on
     * @param off The color when the LED is off
     * @return true If the state was changed
     * @return false If the state was not changed
     */
    bool set_led(bool state, Color on, Color off);

    /**
     * @brief Deserialize the LED from JSON
     *
     * @param j The JSON object
     */
    void from_json(const json &j) override;
    /**
     * @brief Serialize the LED to JSON
     *
     * @param id The id of the LED
     * @return json The JSON representation of the LED
     */
    json to_json(size_t id) const override;
};
